#pragma once
#include <service/boss_zay.hpp>
#include <functional>

namespace BOSS
{
    //! \brief 뷰스크립트 문서객체모델
    class ZaySonDocument
    {
        BOSS_DECLARE_NONCOPYABLE_CLASS(ZaySonDocument)
    public:
        ZaySonDocument(chars chain);
        ~ZaySonDocument();
        ZaySonDocument(ZaySonDocument&& rhs);
        ZaySonDocument& operator=(ZaySonDocument&& rhs);

    public:
        typedef std::function<void(const Solver*)> UpdateCB;

    public:
        void Add(chars variable, chars formula);
        void AddJson(const Context& json, const String nameheader = "");
        void AddFlush();
        void Update(chars variable, chars formula);
        void CheckUpdatedSolvers(uint64 msec, UpdateCB cb);
        void ReserverFlush();

    private:
        static void RemoveCB(chars itemname, payload data);
        class DownloadReserver
        {
            BOSS_DECLARE_STANDARD_CLASS(DownloadReserver)
        public:
            DownloadReserver();
            ~DownloadReserver();
        public:
            static sint32 OnTask(buffer& self, Queue<buffer>& query, Queue<buffer>& answer, id_common common);
        public:
            String mPath;
            String mFileName;
            String mUrl;
        };
        typedef Array<DownloadReserver> DownloadReservers;

    private:
        String mChain;
        Solvers mSolvers;
        sint32 mExecutedCount;
        DownloadReservers mReservers;
        id_tasking mReserverTask;
    };

    //! \brief 뷰스크립트 원형
    class ZaySonInterface
    {
    public:
        enum class ConditionType {Unknown, If, IfFocused, IfHovered, IfPressed, Else, Endif};
        enum class DataType {Unknown, ViewScript, ImageMap};
        enum class RequestType {Unknown, Function, Variable};

    public:
        virtual const String& ViewName() const = 0;
        virtual ZaySonInterface& AddComponent(ZayExtend::ComponentType type, chars name, ZayExtend::ComponentCB cb, chars paramcomment = nullptr) = 0;
        virtual ZaySonInterface& AddGlue(chars name, ZayExtend::GlueCB cb) = 0;
    };

    //! \brief 뷰스크립트 도구
    class ZaySonUtility
    {
    public:
        static String GetSafetyString(chars text);
        static Strings GetCommaStrings(chars text);
        static bool IsFunctionCall(chars text, sint32* prmbegin = nullptr, sint32* prmend = nullptr);
        static ZaySonInterface::ConditionType ToCondition(chars text, bool* withelse = nullptr);
    };

    //! \brief 뷰스크립트
    class ZaySon : public ZaySonInterface
    {
        BOSS_DECLARE_NONCOPYABLE_CLASS(ZaySon)
    public:
        ZaySon();
        ~ZaySon();
        ZaySon(ZaySon&& rhs);
        ZaySon& operator=(ZaySon&& rhs);

    public:
        void Load(chars viewname, const Context& context);
        void Reload(const Context& context);
        const String& ViewName() const override;
        ZaySonInterface& AddComponent(ZayExtend::ComponentType type, chars name, ZayExtend::ComponentCB cb, chars paramcomment = nullptr) override;
        ZaySonInterface& AddGlue(chars name, ZayExtend::GlueCB cb) override;
        const ZayExtend* FindComponent(chars name) const;
        const ZayExtend* FindGlue(chars name) const;
        sint32 Render(ZayPanel& panel, sint32 compmax = 0x7FFFFFFF);

    private:
        String mViewName;
        void* mUIElement;
        Map<ZayExtend> mExtendMap;

    public:
        void SetDebugCompName(chars name, chars comment) const;
        void SetDebugFocusedCompID(sint32 id = -1) const;
        void AddDebugError(chars name) const;
        chars debugCompName() const;
        sint32 debugFocusedCompID() const;
        sint32 debugErrorCountMax() const;
        chars debugErrorName(sint32 i) const;
        float debugErrorShowRate(sint32 i, bool countdown) const;

    private:
        mutable String mDebugCompName;
        mutable sint32 mDebugFocusedCompID;
        static const sint32 mDebugErrorCountMax = 10;
        static const sint32 mDebugErrorShowMax = 150;
        mutable sint32 mDebugErrorFocus;
        mutable String mDebugErrorName[mDebugErrorCountMax];
        mutable sint32 mDebugErrorShowCount[mDebugErrorCountMax];
    };
}
