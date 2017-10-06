#include <boss.hpp>
#include "boss_storage.hpp"

// 단위객체
class StorageUnit
{
public:
    Storage::DeleteCB Delete;
    ClearType Type;
    void* Ptr;
public:
    StorageUnit() {Delete = nullptr; Type = CT_Null; Ptr = nullptr;}
    ~StorageUnit() {}
};
typedef Array<StorageUnit, datatype_class_canmemcpy, 256> StorageUnits;

// 전역변수
static sint32 boss_storage_lastindex = -1;
#if HAS_CXX11_THREAD_LOCAL
    thread_local StorageUnits* boss_storage_root;
#elif defined(_MSC_VER)
    __declspec(thread) StorageUnits* boss_storage_root;
#elif defined(__GNUC__)
    #if BOSS_IPHONE
        static StorageUnits* boss_storage_root; // 수정해야 함!!!!!
    #else
        __thread StorageUnits* boss_storage_root;
    #endif
#else
    #error Unknown compiler
#endif

// 관리객체
class StorageClass
{
public:
    const sint32 Index;

public:
    StorageUnit& GetLocalUnit()
    {
        if(!boss_storage_root)
            boss_storage_root = new StorageUnits();
        return boss_storage_root->AtWherever(Index);
    }

    bool ClearLocalData()
    {
        StorageUnit& CurUnit = GetLocalUnit();
        if(CurUnit.Ptr)
        {
            CurUnit.Delete(CurUnit.Ptr);
            CurUnit.Ptr = nullptr;
            return true;
        }
        return false;
    }

    static sint32 ClearLocalDataAll(ClearType type)
    {
        sint32 ClearCount = 0;
        if(boss_storage_root)
        for(sint32 i = boss_storage_root->Count() - 1; 0 <= i; --i)
        {
            StorageUnit& CurUnit = boss_storage_root->At(i);
            if(CurUnit.Type == type && CurUnit.Ptr)
            {
                CurUnit.Delete(CurUnit.Ptr);
                CurUnit.Ptr = nullptr;
                ClearCount++;
            }
        }
        return ClearCount;
    }

public:
    StorageClass(Storage::DeleteCB cb, ClearType type) : Index(++boss_storage_lastindex)
    {
        StorageUnit& NewUnit = GetLocalUnit();
        NewUnit.Delete = cb;
        NewUnit.Type = type;
    }
    ~StorageClass() {}
};

namespace BOSS
{
    id_storage Storage::Create(DeleteCB cb, ClearType type)
    {
        BOSS_ASSERT("cb인수가 nullptr입니다", cb);
        StorageClass* NewStorage = new StorageClass(cb, type);
        return (id_storage) NewStorage;
    }

    void** Storage::Bind(id_storage storage)
    {
        BOSS_ASSERT("storage인수가 nullptr입니다", storage);
        StorageClass* CurStorage = (StorageClass*) storage;
        StorageUnit& CurUnit = CurStorage->GetLocalUnit();
        return &CurUnit.Ptr;
    }

    bool Storage::Clear(id_storage storage)
    {
        BOSS_ASSERT("storage인수가 nullptr입니다", storage);
        StorageClass* CurStorage = (StorageClass*) storage;
        return CurStorage->ClearLocalData();
    }

    sint32 Storage::ClearAll(ClearLevel level)
    {
        sint32 ClearCount = StorageClass::ClearLocalDataAll(CT_User);
        if(level == CL_SystemAndUser)
            ClearCount += StorageClass::ClearLocalDataAll(CT_System);
        return ClearCount;
    }
}
