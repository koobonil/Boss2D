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
        enum class Type {Unknown, Condition, Asset, Param, Request, Component, View};

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

    protected:
        virtual void Load(const ZaySon& root, const Context& context)
        {
            mRefRoot = &root;
            hook(context("uiname"))
                mUIName = fish.GetString();
            hook(context("comment"))
                mComment = fish.GetString();
        }

    public:
        virtual void Render(ZayPanel& panel, const String& uiname, sint32& compmax) const 
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
        String mUIName;
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
        ZayConditionElement() : ZayUIElement(Type::Condition)
        {mConditionType = ZaySonInterface::ConditionType::Unknown;}
        ~ZayConditionElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            const String ConditionText = context.GetString();
            mConditionType = ZaySonInterface::ToCondition(ConditionText);
            if(mConditionType == ZaySonInterface::ConditionType::Unknown)
                mRefRoot->AddDebugError(String::Format("알 수 없는 조건문입니다(%s, Load)", (chars) ConditionText));

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
            if(ZaySonInterface::ToCondition(src.GetString("")) != ZaySonInterface::ConditionType::Unknown) // oncreate, onclick, compvalues의 경우
            {
                Object<ZayConditionElement> NewCondition(ObjectAllocType::Now);
                NewCondition->Load(root, src);
                dest.AtAdding() = (id_share) NewCondition;
                return true;
            }
            else if(ZaySonInterface::ToCondition(src("compname").GetString("")) != ZaySonInterface::ConditionType::Unknown) // compname의 경우
            {
                Object<ZayConditionElement> NewCondition(ObjectAllocType::Now);
                NewCondition->Load(root, src("compname"));
                dest.AtAdding() = (id_share) NewCondition;
                return true;
            }
            return false;
        };
        static sint32s Collect(const ZayUIElement* self, const ZayUIs& uis, const ZayPanel* panel = nullptr)
        {
            sint32s Collector;
            // 조건문처리로 유효한 CompValue를 수집
            for(sint32 i = 0, iend = uis.Count(); i < iend; ++i)
            {
                if(uis[i].ConstValue().mType == ZayUIElement::Type::Condition)
                {
                    // 조건의 성공여부
                    bool IsTrue = true;
                    auto CurCondition = (const ZayConditionElement*) uis[i].ConstPtr();
                    if(CurCondition->mConditionType == ZaySonInterface::ConditionType::If ||
                        CurCondition->mConditionType == ZaySonInterface::ConditionType::Elif)
                        IsTrue = (ZayUIElement::GetResult(CurCondition->mConditionSolver).ToInteger() != 0);
                    // 포커스확인
                    else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::IfFocused)
                    {
                        if(panel)
                        {
                            const String UIName = ZayUIElement::GetResult(CurCondition->mConditionSolver).ToText();
                            IsTrue = !!(panel->state(self->mRefRoot->UIName() + '.' + UIName) & PS_Focused);
                        }
                    }
                    // 호버확인
                    else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::IfHovered)
                    {
                        if(panel)
                        {
                            const String UIName = ZayUIElement::GetResult(CurCondition->mConditionSolver).ToText();
                            IsTrue = !!(panel->state(self->mRefRoot->UIName() + '.' + UIName) & PS_Hovered);
                        }
                    }
                    // 프레스확인
                    else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::IfPressed)
                    {
                        if(panel)
                        {
                            const String UIName = ZayUIElement::GetResult(CurCondition->mConditionSolver).ToText();
                            IsTrue = !!(panel->state(self->mRefRoot->UIName() + '.' + UIName) & PS_Dragging);
                        }
                    }

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
                                if(CurCondition->mConditionType == ZaySonInterface::ConditionType::Endif) // endif는 조건그룹을 빠져나오게 하고
                                    break;
                                else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::If || // 새로운 조건그룹을 만나면 수락
                                    CurCondition->mConditionType == ZaySonInterface::ConditionType::IfFocused ||
                                    CurCondition->mConditionType == ZaySonInterface::ConditionType::IfHovered ||
                                    CurCondition->mConditionType == ZaySonInterface::ConditionType::IfPressed)
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
                            if(CurCondition->mConditionType == ZaySonInterface::ConditionType::Endif) // endif는 조건그룹을 빠져나오게 하고
                                break;
                            else // 다른 모든 조건은 수락
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
        ZaySonInterface::ConditionType mConditionType;
        String mConditionSolver;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayAssetElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayAssetElement : public ZayUIElement
    {
    public:
        ZayAssetElement() : ZayUIElement(Type::Asset)
        {mDataType = ZaySonInterface::DataType::Unknown;}
        ~ZayAssetElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            mDataName = context("dataname").GetString();
            const String Type = context("datatype").GetString();
            if(!Type.Compare("viewscript"))
                mDataType = ZaySonInterface::DataType::ViewScript;
            else if(!Type.Compare("imagemap"))
                mDataType = ZaySonInterface::DataType::ImageMap;
            mFilePath = context("filepath").GetString();
            mUrl = context("url").GetString();
        }

    public:
        String mDataName;
        ZaySonInterface::DataType mDataType;
        String mFilePath;
        String mUrl;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayParamElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayParamElement : public ZayUIElement
    {
    public:
        ZayParamElement() : ZayUIElement(Type::Param) {}
        ~ZayParamElement() override {}

    public:
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
    // ZayRequestElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayRequestElement : public ZayUIElement
    {
    public:
        ZayRequestElement() : ZayUIElement(Type::Request)
        {
            mRequestType = ZaySonInterface::RequestType::Unknown;
            mGlueForFunction = nullptr;
        }
        ~ZayRequestElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            const String TextTest = context.GetString("");
            sint32 PosB, PosE;
            if(ZaySonInterface::IsFunctionCall(TextTest, &PosB, &PosE)) // 함수호출
            {
                mRequestType = ZaySonInterface::RequestType::Function;
                mRequestName = TextTest.Left(PosB - 1);
                mGlueForFunction = mRefRoot->FindGlue(mRequestName);
                if(!mGlueForFunction)
                    mRefRoot->AddDebugError(String::Format("글루함수를 찾을 수 없습니다(%s, Load)", (chars) mRequestName));
                Context Params(ST_Json, SO_NeedCopy, "[" + TextTest.Middle(PosB, PosE - PosB) + "]");
                for(sint32 i = 0, iend = Params.LengthOfIndexable(); i < iend; ++i)
                    mParamsForFunction.AtAdding() = Params[i].GetString();
            }
            else // 변수입력
            {
                chararray GetName;
                const auto& GetValue = context(0, &GetName);
                mRequestType = ZaySonInterface::RequestType::Variable;
                mRequestName = &GetName[0]; // 좌항
                mFormulaForVariable = GetValue.GetString(); // 우항
            }
        }

    public:
        void DeclareVariable()
        {
            BOSS_ASSERT("잘못된 시나리오입니다", mRequestType == ZaySonInterface::RequestType::Variable);
            mSolverForVariable.Link("chain", mRequestName, false);
            mSolverForVariable.Parse(ZayUIElement::GetResult(mFormulaForVariable).ToText(true));
            mSolverForVariable.Execute();
        }
        void Transaction()
        {
            if(mRequestType == ZaySonInterface::RequestType::Function)
            {
                if(mGlueForFunction)
                {
                    ZayExtend::Payload ParamCollector = mGlueForFunction->MakePayload();
                    for(sint32 i = 0, iend = mParamsForFunction.Count(); i < iend; ++i)
                        ParamCollector(ZayUIElement::GetResult(mParamsForFunction[i]));
                    // ParamCollector가 소멸되면서 Glue함수가 호출됨
                }
                else mRefRoot->AddDebugError(String::Format("글루함수를 실행하는데 실패하였습니다(%s, Transaction)", (chars) mRequestName));
            }
            else if(mRequestType == ZaySonInterface::RequestType::Variable)
            {
                if(auto FindedSolver = Solver::Find("chain", mRequestName))
                {
                    FindedSolver->Parse(ZayUIElement::GetResult(mFormulaForVariable).ToText(true));
                    FindedSolver->Execute();
                }
                else mRefRoot->AddDebugError(String::Format("변수를 업데이트하는데 실패하였습니다(%s, Transaction)", (chars) mRequestName));
            }
        }

    public:
        ZaySonInterface::RequestType mRequestType;
        String mRequestName;
        // 함수용
        const ZayExtend* mGlueForFunction;
        Strings mParamsForFunction;
        // 변수용
        String mFormulaForVariable;
        Solver mSolverForVariable;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayComponentElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayComponentElement : public ZayUIElement
    {
    public:
        ZayComponentElement() : ZayUIElement(Type::Component) {mCompID = -2;}
        ~ZayComponentElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            hook(context("compname"))
                mCompName = fish.GetString();

            hook(context("compid"))
                mCompID = fish.GetInt(-2);

            hook(context("compvalues"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mCompValues, fish[i]))
                    continue;
                Object<ZayParamElement> NewParam(ObjectAllocType::Now);
                NewParam->Load(root, fish[i]);
                mCompValues.AtAdding() = (id_share) NewParam;
            }

            hook(context("onclick"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mClickCodes, fish[i]))
                    continue;
                Object<ZayRequestElement> NewRequest(ObjectAllocType::Now);
                NewRequest->Load(root, fish[i]);
                mClickCodes.AtAdding() = (id_share) NewRequest;
            }

            hook(context("ui"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mChildren, fish[i]))
                    continue;
                Object<ZayComponentElement> NewRenderer(ObjectAllocType::Now);
                NewRenderer->Load(root, fish[i]);
                mChildren.AtAdding() = (id_share) NewRenderer;
            }
        }

    private:
        void Render(ZayPanel& panel, const String& uiname, sint32& compmax) const override
        {
            if(0 < compmax)
            {
                // Debug정보구성
                if(--compmax == 0)
                    mRefRoot->SetDebugCompName(mCompName, mComment);

                if(auto CurComponent = mRefRoot->FindComponent(mCompName))
                {
                    auto RenderDebug = [](const ZayComponentElement* self, ZayPanel& panel, bool content)->void
                    {
                        // 툴에 의한 포커스표현
                        if(self->mCompID == self->mRefRoot->debugFocusedCompID())
                        {
                            ZAY_RGBA(panel, 255, 0, 0, -128)
                            {
                                if(content)
                                    panel.fill();
                                else ZAY_INNER(panel, 5)
                                    panel.rect(10);
                            }
                        }
                    };

                    if(mCompValues.Count() == 0)
                    {
                        String UINameTemp;
                        chars ComponentName = nullptr;
                        if(0 < mUIName.Length()) ComponentName = UINameTemp = mRefRoot->UIName() + '.' + mUIName;
                        else if(0 < mClickCodes.Count()) ComponentName = uiname;
                        ZayExtend::Payload ParamCollector = CurComponent->MakePayload(ComponentName, this);

                        ZAY_EXTEND(ParamCollector >> panel)
                        {
                            RenderDebug(this, panel, CurComponent->HasContentComponent());
                            RenderChildren(panel, uiname, compmax);
                        }
                    }
                    else // CompValue항목이 존재할 경우
                    {
                        // 유효한 CompValue를 모두 실행
                        sint32s CollectedCompValues = ZayConditionElement::Collect(this, mCompValues, &panel);
                        for(sint32 i = 0, iend = CollectedCompValues.Count(); i < iend; ++i)
                        {
                            const String UINameSub = uiname + String::Format(".v%d", CollectedCompValues[i]);
                            String UINameTemp;
                            chars ComponentName = nullptr;
                            if(0 < mUIName.Length()) ComponentName = UINameTemp = mRefRoot->UIName() + '.' + mUIName;
                            else if(0 < mClickCodes.Count()) ComponentName = UINameSub;
                            ZayExtend::Payload ParamCollector = CurComponent->MakePayload(ComponentName, this);

                            // 컴포넌트호출을 위한 파라미터계산 및 수집
                            auto CurCompValue = (const ZayParamElement*) mCompValues[CollectedCompValues[i]].ConstPtr();
                            if(CurCompValue && 0 < CurCompValue->mParamFormulas.Count())
                            {
                                Solvers LocalSolvers; // 파라미터계산용 패널정보 사전입력
                                ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.w", String::FromFloat(panel.w()));
                                ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.h", String::FromFloat(panel.h()));
                                for(sint32 j = 0, jend = CurCompValue->mParamFormulas.Count(); j < jend; ++j)
                                    ParamCollector(ZayUIElement::GetResult(CurCompValue->mParamFormulas[j]));
                            }
                            ZAY_EXTEND(ParamCollector >> panel)
                            {
                                RenderDebug(this, panel, CurComponent->HasContentComponent());
                                RenderChildren(panel, UINameSub, compmax);
                            }
                        }
                    }
                }
                else mRefRoot->AddDebugError(String::Format("컴포넌트함수를 찾을 수 없습니다(%s, Render)", (chars) mCompName));
            }
            else mRefRoot->AddDebugError("Debug모드의 컴포넌트 수량제한으로 부분출력중(→키를 눌러 해소)");
        }
        void RenderChildren(ZayPanel& panel, const String& uiname, sint32& compmax) const
        {
            if(0 < mChildren.Count())
            {
                sint32s CollectedChildren = ZayConditionElement::Collect(this, mChildren, &panel);
                for(sint32 i = 0, iend = CollectedChildren.Count(); i < iend; ++i)
                {
                    auto CurChildren = (const ZayUIElement*) mChildren[CollectedChildren[i]].ConstPtr();
                    CurChildren->Render(panel, uiname + String::Format(".%d", i), compmax);
                }
            }
        }
        void OnTouch() override
        {
            if(0 < mClickCodes.Count())
            {
                sint32s CollectedClickCodes = ZayConditionElement::Collect(this, mClickCodes);
                for(sint32 i = 0, iend = CollectedClickCodes.Count(); i < iend; ++i)
                {
                    auto CurClickCode = (ZayRequestElement*) mClickCodes.At(CollectedClickCodes[i]).Ptr();
                    CurClickCode->Transaction();
                }
            }
        }

    public:
        String mCompName;
        sint32 mCompID;
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

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            hook(context("asset"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mAssets, fish[i]))
                    continue;
                Object<ZayAssetElement> NewResource(ObjectAllocType::Now);
                NewResource->Load(root, fish[i]);
                mAssets.AtAdding() = (id_share) NewResource;
            }

            hook(context("oncreate"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mCreateCodes, fish[i]))
                    continue;
                Object<ZayRequestElement> NewRequest(ObjectAllocType::Now);
                NewRequest->Load(root, fish[i]);
                NewRequest->DeclareVariable();
                mCreateCodes.AtAdding() = (id_share) NewRequest;
            }

            hook(context("ui"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mChildren, fish[i]))
                    continue;
                Object<ZayComponentElement> NewLayer(ObjectAllocType::Now);
                NewLayer->Load(root, fish[i]);
                mChildren.AtAdding() = (id_share) NewLayer;
            }
        }

    private:
        void Render(ZayPanel& panel, const String& uiname, sint32& compmax) const override
        {
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                if(mChildren[i]->mType == ZayUIElement::Type::Component)
                    mChildren[i]->Render(panel, uiname + String::Format(".%d", i), compmax);
        }

    public:
        ZayUIs mAssets;
        ZayUIs mCreateCodes;
        ZayUIs mChildren;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayExtend
    ////////////////////////////////////////////////////////////////////////////////
    ZayExtend::ZayExtend(ComponentType type, ComponentCB ccb, GlueCB gcb)
    {
        mComponentType = type;
        mComponentCB = ccb;
        mGlueCB = gcb;
    }

    ZayExtend::~ZayExtend()
    {
    }

    ZayExtend::Payload::Payload(const ZayExtend* owner, chars uiname, const ZayUIElement* uielement, const SolverValue* param)
    {
        mOwner = owner;
        mUIName = uiname;
        mUIElement = uielement;
        if(param) AddParam(*param);
    }

    ZayExtend::Payload::~Payload()
    {
        if(mOwner->HasGlue())
            mOwner->mGlueCB(*this);
    }

    ZayExtend::Payload& ZayExtend::Payload::operator()(const SolverValue& value)
    {
        AddParam(value);
        return *this;
    }

    ZayPanel::StackBinder ZayExtend::Payload::operator>>(ZayPanel& panel) const
    {
        if(mOwner->HasComponent())
            return mOwner->mComponentCB(panel, *this);
        return panel._push_pass();
    }

    chars ZayExtend::Payload::UIName() const
    {
        return mUIName;
    }

    ZayPanel::SubGestureCB ZayExtend::Payload::MakeGesture() const
    {
        auto UIElement = mUIElement;
        return ZAY_GESTURE_T(t, UIElement)
            {
                if(t == GT_InReleased)
                    ZayUIElement::TouchCore(UIElement);
            };
    }

    sint32 ZayExtend::Payload::ParamCount() const
    {
        return mParams.Count();
    }

    const SolverValue& ZayExtend::Payload::Param(sint32 i) const
    {
        return mParams[i];
    }

    bool ZayExtend::Payload::ParamToBool(sint32 i) const
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

    UIAlign ZayExtend::Payload::ParamToUIAlign(sint32 i) const
    {
        String Result = mParams[i].ToText();
        if(!String::CompareNoCase(Result, "UIA_", 4))
            Result = Result.Right(Result.Length() - 4);

        branch;
        jump(!Result.CompareNoCase("LeftTop")) return UIA_LeftTop;
        jump(!Result.CompareNoCase("CenterTop")) return UIA_CenterTop;
        jump(!Result.CompareNoCase("RightTop")) return UIA_RightTop;
        jump(!Result.CompareNoCase("LeftMiddle")) return UIA_LeftMiddle;
        jump(!Result.CompareNoCase("CenterMiddle")) return UIA_CenterMiddle;
        jump(!Result.CompareNoCase("RightMiddle")) return UIA_RightMiddle;
        jump(!Result.CompareNoCase("LeftBottom")) return UIA_LeftBottom;
        jump(!Result.CompareNoCase("CenterBottom")) return UIA_CenterBottom;
        jump(!Result.CompareNoCase("RightBottom")) return UIA_RightBottom;
        BOSS_ASSERT("알 수 없는 UIAlign입니다", false);
        return UIA_LeftTop;
    }

    UIStretchForm ZayExtend::Payload::ParamToUIStretchForm(sint32 i) const
    {
        String Result = mParams[i].ToText();
        if(!String::CompareNoCase(Result, "UISF_", 5))
            Result = Result.Right(Result.Length() - 5);

        branch;
        jump(!Result.CompareNoCase("Strong")) return UISF_Strong;
        jump(!Result.CompareNoCase("Inner")) return UISF_Inner;
        jump(!Result.CompareNoCase("Outer")) return UISF_Outer;
        jump(!Result.CompareNoCase("Width")) return UISF_Width;
        jump(!Result.CompareNoCase("Height")) return UISF_Height;
        BOSS_ASSERT("알 수 없는 UIStretchForm입니다", false);
        return UISF_Strong;
    }

    UIFontAlign ZayExtend::Payload::ParamToUIFontAlign(sint32 i) const
    {
        String Result = mParams[i].ToText();
        if(!String::CompareNoCase(Result, "UIFA_", 5))
            Result = Result.Right(Result.Length() - 5);

        branch;
        jump(!Result.CompareNoCase("LeftTop")) return UIFA_LeftTop;
        jump(!Result.CompareNoCase("CenterTop")) return UIFA_CenterTop;
        jump(!Result.CompareNoCase("RightTop")) return UIFA_RightTop;
        jump(!Result.CompareNoCase("JustifyTop")) return UIFA_JustifyTop;
        jump(!Result.CompareNoCase("LeftMiddle")) return UIFA_LeftMiddle;
        jump(!Result.CompareNoCase("CenterMiddle")) return UIFA_CenterMiddle;
        jump(!Result.CompareNoCase("RightMiddle")) return UIFA_RightMiddle;
        jump(!Result.CompareNoCase("JustifyMiddle")) return UIFA_JustifyMiddle;
        jump(!Result.CompareNoCase("LeftAscent")) return UIFA_LeftAscent;
        jump(!Result.CompareNoCase("CenterAscent")) return UIFA_CenterAscent;
        jump(!Result.CompareNoCase("RightAscent")) return UIFA_RightAscent;
        jump(!Result.CompareNoCase("JustifyAscent")) return UIFA_JustifyAscent;
        jump(!Result.CompareNoCase("LeftBottom")) return UIFA_LeftBottom;
        jump(!Result.CompareNoCase("CenterBottom")) return UIFA_CenterBottom;
        jump(!Result.CompareNoCase("RightBottom")) return UIFA_RightBottom;
        jump(!Result.CompareNoCase("JustifyBottom")) return UIFA_JustifyBottom;
        BOSS_ASSERT("알 수 없는 UIFontAlign입니다", false);
        return UIFA_LeftTop;
    }

    UIFontElide ZayExtend::Payload::ParamToUIFontElide(sint32 i) const
    {
        String Result = mParams[i].ToText();
        if(!String::CompareNoCase(Result, "UIFE_", 5))
            Result = Result.Right(Result.Length() - 5);

        branch;
        jump(!Result.CompareNoCase("None")) return UIFE_None;
        jump(!Result.CompareNoCase("Left")) return UIFE_Left;
        jump(!Result.CompareNoCase("Center")) return UIFE_Center;
        jump(!Result.CompareNoCase("Right")) return UIFE_Right;
        BOSS_ASSERT("알 수 없는 UIFontElide입니다", false);
        return UIFE_None;
    }

    void ZayExtend::Payload::AddParam(const SolverValue& value)
    {
        mParams.AtAdding() = value;
    }

    const ZayExtend::Payload ZayExtend::operator()() const
    {
        return Payload(this);
    }

    ZayExtend::Payload ZayExtend::operator()(const SolverValue& value) const
    {
        return Payload(this, nullptr, nullptr, &value);
    }

    bool ZayExtend::HasComponent() const
    {
        return (mComponentCB != nullptr);
    }

    bool ZayExtend::HasContentComponent() const
    {
        return (mComponentType == ComponentType::Content || mComponentType == ComponentType::ContentWithParameter);
    }

    bool ZayExtend::HasGlue() const
    {
        return (mGlueCB != nullptr);
    }

    void ZayExtend::ResetForComponent(ComponentType type, ComponentCB cb)
    {
        mComponentType = type;
        mComponentCB = cb;
    }

    void ZayExtend::ResetForGlue(GlueCB cb)
    {
        mGlueCB = cb;
    }

    ZayExtend::Payload ZayExtend::MakePayload(chars uiname, const ZayUIElement* uielement) const
    {
        return Payload(this, uiname, uielement);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ZaySonInterface
    ////////////////////////////////////////////////////////////////////////////////
    bool ZaySonInterface::IsFunctionCall(chars text, sint32* prmbegin, sint32* prmend)
    {
        const String FunctionTest = text;
        if(sint32 PosB = FunctionTest.Find(0, "(") + 1)
        {
            sint32 PosE = FunctionTest.Find(PosB, ")");
            if(PosE != -1)
            {
                if(prmbegin) *prmbegin = PosB;
                if(prmend) *prmend = PosE;
                return true;
            }
        }
        return false;
    }

    ZaySonInterface::ConditionType ZaySonInterface::ToCondition(chars text)
    {
        branch;
        jump(!String::Compare(text, "iffocused(", 10)) // 문자열길이 순서상 먼저
            return ConditionType::IfFocused;
        jump(!String::Compare(text, "ifhovered(", 10))
            return ConditionType::IfHovered;
        jump(!String::Compare(text, "ifpressed(", 10))
            return ConditionType::IfPressed;
        jump(!String::Compare(text, "if(", 3))
            return ConditionType::If;
        jump(!String::Compare(text, "elif(", 5))
            return ConditionType::Elif;
        jump(!String::Compare(text, "else"))
            return ConditionType::Else;
        jump(!String::Compare(text, "endif"))
            return ConditionType::Endif;
        return ConditionType::Unknown;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ZaySon
    ////////////////////////////////////////////////////////////////////////////////
    ZaySon::ZaySon()
    {
        mUIElement = nullptr;
        // 디버그정보
        mDebugFocusedCompID = -1;
        mDebugErrorFocus = 0;
        for(sint32 i = 0; i < mDebugErrorCountMax; ++i)
            mDebugErrorShowCount[i] = 0;
    }

    ZaySon::~ZaySon()
    {
        delete mUIElement;
    }

    ZaySon::ZaySon(ZaySon&& rhs)
    {
        operator=(ToReference(rhs));
    }

    ZaySon& ZaySon::operator=(ZaySon&& rhs)
    {
        mUIName = ToReference(rhs.mUIName);
        delete mUIElement;
        mUIElement = rhs.mUIElement;
        rhs.mUIElement = nullptr;
        mExtendMap = ToReference(rhs.mExtendMap);
        mDebugCompName = ToReference(rhs.mDebugCompName);
        mDebugFocusedCompID = ToReference(rhs.mDebugFocusedCompID);
        mDebugErrorFocus = ToReference(rhs.mDebugErrorFocus);
        for(sint32 i = 0; i < mDebugErrorCountMax; ++i)
        {
            mDebugErrorName[i] = ToReference(rhs.mDebugErrorName[i]);
            mDebugErrorShowCount[i] = ToReference(rhs.mDebugErrorShowCount[i]);
        }
        return *this;
    }

    void ZaySon::SetUIName(chars uiname)
    {
        mUIName = uiname;
    }

    const String ZaySon::UIName() const
    {
        return mUIName;
    }

    void ZaySon::Load(const Context& context)
    {
        delete mUIElement;
        auto NewView = new ZayViewElement();
        NewView->Load(*this, context);
        mUIElement = NewView;
    }

    ZaySonInterface& ZaySon::AddComponent(ZayExtend::ComponentType type, chars name, ZayExtend::ComponentCB cb, chars paramcomment)
    {
        auto& NewFunction = mExtendMap(name);
        NewFunction.ResetForComponent(type, cb);
        return *this;
    }

    ZaySonInterface& ZaySon::AddGlue(chars name, ZayExtend::GlueCB cb)
    {
        auto& NewFunction = mExtendMap(name);
        NewFunction.ResetForGlue(cb);
        return *this;
    }

    const ZayExtend* ZaySon::FindComponent(chars name) const
    {
        if(auto FindedFunc = mExtendMap.Access(name))
        {
            const ZayExtend* Result = (FindedFunc->HasComponent())? FindedFunc : nullptr;
            BOSS_ASSERT(String::Format("\"%s\"는 Component함수가 아닙니다", name), Result);
            return Result;
        }
        BOSS_ASSERT(String::Format("\"%s\"으로 등록된 ZayExtend를 찾을 수 없습니다", name), false);
        return nullptr;
    }

    const ZayExtend* ZaySon::FindGlue(chars name) const
    {
        if(auto FindedFunc = mExtendMap.Access(name))
        {
            const ZayExtend* Result = (FindedFunc->HasGlue())? FindedFunc : nullptr;
            BOSS_ASSERT(String::Format("\"%s\"는 Glue함수가 아닙니다", name), Result);
            return Result;
        }
        BOSS_ASSERT(String::Format("\"%s\"으로 등록된 ZayExtend를 찾을 수 없습니다", name), false);
        return nullptr;
    }

    sint32 ZaySon::Render(ZayPanel& panel, sint32 compmax)
    {
        if(mUIElement)
        {
            mDebugCompName = "(null)";
            const sint32 OldCompMax = compmax;
            mUIElement->Render(panel, UIName(), compmax);
            return OldCompMax - compmax;
        }
        return 0;
    }

    void ZaySon::SetDebugCompName(chars name, chars comment) const
    {
        mDebugCompName = name;
        if(comment[0] != '\0')
            mDebugCompName = mDebugCompName + '(' + comment + ')';
    }

    void ZaySon::SetDebugFocusedCompID(sint32 id) const
    {
        mDebugFocusedCompID = id;
    }

    void ZaySon::AddDebugError(chars name) const
    {
        // 같은 메시지는 새로 추가하지 않고 강조만 함
        for(sint32 i = 0; i < mDebugErrorCountMax; ++i)
        {
            if(0 < mDebugErrorShowCount[i] && !mDebugErrorName[i].Compare(name))
            {
                mDebugErrorShowCount[i] = mDebugErrorShowMax;
                return;
            }
        }
        mDebugErrorFocus = (mDebugErrorFocus + mDebugErrorCountMax - 1) % mDebugErrorCountMax;
        mDebugErrorName[mDebugErrorFocus] = name;
        mDebugErrorShowCount[mDebugErrorFocus] = mDebugErrorShowMax;
    }

    chars ZaySon::debugCompName() const
    {
        return mDebugCompName;
    }

    sint32 ZaySon::debugFocusedCompID() const
    {
        return mDebugFocusedCompID;
    }

    sint32 ZaySon::debugErrorCountMax() const
    {
        return mDebugErrorCountMax;
    }

    chars ZaySon::debugErrorName(sint32 i) const
    {
        return mDebugErrorName[(mDebugErrorFocus + i) % mDebugErrorCountMax];
    }

    float ZaySon::debugErrorShowRate(sint32 i, bool countdown) const
    {
        sint32& ShowCount = mDebugErrorShowCount[(mDebugErrorFocus + i) % mDebugErrorCountMax];
        const float Result = ShowCount / (float) mDebugErrorShowMax;
        if(countdown && 0 < ShowCount)
            ShowCount--;
        return Result;
    }
}
