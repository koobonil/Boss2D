#include <boss.hpp>
#include "boss_zayson.hpp"

namespace BOSS
{
    ////////////////////////////////////////////////////////////////////////////////
    // ZayUIElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayUIElement
    {
        BOSS_DECLARE_STANDARD_CLASS(ZayUIElement)
    public:
        enum class Type {Unknown, Condition, Asset, Value, Param, Component, View};

    public:
        ZayUIElement(Type type = Type::Unknown) : mType(type) {mRefRoot = nullptr;}
        virtual ~ZayUIElement() {}

    public:
        static void SetSolver(Solver& solver, chars variable, chars formula)
        {
            solver.Link("chain", variable, false);
            solver.Parse(formula);
            solver.Execute();
        }
        static SolverValue GetResult(chars formula)
        {
            Solver NewSolver;
            SetSolver(NewSolver, "_", formula);
            return NewSolver.result();
        }

    public:
        virtual void Load(const ZaySon& root, const Context& context)
        {
            mRefRoot = &root;
            hook(context("comment"))
                mComment = fish.GetString();
        }
        virtual void Render(ZayPanel& panel, const ZaySon& zayson, const String& uiname, sint32& compmax) const 
        {
        }
        virtual void OnTouch()
        {
        }

    public:
        static void TouchCore(const ZayUIElement* payload)
        {
            ((ZayUIElement*) payload)->OnTouch();
        }

    public:
        Type mType;
        const ZaySon* mRefRoot;
        String mComment;
    };
    typedef Object<ZayUIElement> ZayUI;
    typedef Array<ZayUI> ZayUIs;

    ////////////////////////////////////////////////////////////////////////////////
    // ZayConditionElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayConditionElement : public ZayUIElement
    {
    public:
        enum class ConditionType {Unknown, If, Elif, Else, Endif};

    public:
        ZayConditionElement() : ZayUIElement(Type::Condition)
        {mConditionType = ConditionType::Unknown;}
        ~ZayConditionElement() override {}

    private:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            const String ConditionText = context.GetString();
            branch;
            jump(!String::Compare("if", ConditionText, 2))
                mConditionType = ConditionType::If;
            jump(!String::Compare("elif", ConditionText, 4))
                mConditionType = ConditionType::Elif;
            jump(!String::Compare("else", ConditionText, 4))
                mConditionType = ConditionType::Else;
            jump(!String::Compare("endif", ConditionText, 5))
                mConditionType = ConditionType::Endif;

            if(sint32 PosB = ConditionText.Find(0, "(") + 1)
            {
                sint32 PosE = ConditionText.Find(PosB + 1, ")");
                if(PosE != -1)
                    mConditionSolver = String(((chars) ConditionText) + PosB, PosE - PosB);
            }
        }

    public:
        static bool Test(const ZaySon& root, ZayUIs& dest, const Context& src)
        {
            if(src.GetString(nullptr))
            {
                Object<ZayConditionElement> NewCondition(ObjectAllocType::Now);
                ((ZayUIElement*) NewCondition.Ptr())->Load(root, src);
                dest.AtAdding() = (id_share) NewCondition;
                return true;
            }
            return false;
        };
        static sint32s Collect(const ZayUIs& uis)
        {
            sint32s Collector;
            // 조건문처리로 유효한 CompValue를 수집
            for(sint32 i = 0, iend = uis.Count(); i < iend; ++i)
            {
                if(uis[i].ConstValue().mType == ZayUIElement::Type::Condition)
                {
                    auto CurCondition = (const ZayConditionElement*) uis[i].ConstPtr();
                    bool IsTrue = true;
                    if(CurCondition->mConditionType == ZayConditionElement::ConditionType::If ||
                        CurCondition->mConditionType == ZayConditionElement::ConditionType::Elif)
                        IsTrue = (ZayUIElement::GetResult(CurCondition->mConditionSolver).ToInteger() != 0);
                    if(IsTrue)
                    {
                        while(i + 1 < iend && uis[i + 1].ConstValue().mType != ZayUIElement::Type::Condition)
                            Collector.AtAdding() = ++i;
                        // 선진입
                        while(i + 1 < iend)
                        {
                            if(uis[++i].ConstValue().mType == ZayUIElement::Type::Condition)
                            {
                                CurCondition = (const ZayConditionElement*) uis[i].ConstPtr();
                                if(CurCondition->mConditionType == ZayConditionElement::ConditionType::Endif) // endif는 조건그룹을 빠져나오게 하고
                                    break;
                                else if(CurCondition->mConditionType == ZayConditionElement::ConditionType::If) // 새로운 조건그룹을 만나면 처리
                                {
                                    i--;
                                    break;
                                }
                            }
                        }
                    }
                    else while(i + 1 < iend)
                    {
                        if(uis[++i].ConstValue().mType == ZayUIElement::Type::Condition)
                        {
                            CurCondition = (const ZayConditionElement*) uis[i].ConstPtr();
                            if(CurCondition->mConditionType == ZayConditionElement::ConditionType::Endif) // Endif는 조건그룹을 빠져나오게 하고
                                break;
                            else // 다른 모든 조건은 처리
                            {
                                i--;
                                break;
                            }
                        }
                    }
                }
                else Collector.AtAdding() = i;
            }
            return Collector;
        }

