/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */

#include BOSS_LIBGIT2_U_common_h //original-code:"common.h"
#include BOSS_LIBGIT2_V_zlib_h //original-code:<zlib.h>
#include BOSS_LIBGIT2_U_git2__object_h //original-code:"git2/object.h"
#include BOSS_LIBGIT2_U_git2__sys__odb_backend_h //original-code:"git2/sys/odb_backend.h"
#include "fileops.h"
#include BOSS_LIBGIT2_U_hash_h //original-code:"hash.h"
#include BOSS_LIBGIT2_U_odb_h //original-code:"odb.h"
#include "delta-apply.h"
#include "filter.h"
#include BOSS_LIBGIT2_U_repository_h //original-code:"repository.h"

#include BOSS_LIBGIT2_U_git2__odb_backend_h //original-code:"git2/odb_backend.h"
#include BOSS_LIBGIT2_U_git2__oid_h //original-code:"git2/oid.h"

#define GIT_ALTERNATES_FILE "info/alternates"

/* TODO: is this correct? */
#define GIT_LOOSE_PRIORITY 2
#define GIT_PACKED_PRIORITY 1

#define GIT_ALTERNATES_MAX_DEPTH 5

typedef struct
{
	git_odb_backend *backend;
	int priority;
	bool is_alternate;
	ino_t disk_inode;
} backend_internal;

static git_cache *odb_cache(git_odb *odb)
{
	if (odb->rc.owner != NULL) {
		git_repository *owner = odb->rc.owner;
		return &owner->objects;
	}

	return &odb->own_cache;
}

static int load_alternates(git_odb *odb, const char *objects_dir, int alternate_depth);

int git_odb__format_object_header(char *hdr, size_t n, size_t obj_len, git_otype obj_type)
{
	const char *type_str = git_object_type2string(obj_type);
	int len = p_snprintf(hdr, n, "%s %"PRIuZ, type_str, obj_len);
	assert(len > 0 && len <= (int)n);
	return len+1;
}

int git_odb__hashobj(git_oid *id, git_rawobj *obj)
{
	git_buf_vec vec[2];
	char header[64];
	int hdrlen;

	assert(id && obj);

	if (!git_object_typeisloose(obj->type))
		return -1;

	if (!obj->data && obj->len != 0)
		return -1;

	hdrlen = git_odb__format_object_header(header, sizeof(header), obj->len, obj->type);

	vec[0].data = header;
	vec[0].len = hdrlen;
	vec[1].data = obj->data;
	vec[1].len = obj->len;

	git_hash_vec(id, vec, 2);

	return 0;
}


static git_odb_object *odb_object__alloc(const git_oid *oid, git_rawobj *source)
{
	git_odb_object *object = git__calloc(1, sizeof(git_odb_object));

	if (object != NULL) {
		git_oid_cpy(&object->cached.oid, oid);
		object->cached.type = source->type;
		object->cached.size = source->len;
		object->buffer      = source->data;
	}

	return object;
}

void git_odb_object__free(void *object)
{
	if (object != NULL) {
		git__free(((git_odb_object *)object)->buffer);
		git__free(object);
	}
}

const git_oid *git_odb_object_id(git_odb_object *object)
{
	return &object->cached.oid;
}

const void *git_odb_object_data(git_odb_object *object)
{
	return object->buffer;
}

size_t git_odb_object_size(git_odb_object *object)
{
	return object->cached.size;
}

git_otype git_odb_object_type(git_odb_object *object)
{
	return object->cached.type;
}

int git_odb_object_dup(git_odb_object **dest, git_odb_object *source)
{
	git_cached_obj_incref(source);
	*dest = source;
	return 0;
}

void git_odb_object_free(git_odb_object *object)
{
	if (object == NULL)
		return;

	git_cached_obj_decref(object);
}

