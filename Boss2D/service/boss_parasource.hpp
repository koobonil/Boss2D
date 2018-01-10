#pragma once
#include <service/boss_zay.hpp>

namespace BOSS
{
    //! \brief 웹게시물형 리소스파서
    class ContactPool;
    class ParaSource
    {
    public:
        class View
        {
        public:
            View();
            virtual ~View();

        public:
            void Init(chars script);
            virtual ZayPanel::SubRenderCB GetRenderer();

        protected:
            id_tasking mTasking;
            Image mImage;
            uint64 mLoadingMsec;
        };

    public:
        enum Type {IIS, NaverCafe};

    public:
        ParaSource(Type type);
        ~ParaSource();

    public:
        inline chars GetLastError() const {return mLastError;}

    public:
        void SetContact(chars domain, uint16 port, sint32 timeout = 5000);
        bool GetFile(uint08s& file, chars path, chars args = nullptr);
        bool GetJson(Context& json, chars path, chars args = nullptr);
        bool GetJsons(Contexts& jsons, chars path, chars args = nullptr);
        bool GetLastSpecialJson(Context& json);

    private:
        typedef bool (*JsonLoaderCB)(payload data, chars content, sint32 length);
        bool LoadJsons(JsonLoaderCB cb, payload data, chars path, chars args);
        bool HTTPQuery(chars path, chars args);
        bool SetLastError(chars message);

    private:
        const Type mType;
        chararray mResponseData;
        ContactPool* mLastContact;
        String mLastError;
    };
}
