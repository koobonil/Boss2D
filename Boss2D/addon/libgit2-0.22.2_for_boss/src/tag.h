/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_tag_h__
#define INCLUDE_tag_h__

#include BOSS_LIBGIT2_U_git2__tag_h //original-code:"git2/tag.h"
#include BOSS_LIBGIT2_U_repository_h //original-code:"repository.h"
#include BOSS_LIBGIT2_U_odb_h //original-code:"odb.h"

struct git_tag {
	git_object object;

	git_oid target;
	git_otype type;

	char *tag_name;
	git_signature *tagger;
	char *message;
};

void git_tag__free(void *tag);
int git_tag__parse(void *tag, git_odb_object *obj);

#endif