int git_odb__hashfd(git_oid *out, git_file fd, size_t size, git_otype type)
{
	int hdr_len;
	char hdr[64], buffer[2048];
	git_hash_ctx ctx;
	ssize_t read_len = 0;
	int error = 0;

	if (!git_object_typeisloose(type)) {
		giterr_set(GITERR_INVALID, "Invalid object type for hash");
		return -1;
	}

	if ((error = git_hash_ctx_init(&ctx)) < 0)
		return -1;

	hdr_len = git_odb__format_object_header(hdr, sizeof(hdr), size, type);

	if ((error = git_hash_update(&ctx, hdr, hdr_len)) < 0)
		goto done;

	while (size > 0 && (read_len = p_read(fd, buffer, sizeof(buffer))) > 0) {
		if ((error = git_hash_update(&ctx, buffer, read_len)) < 0)
			goto done;

		size -= read_len;
	}

	/* If p_read returned an error code, the read obviously failed.
	 * If size is not zero, the file was truncated after we originally
	 * stat'd it, so we consider this a read failure too */
	if (read_len < 0 || size > 0) {
		giterr_set(GITERR_OS, "Error reading file for hashing");
		error = -1;

		goto done;
	}

	error = git_hash_final(out, &ctx);

done:
	git_hash_ctx_cleanup(&ctx);
	return error;
}

int git_odb__hashfd_filtered(
	git_oid *out, git_file fd, size_t size, git_otype type, git_filter_list *fl)
{
	int error;
	git_buf raw = GIT_BUF_INIT;

	if (!fl)
		return git_odb__hashfd(out, fd, size, type);

	/* size of data is used in header, so we have to read the whole file
	 * into memory to apply filters before beginning to calculate the hash
	 */

	if (!(error = git_futils_readbuffer_fd(&raw, fd, size))) {
		git_buf post = GIT_BUF_INIT;

		error = git_filter_list_apply_to_data(&post, fl, &raw);

		git_buf_free(&raw);

		if (!error)
			error = git_odb_hash(out, post.ptr, post.size, type);

		git_buf_free(&post);
	}

	return error;
}

int git_odb__hashlink(git_oid *out, const char *path)
{
	struct stat st;
	git_off_t size;
	int result;

	if (git_path_lstat(path, &st) < 0)
		return -1;

	size = st.st_size;

	if (!git__is_sizet(size)) {
		giterr_set(GITERR_OS, "File size overflow for 32-bit systems");
		return -1;
	}

	if (S_ISLNK(st.st_mode)) {
		char *link_data;
		ssize_t read_len;

		link_data = git__malloc((size_t)(size + 1));
		GITERR_CHECK_ALLOC(link_data);

		read_len = p_readlink(path, link_data, (size_t)size);
		link_data[size] = '\0';
		if (read_len != (ssize_t)size) {
			giterr_set(GITERR_OS, "Failed to read symlink data for '%s'", path);
			git__free(link_data);
			return -1;
		}

		result = git_odb_hash(out, link_data, (size_t)size, GIT_OBJ_BLOB);
		git__free(link_data);
	} else {
		int fd = git_futils_open_ro(path);
		if (fd < 0)
			return -1;
		result = git_odb__hashfd(out, fd, (size_t)size, GIT_OBJ_BLOB);
		p_close(fd);
	}

	return result;
}

int git_odb_hashfile(git_oid *out, const char *path, git_otype type)
{
	git_off_t size;
	int result, fd = git_futils_open_ro(path);
	if (fd < 0)
		return fd;

	if ((size = git_futils_filesize(fd)) < 0 || !git__is_sizet(size)) {
		giterr_set(GITERR_OS, "File size overflow for 32-bit systems");
		p_close(fd);
		return -1;
	}

	result = git_odb__hashfd(out, fd, (size_t)size, type);
	p_close(fd);
	return result;
}

int git_odb_hash(git_oid *id, const void *data, size_t len, git_otype type)
{
	git_rawobj raw;

	assert(id);

	raw.data = (void *)data;
	raw.len = len;
	raw.type = type;

	return git_odb__hashobj(id, &raw);
}