    public:
        ConditionType mConditionType;
        String mConditionSolver;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayAssetElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayAssetElement : public ZayUIElement
    {
    public:
        enum class DataType {Unknown, ViewScript, ImageMap};

    public:
        ZayAssetElement() : ZayUIElement(Type::Asset)
        {mDataType = DataType::Unknown;}
        ~ZayAssetElement() override {}

    private:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            mDataName = context("dataname").GetString();
            const String Type = context("datatype").GetString();
            if(!Type.Compare("viewscript"))
                mDataType = DataType::ViewScript;
            else if(!Type.Compare("imagemap"))
                mDataType = DataType::ImageMap;
            mFilePath = context("filepath").GetString();
            mUrl = context("url").GetString();
        }

    public:
        String mDataName;
        DataType mDataType;
        String mFilePath;
        String mUrl;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayVariableElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayVariableElement : public ZayUIElement
    {
    public:
        ZayVariableElement() : ZayUIElement(Type::Value) {}
        ~ZayVariableElement() override {}

    private:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            chararray GetName;
            const auto& GetValue = context(0, &GetName);
            mValueName = &GetName[0];
            mValueFormula = GetValue.GetString();
        }

    public:
        void Bind()
        {
            mSolver.Link("chain", mValueName, false);
            mSolver.Parse(ZayUIElement::GetResult(mValueFormula).ToText());
            mSolver.Execute();
        }
        void Transform()
        {
            if(auto FindedSolver = Solver::Find("chain", mValueName))
            {
                FindedSolver->Parse(ZayUIElement::GetResult(mValueFormula).ToText());
                FindedSolver->Execute();
            }
        }

    public:
        String mValueName;
        String mValueFormula;
        Solver mSolver;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayParamElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayParamElement : public ZayUIElement
    {
    public:
        ZayParamElement() : ZayUIElement(Type::Param) {}
        ~ZayParamElement() override {}

    private:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            for(sint32 i = 0, iend = context.LengthOfIndexable(); i < iend; ++i)
                mParamFormulas.AtAdding() = context[i].GetString();
        }

    public:
        Strings mParamFormulas;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayComponentElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayComponentElement : public ZayUIElement
    {
    public:
        ZayComponentElement() : ZayUIElement(Type::Component) {}
        ~ZayComponentElement() override {}

    private:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            hook(context("compname"))
                mCompName = fish.GetString();

            hook(context("compvalues"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mCompValues, fish[i]))
                    continue;
                Object<ZayParamElement> NewParam(ObjectAllocType::Now);
                ((ZayUIElement*) NewParam.Ptr())->Load(root, fish[i]);
                mCompValues.AtAdding() = (id_share) NewParam;
            }

