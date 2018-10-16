﻿#pragma once
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
        typedef std::function<ZayPanel::StackBinder(ZayPanel& panel, const Payload& params)> ComponentCB;
        typedef std::function<void(const Payload& params)> GlueCB;
    public:
        ZayExtend();
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
        bool HasGlue() const;
        void ResetForComponent(ComponentCB cb);
        void ResetForGlue(GlueCB cb);
        Payload MakePayload(chars uiname = nullptr, const ZayUIElement* uielement = nullptr) const;

    private:
        ComponentCB mComponentCB;
        GlueCB mGlueCB;
    };

    //! \brief 뷰의 스크립트운영
    class ZaySon
    {
        BOSS_DECLARE_NONCOPYABLE_CLASS(ZaySon)
    public:
        ZaySon();
        ~ZaySon();
        ZaySon(ZaySon&& rhs);
        ZaySon& operator=(ZaySon&& rhs);

    public:
        void Load(const Context& context);
        void AddComponent(chars name, ZayExtend::ComponentCB cb);
        void AddGlue(chars name, ZayExtend::GlueCB cb);
        const ZayExtend* FindComponent(chars name) const;
        const ZayExtend* FindGlue(chars name) const;
        sint32 Render(ZayPanel& panel, sint32 compmax = 0x7FFFFFFF);

    private:
        ZayUIElement* mUIElement;
        Map<ZayExtend> mExtendMap;

    public:
        mutable String mDebugCompName;
    };
}