/**
 * FAKE WSTREAM
 */

typedef struct {
	git_odb_stream stream;
	char *buffer;
	size_t size, written;
	git_otype type;
} fake_wstream;

static int fake_wstream__fwrite(git_odb_stream *_stream, const git_oid *oid)
{
	fake_wstream *stream = (fake_wstream *)_stream;
	return _stream->backend->write(_stream->backend, oid, stream->buffer, stream->size, stream->type);
}

static int fake_wstream__write(git_odb_stream *_stream, const char *data, size_t len)
{
	fake_wstream *stream = (fake_wstream *)_stream;

	if (stream->written + len > stream->size)
		return -1;

	memcpy(stream->buffer + stream->written, data, len);
	stream->written += len;
	return 0;
}

static void fake_wstream__free(git_odb_stream *_stream)
{
	fake_wstream *stream = (fake_wstream *)_stream;

	git__free(stream->buffer);
	git__free(stream);
}

static int init_fake_wstream(git_odb_stream **stream_p, git_odb_backend *backend, size_t size, git_otype type)
{
	fake_wstream *stream;

	stream = git__calloc(1, sizeof(fake_wstream));
	GITERR_CHECK_ALLOC(stream);

	stream->size = size;
	stream->type = type;
	stream->buffer = git__malloc(size);
	if (stream->buffer == NULL) {
		git__free(stream);
		return -1;
	}

	stream->stream.backend = backend;
	stream->stream.read = NULL; /* read only */
	stream->stream.write = &fake_wstream__write;
	stream->stream.finalize_write = &fake_wstream__fwrite;
	stream->stream.free = &fake_wstream__free;
	stream->stream.mode = GIT_STREAM_WRONLY;

	*stream_p = (git_odb_stream *)stream;
	return 0;
}

/***********************************************************
 *
 * OBJECT DATABASE PUBLIC API
 *
 * Public calls for the ODB functionality
 *
 ***********************************************************/

static int backend_sort_cmp(const void *a, const void *b)
{
	const backend_internal *backend_a = (const backend_internal *)(a);
	const backend_internal *backend_b = (const backend_internal *)(b);

	if (backend_a->is_alternate == backend_b->is_alternate)
		return (backend_b->priority - backend_a->priority);

	return backend_a->is_alternate ? 1 : -1;
}

int git_odb_new(git_odb **out)
{
	git_odb *db = git__calloc(1, sizeof(*db));
	GITERR_CHECK_ALLOC(db);

	if (git_cache_init(&db->own_cache) < 0 ||
		git_vector_init(&db->backends, 4, backend_sort_cmp) < 0) {
		git__free(db);
		return -1;
	}

	*out = db;
	GIT_REFCOUNT_INC(db);
	return 0;
}

static int add_backend_internal(
	git_odb *odb, git_odb_backend *backend,
	int priority, bool is_alternate, ino_t disk_inode)
{
	backend_internal *internal;

	assert(odb && backend);

	GITERR_CHECK_VERSION(backend, GIT_ODB_BACKEND_VERSION, "git_odb_backend");

	/* Check if the backend is already owned by another ODB */
	assert(!backend->odb || backend->odb == odb);

	internal = git__malloc(sizeof(backend_internal));
	GITERR_CHECK_ALLOC(internal);

	internal->backend = backend;
	internal->priority = priority;
	internal->is_alternate = is_alternate;
	internal->disk_inode = disk_inode;

	if (git_vector_insert(&odb->backends, internal) < 0) {
		git__free(internal);
		return -1;
	}

	git_vector_sort(&odb->backends);
	internal->backend->odb = odb;

	return 0;
}

int git_odb_add_backend(git_odb *odb, git_odb_backend *backend, int priority)
{
	return add_backend_internal(odb, backend, priority, false, 0);
}

