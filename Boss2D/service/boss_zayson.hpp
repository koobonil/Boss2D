#pragma once
#include <service/boss_zay.hpp>
#include <element/boss_solver.hpp>

#define ZAY_EXTEND(...) if(auto _ = (__VA_ARGS__))

namespace BOSS
{
    class ZayUIElement;

    //! \brief 뷰의 확장함수관리
    class ZayExtend
    {
        BOSS_DECLARE_STANDARD_CLASS(ZayExtend)
    public:
        class Payload;
        enum class ComponentType {Unknown, Content, ContentWithParameter, Option, Layout, Loop, Condition, ConditionWithOperation, ConditionWithEvent};
        typedef std::function<ZayPanel::StackBinder(ZayPanel& panel, const Payload& params)> ComponentCB;
        typedef std::function<void(const Payload& params)> GlueCB;

    public:
        ZayExtend(ComponentType type = ComponentType::Unknown, ComponentCB ccb = nullptr, GlueCB gcb = nullptr);
        ~ZayExtend();

    public: // 파라미터와 함께 함수호출
        class Payload
        {
            BOSS_DECLARE_NONCOPYABLE_CLASS(Payload)
        public:
            Payload(const ZayExtend* owner, chars uiname = nullptr, const ZayUIElement* uielement = nullptr, const SolverValue* param = nullptr);
            ~Payload();

        public:
            Payload& operator()(const SolverValue& value);
            ZayPanel::StackBinder operator>>(ZayPanel& panel) const;

        public:
            chars UIName() const;
            ZayPanel::SubGestureCB MakeGesture() const;
            sint32 ParamCount() const;
            const SolverValue& Param(sint32 i) const;
            bool ParamToBool(sint32 i) const;
            UIAlign ParamToUIAlign(sint32 i) const;
            UIStretchForm ParamToUIStretchForm(sint32 i) const;
            UIFontAlign ParamToUIFontAlign(sint32 i) const;
            UIFontElide ParamToUIFontElide(sint32 i) const;

        private:
            void AddParam(const SolverValue& value);

        private:
            const ZayExtend* mOwner;
            chars mUIName;
            const ZayUIElement* mUIElement;
            SolverValues mParams;
        };
        const Payload operator()() const;
        Payload operator()(const SolverValue& value) const;

    public:
        bool HasComponent() const;
        bool HasContentComponent() const;
        bool HasGlue() const;
        void ResetForComponent(ComponentType type, ComponentCB cb);
        void ResetForGlue(GlueCB cb);
        Payload MakePayload(chars uiname = nullptr, const ZayUIElement* uielement = nullptr) const;

    private:
        ComponentType mComponentType;
        ComponentCB mComponentCB;
        GlueCB mGlueCB;
    };

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

    //! \brief 뷰의 스크립트운영
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
        ZayUIElement* mUIElement;
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

    class ZaySonUtility
    {
    public:
        static String GetSafetyString(chars text);
        static Strings GetCommaStrings(chars text);
        static bool IsFunctionCall(chars text, sint32* prmbegin = nullptr, sint32* prmend = nullptr);
        static ZaySonInterface::ConditionType ToCondition(chars text, bool* withelse = nullptr);
    };
}
