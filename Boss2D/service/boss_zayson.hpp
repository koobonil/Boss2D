#pragma once
#include <service/boss_zay.hpp>
#include <element/boss_solver.hpp>

#define ZAY_EXTEND(...) if(auto _ = (__VA_ARGS__))

namespace BOSS
{
    class ZayUIElement;

    //! \brief 뷰의 확장스타일
    class ZayExtend
    {
    public:
        class Params;
        typedef ZayPanel::StackBinder (*Component)(ZayPanel& panel, const Params& params);

    public:
        ZayExtend(Component com = nullptr);
        ~ZayExtend();

    public:
        void Reset(Component com);

    public: // 파라미터와 함께 함수호출
        class Params
        {
        public:
            Params(Component com, id_cloned_share param = nullptr);
            ~Params();

        public:
            Params& operator()(sint32 value);
            Params& operator()(sint64 value);
            Params& operator()(float value);
            Params& operator()(double value);
            ZayPanel::StackBinder operator>>(ZayPanel& panel) const;

        public:
            sint32 Count() const;
            id_cloned_share Take(sint32 i) const;

        private:
            void AddParam(id_cloned_share param);

        private:
            Component mCom;
            Remote::Params mParams;
        };
        Params operator()() const;
        Params operator()(sint32 value) const;
        Params operator()(sint64 value) const;
        Params operator()(float value) const;
        Params operator()(double value) const;

    private:
        Component mCom;
    };

    //! \brief 뷰의 스크립트운영
    class ZaySon
    {
    public:
        ZaySon();
        ~ZaySon();

    public:
        void Load(const Context& context);
        void AddComponent(chars name, ZayExtend::Component com);
        const ZayExtend* FindComponent(chars name) const;
        sint32 Render(ZayPanel& panel, sint32 compmax = 0x7FFFFFFF);

    private:
        ZayUIElement* mView;
        Map<ZayExtend> mComponentMap;

    public:
        mutable String mDebugCompName;
    };
}