int git_odb_add_alternate(git_odb *odb, git_odb_backend *backend, int priority)
{
	return add_backend_internal(odb, backend, priority, true, 0);
}

size_t git_odb_num_backends(git_odb *odb)
{
	assert(odb);
	return odb->backends.length;
}

static int git_odb__error_unsupported_in_backend(const char *action)
{
	giterr_set(GITERR_ODB,
		"Cannot %s - unsupported in the loaded odb backends", action);
	return -1;
}


int git_odb_get_backend(git_odb_backend **out, git_odb *odb, size_t pos)
{
	backend_internal *internal;

	assert(out && odb);
	internal = git_vector_get(&odb->backends, pos);

	if (internal && internal->backend) {
		*out = internal->backend;
		return 0;
	}

	giterr_set(GITERR_ODB, "No ODB backend loaded at index %" PRIuZ, pos);
	return GIT_ENOTFOUND;
}

static int add_default_backends(
	git_odb *db, const char *objects_dir,
	bool as_alternates, int alternate_depth)
{
	size_t i;
	struct stat st;
	ino_t inode;
	git_odb_backend *loose, *packed;

	/* TODO: inodes are not really relevant on Win32, so we need to find
	 * a cross-platform workaround for this */
#ifdef GIT_WIN32
	GIT_UNUSED(i);
	GIT_UNUSED(st);

	inode = 0;
#else
	if (p_stat(objects_dir, &st) < 0) {
		if (as_alternates)
			return 0;

		giterr_set(GITERR_ODB, "Failed to load object database in '%s'", objects_dir);
		return -1;
	}

	inode = st.st_ino;

	for (i = 0; i < db->backends.length; ++i) {
		backend_internal *backend = git_vector_get(&db->backends, i);
		if (backend->disk_inode == inode)
			return 0;
	}
#endif

	/* add the loose object backend */
	if (git_odb_backend_loose(&loose, objects_dir, -1, 0, 0, 0) < 0 ||
		add_backend_internal(db, loose, GIT_LOOSE_PRIORITY, as_alternates, inode) < 0)
		return -1;

	/* add the packed file backend */
	if (git_odb_backend_pack(&packed, objects_dir) < 0 ||
		add_backend_internal(db, packed, GIT_PACKED_PRIORITY, as_alternates, inode) < 0)
		return -1;

	return load_alternates(db, objects_dir, alternate_depth);
}

static int load_alternates(git_odb *odb, const char *objects_dir, int alternate_depth)
{
	git_buf alternates_path = GIT_BUF_INIT;
	git_buf alternates_buf = GIT_BUF_INIT;
	char *buffer;
	const char *alternate;
	int result = 0;

	/* Git reports an error, we just ignore anything deeper */
	if (alternate_depth > GIT_ALTERNATES_MAX_DEPTH)
		return 0;

	if (git_buf_joinpath(&alternates_path, objects_dir, GIT_ALTERNATES_FILE) < 0)
		return -1;

	if (git_path_exists(alternates_path.ptr) == false) {
		git_buf_free(&alternates_path);
		return 0;
	}

	if (git_futils_readbuffer(&alternates_buf, alternates_path.ptr) < 0) {
		git_buf_free(&alternates_path);
		return -1;
	}

	buffer = (char *)alternates_buf.ptr;

	/* add each alternate as a new backend; one alternate per line */
	while ((alternate = git__strtok(&buffer, "\r\n")) != NULL) {
		if (*alternate == '\0' || *alternate == '#')
			continue;

		/*
		 * Relative path: build based on the current `objects`
		 * folder. However, relative paths are only allowed in
		 * the current repository.
		 */
		if (*alternate == '.' && !alternate_depth) {
			if ((result = git_buf_joinpath(&alternates_path, objects_dir, alternate)) < 0)
				break;
			alternate = git_buf_cstr(&alternates_path);
		}

		if ((result = add_default_backends(odb, alternate, true, alternate_depth + 1)) < 0)
			break;
	}

	git_buf_free(&alternates_path);
	git_buf_free(&alternates_buf);

	return result;
}