            hook(context("onclick"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mClickCodes, fish[i]))
                    continue;
                Object<ZayVariableElement> NewVariable(ObjectAllocType::Now);
                ((ZayUIElement*) NewVariable.Ptr())->Load(root, fish[i]);
                mClickCodes.AtAdding() = (id_share) NewVariable;
            }

            hook(context("ui"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mChildren, fish[i]))
                    continue;
                Object<ZayComponentElement> NewRenderer(ObjectAllocType::Now);
                ((ZayUIElement*) NewRenderer.Ptr())->Load(root, fish[i]);
                mChildren.AtAdding() = (id_share) NewRenderer;
            }
        }
        void Render(ZayPanel& panel, const ZaySon& zayson, const String& uiname, sint32& compmax) const override
        {
            if(0 < compmax)
            {
                if(--compmax == 0)
                {
                    // Debug정보구성
                    mRefRoot->mDebugCompName = mCompName;
                    if(0 < mComment.Length())
                        mRefRoot->mDebugCompName += '(' + mComment + ')';
                }

                if(auto CurComponent = zayson.FindComponent(mCompName))
                {
                    if(mCompValues.Count() == 0)
                    {
                        chars UIName = (0 < mClickCodes.Count())? (chars) uiname : nullptr;
                        ZayComponent::Payload ParamCollector = CurComponent->MakePayload(UIName, this);
                        ZAY_EXTEND(ParamCollector >> panel)
                            RenderChildren(panel, zayson, uiname, compmax);
                    }
                    else // CompValue항목이 존재할 경우
                    {
                        // 유효한 CompValue를 모두 실행
                        sint32s CollectedCompValues = ZayConditionElement::Collect(mCompValues);
                        for(sint32 i = 0, iend = CollectedCompValues.Count(); i < iend; ++i)
                        {
                            auto CurCompValue = (const ZayParamElement*) mCompValues[CollectedCompValues[i]].ConstPtr();
                            // 컴포넌트호출을 위한 파라미터계산 및 수집
                            const String UINameSub = String::Format("%s-V%d", (chars) uiname, CollectedCompValues[i]);
                            chars UIName = (0 < mClickCodes.Count())? (chars) UINameSub : nullptr;
                            ZayComponent::Payload ParamCollector = CurComponent->MakePayload(UIName, this);
                            if(CurCompValue && 0 < CurCompValue->mParamFormulas.Count())
                            {
                                Solvers LocalSolvers; // 파라미터계산용 패널정보 사전입력
                                ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.width", String::FromFloat(panel.w()));
                                ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.height", String::FromFloat(panel.h()));
                                for(sint32 j = 0, jend = CurCompValue->mParamFormulas.Count(); j < jend; ++j)
                                    ParamCollector(ZayUIElement::GetResult(CurCompValue->mParamFormulas[j]));
                            }
                            ZAY_EXTEND(ParamCollector >> panel)
                                RenderChildren(panel, zayson, UINameSub, compmax);
                        }
                    }
                }
            }
        }
        void RenderChildren(ZayPanel& panel, const ZaySon& zayson, const String& uiname, sint32& compmax) const
        {
            if(0 < mChildren.Count())
            {
                sint32s CollectedChildren = ZayConditionElement::Collect(mChildren);
                for(sint32 i = 0, iend = CollectedChildren.Count(); i < iend; ++i)
                {
                    auto CurChildren = (const ZayUIElement*) mChildren[CollectedChildren[i]].ConstPtr();
                    CurChildren->Render(panel, zayson, uiname + String::FromInteger(-1 - i), compmax);
                }
            }
        }
        void OnTouch() override
        {
            if(0 < mClickCodes.Count())
            {
                sint32s CollectedClickCodes = ZayConditionElement::Collect(mClickCodes);
                for(sint32 i = 0, iend = CollectedClickCodes.Count(); i < iend; ++i)
                {
                    auto CurClickCode = (ZayVariableElement*) mClickCodes.At(CollectedClickCodes[i]).Ptr();
                    CurClickCode->Transform();
                }
            }
        }

    public:
        String mCompName;
        ZayUIs mCompValues;
        ZayUIs mClickCodes;
        ZayUIs mChildren;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayViewElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayViewElement : public ZayUIElement
    {
    public:
        ZayViewElement() : ZayUIElement(Type::View) {}
        ~ZayViewElement() override {}

    private:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            hook(context("asset"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mAssets, fish[i]))
                    continue;
                Object<ZayAssetElement> NewResource(ObjectAllocType::Now);
                ((ZayUIElement*) NewResource.Ptr())->Load(root, fish[i]);
                mAssets.AtAdding() = (id_share) NewResource;
            }

            hook(context("oncreate"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mCreateCodes, fish[i]))
                    continue;
                Object<ZayVariableElement> NewVariable(ObjectAllocType::Now);
                ((ZayUIElement*) NewVariable.Ptr())->Load(root, fish[i]);
                NewVariable->Bind();
                mCreateCodes.AtAdding() = (id_share) NewVariable;
            }

            hook(context("ui"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mChildren, fish[i]))
                    continue;
                Object<ZayComponentElement> NewLayer(ObjectAllocType::Now);
                ((ZayUIElement*) NewLayer.Ptr())->Load(root, fish[i]);
                mChildren.AtAdding() = (id_share) NewLayer;
            }
        }
        void Render(ZayPanel& panel, const ZaySon& zayson, const String& uiname, sint32& compmax) const override
        {
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                if(mChildren[i]->mType == ZayUIElement::Type::Component)
                    mChildren[i]->Render(panel, zayson, uiname + String::FromInteger(-1 - i), compmax);
        }

    public:
        ZayUIs mAssets;
        ZayUIs mCreateCodes;
        ZayUIs mChildren;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayComponent
    ////////////////////////////////////////////////////////////////////////////////
    ZayComponent::ZayComponent(CallBack cb)
    {
        mCB = cb;
    }

    ZayComponent::~ZayComponent()
    {
    }

    ZayComponent::Payload::Payload(CallBack cb, chars uiname, const ZayUIElement* uielement, const SolverValue* param)
    {
        mCB = cb;
        mUIName = uiname;
        mUIElement = uielement;
        if(param) AddParam(*param);
    }

    ZayComponent::Payload::~Payload()
    {
    }

    ZayComponent::Payload& ZayComponent::Payload::operator()(const SolverValue& value)
    {AddParam(value); return *this;}

    ZayPanel::StackBinder ZayComponent::Payload::operator>>(ZayPanel& panel) const
    {return mCB(panel, *this);}

    chars ZayComponent::Payload::UIName() const
    {return mUIName;}

    ZayPanel::SubGestureCB ZayComponent::Payload::MakeGesture() const
    {
        auto UIElement = mUIElement;
        return ZAY_GESTURE_T(t, UIElement)
            {
                if(t == GT_InReleased)
                    ZayUIElement::TouchCore(UIElement);
            };
    }

    sint32 ZayComponent::Payload::ParamCount() const
    {return mParams.Count();}

    const SolverValue& ZayComponent::Payload::Param(sint32 i) const
    {return mParams[i];}

    bool ZayComponent::Payload::ParamToBool(sint32 i) const
    {
        if(mParams[i].GetType() == SolverValueType::Text)
        {
            if(!mParams[i].ToText().CompareNoCase("true"))
                return true;
            if(!mParams[i].ToText().CompareNoCase("false"))
                return false;
            BOSS_ASSERT("알 수 없는 Bool입니다", false);
        }
        return !!mParams[i].ToInteger();
    }

    UIAlign ZayComponent::Payload::ParamToUIAlign(sint32 i) const
    {
        const String Result = mParams[i].ToText();
        branch;
        jump(!Result.Compare("UIA_LeftTop")) return UIA_LeftTop;
        jump(!Result.Compare("UIA_CenterTop")) return UIA_CenterTop;
        jump(!Result.Compare("UIA_RightTop")) return UIA_RightTop;
        jump(!Result.Compare("UIA_LeftMiddle")) return UIA_LeftMiddle;
        jump(!Result.Compare("UIA_CenterMiddle")) return UIA_CenterMiddle;
        jump(!Result.Compare("UIA_RightMiddle")) return UIA_RightMiddle;
        jump(!Result.Compare("UIA_LeftBottom")) return UIA_LeftBottom;
        jump(!Result.Compare("UIA_CenterBottom")) return UIA_CenterBottom;
        jump(!Result.Compare("UIA_RightBottom")) return UIA_RightBottom;
        BOSS_ASSERT("알 수 없는 UIAlign입니다", false);
        return UIA_LeftTop;
    }

    UIStretchForm ZayComponent::Payload::ParamToUIStretchForm(sint32 i) const
    {
        const String Result = mParams[i].ToText();
        branch;
        jump(!Result.Compare("UISF_Strong")) return UISF_Strong;
        jump(!Result.Compare("UISF_Inner")) return UISF_Inner;
        jump(!Result.Compare("UISF_Outer")) return UISF_Outer;
        jump(!Result.Compare("UISF_Width")) return UISF_Width;
        jump(!Result.Compare("UISF_Height")) return UISF_Height;
        BOSS_ASSERT("알 수 없는 UIStretchForm입니다", false);
        return UISF_Strong;
    }

    UIFontAlign ZayComponent::Payload::ParamToUIFontAlign(sint32 i) const
    {
        const String Result = mParams[i].ToText();
        branch;
        jump(!Result.Compare("UIFA_LeftTop")) return UIFA_LeftTop;
        jump(!Result.Compare("UIFA_CenterTop")) return UIFA_CenterTop;
        jump(!Result.Compare("UIFA_RightTop")) return UIFA_RightTop;
        jump(!Result.Compare("UIFA_JustifyTop")) return UIFA_JustifyTop;
        jump(!Result.Compare("UIFA_LeftMiddle")) return UIFA_LeftMiddle;
        jump(!Result.Compare("UIFA_CenterMiddle")) return UIFA_CenterMiddle;
        jump(!Result.Compare("UIFA_RightMiddle")) return UIFA_RightMiddle;
        jump(!Result.Compare("UIFA_JustifyMiddle")) return UIFA_JustifyMiddle;
        jump(!Result.Compare("UIFA_LeftAscent")) return UIFA_LeftAscent;
        jump(!Result.Compare("UIFA_CenterAscent")) return UIFA_CenterAscent;
        jump(!Result.Compare("UIFA_RightAscent")) return UIFA_RightAscent;
        jump(!Result.Compare("UIFA_JustifyAscent")) return UIFA_JustifyAscent;
        jump(!Result.Compare("UIFA_LeftBottom")) return UIFA_LeftBottom;
        jump(!Result.Compare("UIFA_CenterBottom")) return UIFA_CenterBottom;
        jump(!Result.Compare("UIFA_RightBottom")) return UIFA_RightBottom;
        jump(!Result.Compare("UIFA_JustifyBottom")) return UIFA_JustifyBottom;
        BOSS_ASSERT("알 수 없는 UIFontAlign입니다", false);
        return UIFA_LeftTop;
    }

    UIFontElide ZayComponent::Payload::ParamToUIFontElide(sint32 i) const
    {
        const String Result = mParams[i].ToText();
        branch;
        jump(!Result.Compare("UIFE_None")) return UIFE_None;
        jump(!Result.Compare("UIFE_Left")) return UIFE_Left;
        jump(!Result.Compare("UIFE_Center")) return UIFE_Center;
        jump(!Result.Compare("UIFE_Right")) return UIFE_Right;
        BOSS_ASSERT("알 수 없는 UIFontElide입니다", false);
        return UIFE_None;
    }

    void ZayComponent::Payload::AddParam(const SolverValue& value)
    {mParams.AtAdding() = value;}

    const ZayComponent::Payload ZayComponent::operator()() const
    {return ZayComponent::Payload(mCB);}

    ZayComponent::Payload ZayComponent::operator()(const SolverValue& value) const
    {return ZayComponent::Payload(mCB, nullptr, nullptr, &value);}

    void ZayComponent::Reset(CallBack cb)
    {
        mCB = cb;
    }

    ZayComponent::Payload ZayComponent::MakePayload(chars uiname, const ZayUIElement* uielement) const
    {
        return ZayComponent::Payload(mCB, uiname, uielement);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ZaySon
    ////////////////////////////////////////////////////////////////////////////////
    ZaySon::ZaySon()
    {
        mView = nullptr;
    }

    ZaySon::~ZaySon()
    {
        delete mView;
    }

    void ZaySon::Load(const Context& context)
    {
        delete mView;
        mView = new ZayViewElement();
        mView->Load(*this, context);
    }

    void ZaySon::AddComponent(chars name, ZayComponent::CallBack cb)
    {
        auto& NewFunction = mComponentMap(name);
        NewFunction.Reset(cb);
    }

    const ZayComponent* ZaySon::FindComponent(chars name) const
    {
        if(auto FindedFunc = mComponentMap.Access(name))
            return FindedFunc;
        return nullptr;
    }

    sint32 ZaySon::Render(ZayPanel& panel, sint32 compmax)
    {
        if(mView)
        {
            mDebugCompName = "(null)";
            const sint32 OldCompMax = compmax;
            mView->Render(panel, *this, "View", compmax);
            return OldCompMax - compmax;
        }
        return 0;
    }
}
