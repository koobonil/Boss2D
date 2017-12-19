#pragma once
#include <boss.hpp>

namespace BOSS
{
    //! \brief 웹게시물형 리소스파서
    class ContactPool;
    class ParaSource
    {
    public:
        ParaSource();
        ~ParaSource();

    public:
        void SetContact(chars domain, uint16 port, sint32 timeout = 5000);
        bool GetFile(uint08s& file, chars path, chars args = nullptr);
        bool GetJson(Contexts& jsons, chars path, chars args = nullptr);

    public:
        inline chars GetLastError() const {return mLastError;}

    private:
        bool HTTPQuery(chars path, chars args);

    private:
        chararray mResponseText;
        ContactPool* mLastContact;
        String mLastError;
    };
}