int git_odb_add_disk_alternate(git_odb *odb, const char *path)
{
	return add_default_backends(odb, path, true, 0);
}

int git_odb_open(git_odb **out, const char *objects_dir)
{
	git_odb *db;

	assert(out && objects_dir);

	*out = NULL;

	if (git_odb_new(&db) < 0)
		return -1;

	if (add_default_backends(db, objects_dir, 0, 0) < 0) {
		git_odb_free(db);
		return -1;
	}

	*out = db;
	return 0;
}

static void odb_free(git_odb *db)
{
	size_t i;

	for (i = 0; i < db->backends.length; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *backend = internal->backend;

		if (backend->free) backend->free(backend);
		else git__free(backend);

		git__free(internal);
	}

	git_vector_free(&db->backends);
	git_cache_free(&db->own_cache);

	git__memzero(db, sizeof(*db));
	git__free(db);
}

void git_odb_free(git_odb *db)
{
	if (db == NULL)
		return;

	GIT_REFCOUNT_DEC(db, odb_free);
}

int git_odb_exists(git_odb *db, const git_oid *id)
{
	git_odb_object *object;
	size_t i;
	bool found = false;

	assert(db && id);

	if ((object = git_cache_get_raw(odb_cache(db), id)) != NULL) {
		git_odb_object_free(object);
		return (int)true;
	}

	for (i = 0; i < db->backends.length && !found; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (b->exists != NULL)
			found = (bool)b->exists(b, id);
	}

	return (int)found;
}

int git_odb_exists_prefix(
	git_oid *out, git_odb *db, const git_oid *short_id, size_t len)
{
	int error = GIT_ENOTFOUND, num_found = 0;
	size_t i;
	git_oid key = {{0}}, last_found = {{0}}, found;

	assert(db && short_id);

	if (len < GIT_OID_MINPREFIXLEN)
		return git_odb__error_ambiguous("prefix length too short");
	if (len > GIT_OID_HEXSZ)
		len = GIT_OID_HEXSZ;

	if (len == GIT_OID_HEXSZ) {
		if (git_odb_exists(db, short_id)) {
			if (out)
				git_oid_cpy(out, short_id);
			return 0;
		} else {
			return git_odb__error_notfound("no match for id prefix", short_id);
		}
	}

	/* just copy valid part of short_id */
	memcpy(&key.id, short_id->id, (len + 1) / 2);
	if (len & 1)
		key.id[len / 2] &= 0xF0;

	for (i = 0; i < db->backends.length; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (!b->exists_prefix)
			continue;

		error = b->exists_prefix(&found, b, &key, len);
		if (error == GIT_ENOTFOUND || error == GIT_PASSTHROUGH)
			continue;
		if (error)
			return error;

		/* make sure found item doesn't introduce ambiguity */
		if (num_found) {
			if (git_oid__cmp(&last_found, &found))
				return git_odb__error_ambiguous("multiple matches for prefix");
		} else {
			git_oid_cpy(&last_found, &found);
			num_found++;
		}
	}

	if (!num_found)
		return git_odb__error_notfound("no match for id prefix", &key);
	if (out)
		git_oid_cpy(out, &last_found);

	return 0;
}

int git_odb_read_header(size_t *len_p, git_otype *type_p, git_odb *db, const git_oid *id)
{
	int error;
	git_odb_object *object;

	error = git_odb__read_header_or_object(&object, len_p, type_p, db, id);

	if (object)
		git_odb_object_free(object);

	return error;
}

