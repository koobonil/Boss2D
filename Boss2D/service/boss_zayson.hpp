#pragma once
#include <service/boss_zay.hpp>
#include <element/boss_solver.hpp>

#define ZAY_EXTEND(...) if(auto _ = (__VA_ARGS__))

namespace BOSS
{
    class ZayUIElement;

    //! \brief 뷰의 UI콤포넌트
    class ZayComponent
    {
        BOSS_DECLARE_STANDARD_CLASS(ZayComponent)
    public:
        class Payload;
        typedef ZayPanel::StackBinder (*CallBack)(ZayPanel& panel, const Payload& params);
    public:
        ZayComponent(CallBack cb = nullptr);
        ~ZayComponent();

    public: // 파라미터와 함께 함수호출
        class Payload
        {
            BOSS_DECLARE_NONCOPYABLE_CLASS(Payload)
        public:
            Payload(CallBack cb, chars uiname = nullptr, const ZayUIElement* uielement = nullptr, id_cloned_share param = nullptr);
            ~Payload();

        public:
            Payload& operator()(sint32 value);
            Payload& operator()(sint64 value);
            Payload& operator()(float value);
            Payload& operator()(double value);
            ZayPanel::StackBinder operator>>(ZayPanel& panel) const;

        public:
            chars UIName() const;
            ZayPanel::SubGestureCB MakeGesture() const;
            sint32 ParamCount() const;
            id_cloned_share TakeParam(sint32 i) const;

        private:
            void AddParam(id_cloned_share param);

        private:
            CallBack mCB;
            chars mUIName;
            const ZayUIElement* mUIElement;
            Remote::Params mParams;
        };
        const Payload operator()() const;
        Payload operator()(sint32 value) const;
        Payload operator()(sint64 value) const;
        Payload operator()(float value) const;
        Payload operator()(double value) const;

    public:
        void Reset(CallBack cb);
        Payload MakePayload(chars uiname = nullptr, const ZayUIElement* uielement = nullptr) const;

    private:
        CallBack mCB;
    };

    //! \brief 뷰의 스크립트운영
    class ZaySon
    {
        BOSS_DECLARE_NONCOPYABLE_CLASS(ZaySon)
    public:
        ZaySon();
        ~ZaySon();

    public:
        void Load(const Context& context);
        void AddComponent(chars name, ZayComponent::CallBack cb);
        const ZayComponent* FindComponent(chars name) const;
        sint32 Render(ZayPanel& panel, sint32 compmax = 0x7FFFFFFF);

    private:
        ZayUIElement* mView;
        Map<ZayComponent> mComponentMap;

    public:
        mutable String mDebugCompName;
    };
}
