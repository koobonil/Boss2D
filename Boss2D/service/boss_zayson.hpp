#pragma once
#include <service/boss_zay.hpp>

#define ZAY_EXTEND(...) if(auto _ = __VA_ARGS__)

namespace BOSS
{
    //! \brief 뷰의 확장스타일
    class ZayExtend
    {
    public:
        typedef ZayPanel::StackBinder (*CallBack)(ZayPanel& panel);

    public:
        ZayExtend(CallBack cb = nullptr);
        ~ZayExtend();

    public:
        void Reset(CallBack cb);

    public: // 함수호출
        class ZayParam
        {
        public:
            ZayParam(CallBack cb = nullptr);
            ~ZayParam();

        public:
            ZayPanel::StackBinder operator>>(ZayPanel& panel) const;

        private:
            CallBack mCB;
        };
        const ZayParam operator()() const;
        const ZayParam operator()(sint32 value) const;

    private:
        CallBack mCB;
    };

    class ZayUIElement;

    //! \brief 뷰의 스크립트운영
    class ZaySon
    {
    public:
        ZaySon();
        ~ZaySon();

    public:
        void Load(const Context& context);
        void AddFunction(chars name, ZayExtend::CallBack cb);
        const ZayExtend* FindFunction(chars name) const;
        void Render(ZayPanel& panel);

    private:
        ZayUIElement* mView;
        Map<ZayExtend> mFunctionMap;
    };
}