int git_odb__read_header_or_object(
	git_odb_object **out, size_t *len_p, git_otype *type_p,
	git_odb *db, const git_oid *id)
{
	size_t i;
	int error = GIT_ENOTFOUND;
	git_odb_object *object;

	assert(db && id && out && len_p && type_p);

	if ((object = git_cache_get_raw(odb_cache(db), id)) != NULL) {
		*len_p = object->cached.size;
		*type_p = object->cached.type;
		*out = object;
		return 0;
	}

	*out = NULL;

	for (i = 0; i < db->backends.length && error < 0; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (b->read_header != NULL)
			error = b->read_header(len_p, type_p, b, id);
	}

	if (!error || error == GIT_PASSTHROUGH)
		return 0;

	/*
	 * no backend could read only the header.
	 * try reading the whole object and freeing the contents
	 */
	if ((error = git_odb_read(&object, db, id)) < 0)
		return error; /* error already set - pass along */

	*len_p = object->cached.size;
	*type_p = object->cached.type;
	*out = object;

	return 0;
}

static git_oid empty_blob = {{ 0xe6, 0x9d, 0xe2, 0x9b, 0xb2, 0xd1, 0xd6, 0x43, 0x4b, 0x8b,
			       0x29, 0xae, 0x77, 0x5a, 0xd8, 0xc2, 0xe4, 0x8c, 0x53, 0x91 }};
static git_oid empty_tree = {{ 0x4b, 0x82, 0x5d, 0xc6, 0x42, 0xcb, 0x6e, 0xb9, 0xa0, 0x60,
			       0xe5, 0x4b, 0xf8, 0xd6, 0x92, 0x88, 0xfb, 0xee, 0x49, 0x04 }};

static int hardcoded_objects(git_rawobj *raw, const git_oid *id)
{
	if (!git_oid_cmp(id, &empty_blob)) {
		raw->type = GIT_OBJ_BLOB;
		raw->len = 0;
		raw->data = git__calloc(1, sizeof(uint8_t));
		return 0;
	} else if (!git_oid_cmp(id, &empty_tree)) {
		raw->type = GIT_OBJ_TREE;
		raw->len = 0;
		raw->data = git__calloc(1, sizeof(uint8_t));
		return 0;
	} else {
		return GIT_ENOTFOUND;
	}
}

int git_odb_read(git_odb_object **out, git_odb *db, const git_oid *id)
{
	size_t i, reads = 0;
	int error;
	git_rawobj raw;
	git_odb_object *object;

	assert(out && db && id);

	*out = git_cache_get_raw(odb_cache(db), id);
	if (*out != NULL)
		return 0;

	error = hardcoded_objects(&raw, id);

	for (i = 0; i < db->backends.length && error < 0; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (b->read != NULL) {
			++reads;
			error = b->read(&raw.data, &raw.len, &raw.type, b, id);
		}
	}

	if (error && error != GIT_PASSTHROUGH) {
		if (!reads)
			return git_odb__error_notfound("no match for id", id);
		return error;
	}

	giterr_clear();
	if ((object = odb_object__alloc(id, &raw)) == NULL)
		return -1;

	*out = git_cache_store_raw(odb_cache(db), object);
	return 0;
}

int git_odb_read_prefix(
	git_odb_object **out, git_odb *db, const git_oid *short_id, size_t len)
{
	size_t i;
	int error = GIT_ENOTFOUND;
	git_oid key = {{0}}, found_full_oid = {{0}};
	git_rawobj raw;
	void *data = NULL;
	bool found = false;
	git_odb_object *object;

	assert(out && db);

	if (len < GIT_OID_MINPREFIXLEN)
		return git_odb__error_ambiguous("prefix length too short");
	if (len > GIT_OID_HEXSZ)
		len = GIT_OID_HEXSZ;

	if (len == GIT_OID_HEXSZ) {
		*out = git_cache_get_raw(odb_cache(db), short_id);
		if (*out != NULL)
			return 0;
	}

	/* just copy valid part of short_id */
	memcpy(&key.id, short_id->id, (len + 1) / 2);
	if (len & 1)
		key.id[len / 2] &= 0xF0;

	for (i = 0; i < db->backends.length; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (b->read_prefix != NULL) {
			git_oid full_oid;
			error = b->read_prefix(&full_oid, &raw.data, &raw.len, &raw.type, b, &key, len);
			if (error == GIT_ENOTFOUND || error == GIT_PASSTHROUGH)
				continue;

			if (error)
				return error;

			git__free(data);
			data = raw.data;

			if (found && git_oid__cmp(&full_oid, &found_full_oid)) {
				git__free(raw.data);
				return git_odb__error_ambiguous("multiple matches for prefix");
			}

			found_full_oid = full_oid;
			found = true;
		}
	}

	if (!found)
		return git_odb__error_notfound("no match for prefix", &key);

	if ((object = odb_object__alloc(&found_full_oid, &raw)) == NULL)
		return -1;

	*out = git_cache_store_raw(odb_cache(db), object);
	return 0;
}

int git_odb_foreach(git_odb *db, git_odb_foreach_cb cb, void *payload)
{
	unsigned int i;
	backend_internal *internal;

	git_vector_foreach(&db->backends, i, internal) {
		git_odb_backend *b = internal->backend;
		int error = b->foreach(b, cb, payload);
		if (error < 0)
			return error;
	}

	return 0;
}

int git_odb_write(
	git_oid *oid, git_odb *db, const void *data, size_t len, git_otype type)
{
	size_t i;
	int error = GIT_ERROR;
	git_odb_stream *stream;

	assert(oid && db);

	git_odb_hash(oid, data, len, type);
	if (git_odb_exists(db, oid))
		return 0;

	for (i = 0; i < db->backends.length && error < 0; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		/* we don't write in alternates! */
		if (internal->is_alternate)
			continue;

		if (b->write != NULL)
			error = b->write(b, oid, data, len, type);
	}

	if (!error || error == GIT_PASSTHROUGH)
		return 0;

	/* if no backends were able to write the object directly, we try a
	 * streaming write to the backends; just write the whole object into the
	 * stream in one push
	 */
	if ((error = git_odb_open_wstream(&stream, db, len, type)) != 0)
		return error;

	stream->write(stream, data, len);
	error = stream->finalize_write(stream, oid);
	git_odb_stream_free(stream);

	return error;
}

static void hash_header_odb(git_hash_ctx *ctx, size_t size, git_otype type) //oz
{
	char header[64];
	int hdrlen;

	hdrlen = git_odb__format_object_header(header, sizeof(header), size, type);
	git_hash_update(ctx, header, hdrlen);
}

int git_odb_open_wstream(
	git_odb_stream **stream, git_odb *db, size_t size, git_otype type)
{
	size_t i, writes = 0;
	int error = GIT_ERROR;
	git_hash_ctx *ctx = NULL;

	assert(stream && db);

	for (i = 0; i < db->backends.length && error < 0; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		/* we don't write in alternates! */
		if (internal->is_alternate)
			continue;

		if (b->writestream != NULL) {
			++writes;
			error = b->writestream(stream, b, size, type);
		} else if (b->write != NULL) {
			++writes;
			error = init_fake_wstream(stream, b, size, type);
		}
	}

	if (error < 0) {
		if (error == GIT_PASSTHROUGH)
			error = 0;
		else if (!writes)
			error = git_odb__error_unsupported_in_backend("write object");

		goto done;
	}

	ctx = git__malloc(sizeof(git_hash_ctx));
	GITERR_CHECK_ALLOC(ctx);

	if ((error = git_hash_ctx_init(ctx)) < 0)
		goto done;

	hash_header_odb(ctx, size, type); //oz
	(*stream)->hash_ctx = ctx;

	(*stream)->declared_size = size;
	(*stream)->received_bytes = 0;

done:
	return error;
}

static int git_odb_stream__invalid_length(
	const git_odb_stream *stream,
	const char *action)
{
	giterr_set(GITERR_ODB,
		"Cannot %s - "
		"Invalid length. %"PRIuZ" was expected. The "
		"total size of the received chunks amounts to %"PRIuZ".",
		action, stream->declared_size, stream->received_bytes);		

	return -1;
}

int git_odb_stream_write(git_odb_stream *stream, const char *buffer, size_t len)
{
	git_hash_update(stream->hash_ctx, buffer, len);

	stream->received_bytes += len;

	if (stream->received_bytes > stream->declared_size)
		return git_odb_stream__invalid_length(stream,
			"stream_write()");

	return stream->write(stream, buffer, len);
}

int git_odb_stream_finalize_write(git_oid *out, git_odb_stream *stream)
{
	if (stream->received_bytes != stream->declared_size)
		return git_odb_stream__invalid_length(stream,
			"stream_finalize_write()");

	git_hash_final(out, stream->hash_ctx);

	if (git_odb_exists(stream->backend->odb, out))
		return 0;

	return stream->finalize_write(stream, out);
}

int git_odb_stream_read(git_odb_stream *stream, char *buffer, size_t len)
{
	return stream->read(stream, buffer, len);
}

void git_odb_stream_free(git_odb_stream *stream)
{
	if (stream == NULL)
		return;

	git_hash_ctx_cleanup(stream->hash_ctx);
	git__free(stream->hash_ctx);
	stream->free(stream);
}

int git_odb_open_rstream(git_odb_stream **stream, git_odb *db, const git_oid *oid)
{
	size_t i, reads = 0;
	int error = GIT_ERROR;

	assert(stream && db);

	for (i = 0; i < db->backends.length && error < 0; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (b->readstream != NULL) {
			++reads;
			error = b->readstream(stream, b, oid);
		}
	}

	if (error == GIT_PASSTHROUGH)
		error = 0;
	if (error < 0 && !reads)
		error = git_odb__error_unsupported_in_backend("read object streamed");

	return error;
}

int git_odb_write_pack(struct git_odb_writepack **out, git_odb *db, git_transfer_progress_cb progress_cb, void *progress_payload)
{
	size_t i, writes = 0;
	int error = GIT_ERROR;

	assert(out && db);

	for (i = 0; i < db->backends.length && error < 0; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		/* we don't write in alternates! */
		if (internal->is_alternate)
			continue;

		if (b->writepack != NULL) {
			++writes;
			error = b->writepack(out, b, db, progress_cb, progress_payload);
		}
	}

	if (error == GIT_PASSTHROUGH)
		error = 0;
	if (error < 0 && !writes)
		error = git_odb__error_unsupported_in_backend("write pack");

	return error;
}

void *git_odb_backend_malloc(git_odb_backend *backend, size_t len)
{
	GIT_UNUSED(backend);
	return git__malloc(len);
}

int git_odb_refresh(struct git_odb *db)
{
	size_t i;
	assert(db);

	for (i = 0; i < db->backends.length; ++i) {
		backend_internal *internal = git_vector_get(&db->backends, i);
		git_odb_backend *b = internal->backend;

		if (b->refresh != NULL) {
			int error = b->refresh(b);
			if (error < 0)
				return error;
		}
	}

	return 0;
}

int git_odb__error_notfound(const char *message, const git_oid *oid)
{
	if (oid != NULL) {
		char oid_str[GIT_OID_HEXSZ + 1];
		git_oid_tostr(oid_str, sizeof(oid_str), oid);
		giterr_set(GITERR_ODB, "Object not found - %s (%s)", message, oid_str);
	} else
		giterr_set(GITERR_ODB, "Object not found - %s", message);

	return GIT_ENOTFOUND;
}

int git_odb__error_ambiguous(const char *message)
{
	giterr_set(GITERR_ODB, "Ambiguous SHA1 prefix - %s", message);
	return GIT_EAMBIGUOUS;
}

int git_odb_init_backend(git_odb_backend *backend, unsigned int version)
{
	GIT_INIT_STRUCTURE_FROM_TEMPLATE(
		backend, version, git_odb_backend, GIT_ODB_BACKEND_INIT);
	return 0;
}
