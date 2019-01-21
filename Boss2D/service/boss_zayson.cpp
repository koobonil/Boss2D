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
        enum class ConditionType {Unknown, If, IfFocused, IfHovered, IfPressed, Elif, Else, Endif};

    public:
        ZayConditionElement() : ZayUIElement(Type::Condition)
        {mConditionType = ConditionType::Unknown;}
        ~ZayConditionElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            const String ConditionText = context.GetString();
            branch;
            jump(!String::Compare("iffocused", ConditionText, 9)) // 문자열길이 순서상 먼저
                mConditionType = ConditionType::IfFocused;
            jump(!String::Compare("ifhovered", ConditionText, 9))
                mConditionType = ConditionType::IfHovered;
            jump(!String::Compare("ifpressed", ConditionText, 9))
                mConditionType = ConditionType::IfPressed;
            jump(!String::Compare("if", ConditionText, 2))
                mConditionType = ConditionType::If;
            jump(!String::Compare("elif", ConditionText, 4))
                mConditionType = ConditionType::Elif;
            jump(!String::Compare("else", ConditionText, 4))
                mConditionType = ConditionType::Else;
            jump(!String::Compare("endif", ConditionText, 5))
                mConditionType = ConditionType::Endif;
            else mRefRoot->AddDebugError(String::Format("알 수 없는 조건문입니다(%s, Load)", (chars) ConditionText));

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
                NewCondition->Load(root, src);
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
                    if(CurCondition->mConditionType == ZayConditionElement::ConditionType::If ||
                        CurCondition->mConditionType == ZayConditionElement::ConditionType::Elif)
                        IsTrue = (ZayUIElement::GetResult(CurCondition->mConditionSolver).ToInteger() != 0);
                    // 포커스확인
                    else if(CurCondition->mConditionType == ZayConditionElement::ConditionType::IfFocused)
                    {
                        if(panel)
                        {
                            const String UIName = ZayUIElement::GetResult(CurCondition->mConditionSolver).ToText();
                            IsTrue = !!(panel->state(self->mRefRoot->UIName() + '.' + UIName) & PS_Focused);
                        }
                    }
                    // 호버확인
                    else if(CurCondition->mConditionType == ZayConditionElement::ConditionType::IfHovered)
                    {
                        if(panel)
                        {
                            const String UIName = ZayUIElement::GetResult(CurCondition->mConditionSolver).ToText();
                            IsTrue = !!(panel->state(self->mRefRoot->UIName() + '.' + UIName) & PS_Hovered);
                        }
                    }
                    // 프레스확인
                    else if(CurCondition->mConditionType == ZayConditionElement::ConditionType::IfPressed)
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
                                if(CurCondition->mConditionType == ZayConditionElement::ConditionType::Endif) // endif는 조건그룹을 빠져나오게 하고
                                    break;
                                else if(CurCondition->mConditionType == ZayConditionElement::ConditionType::If || // 새로운 조건그룹을 만나면 수락
                                    CurCondition->mConditionType == ZayConditionElement::ConditionType::IfFocused ||
                                    CurCondition->mConditionType == ZayConditionElement::ConditionType::IfHovered ||
                                    CurCondition->mConditionType == ZayConditionElement::ConditionType::IfPressed)
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
                            if(CurCondition->mConditionType == ZayConditionElement::ConditionType::Endif) // endif는 조건그룹을 빠져나오게 하고
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

    public:
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
        enum class RequestType {Unknown, Function, Variable};

    public:
        ZayRequestElement() : ZayUIElement(Type::Request)
        {
            mRequestType = RequestType::Unknown;
            mGlueForFunction = nullptr;
        }
        ~ZayRequestElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            chararray GetName;
            const auto& GetValue = context(0, &GetName);
            mRequestName = &GetName[0];

            // 함수
            if(!String::Compare("call", mRequestName, 4))
            if(sint32 PosB = mRequestName.Find(0, "(") + 1)
            {
                sint32 PosE = mRequestName.Find(PosB + 1, ")");
                if(PosE != -1)
                {
                    const String FunctionName = String(((chars) mRequestName) + PosB, PosE - PosB);
                    mRequestType = RequestType::Function;
                    mGlueForFunction = mRefRoot->FindGlue(FunctionName);
                    mParamForFunction.Load(root, GetValue);
                    if(mGlueForFunction == nullptr)
                        mRefRoot->AddDebugError(String::Format("글루함수를 찾을 수 없습니다(%s, Load)", (chars) FunctionName));
                }
            }

            // 변수
            if(mRequestType == RequestType::Unknown)
            {
                mRequestType = RequestType::Variable;
                mFormulaForVariable = GetValue.GetString();
            }
        }

    public:
        void DeclareVariable()
        {
            BOSS_ASSERT("잘못된 시나리오입니다", mRequestType == RequestType::Variable);
            mSolverForVariable.Link("chain", mRequestName, false);
            mSolverForVariable.Parse(ZayUIElement::GetResult(mFormulaForVariable).ToText());
            mSolverForVariable.Execute();
        }
        void Transaction()
        {
            if(mRequestType == RequestType::Function)
            {
                if(mGlueForFunction)
                {
                    ZayExtend::Payload ParamCollector = mGlueForFunction->MakePayload();
                    if(0 < mParamForFunction.mParamFormulas.Count())
                        for(sint32 i = 0, iend = mParamForFunction.mParamFormulas.Count(); i < iend; ++i)
                            ParamCollector(ZayUIElement::GetResult(mParamForFunction.mParamFormulas[i]));
                    // ParamCollector가 소멸되면서 Glue함수가 호출됨
                }
                else mRefRoot->AddDebugError(String::Format("글루함수를 실행하는데 실패하였습니다(%s, Transaction)", (chars) mRequestName));
            }
            else if(mRequestType == RequestType::Variable)
            {
                if(auto FindedSolver = Solver::Find("chain", mRequestName))
                {
                    FindedSolver->Parse(ZayUIElement::GetResult(mFormulaForVariable).ToText());
                    FindedSolver->Execute();
                }
                else mRefRoot->AddDebugError(String::Format("변수를 업데이트하는데 실패하였습니다(%s, Transaction)", (chars) mRequestName));
            }
        }

    public:
        RequestType mRequestType;
        String mRequestName;
        // 함수용
        const ZayExtend* mGlueForFunction;
        ZayParamElement mParamForFunction;
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
        ZayComponentElement() : ZayUIElement(Type::Component) {}
        ~ZayComponentElement() override {}

    public:
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
                    if(mCompValues.Count() == 0)
                    {
                        String UINameTemp;
                        chars ComponentName = nullptr;
                        if(0 < mUIName.Length()) ComponentName = UINameTemp = mRefRoot->UIName() + '.' + mUIName;
                        else if(0 < mClickCodes.Count()) ComponentName = uiname;
                        ZayExtend::Payload ParamCollector = CurComponent->MakePayload(ComponentName, this);

                        ZAY_EXTEND(ParamCollector >> panel)
                            RenderChildren(panel, uiname, compmax);
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
                                ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.width", String::FromFloat(panel.w()));
                                ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.height", String::FromFloat(panel.h()));
                                for(sint32 j = 0, jend = CurCompValue->mParamFormulas.Count(); j < jend; ++j)
                                    ParamCollector(ZayUIElement::GetResult(CurCompValue->mParamFormulas[j]));
                            }
                            ZAY_EXTEND(ParamCollector >> panel)
                                RenderChildren(panel, UINameSub, compmax);
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
    ZayExtend::ZayExtend(ComponentCB ccb, GlueCB gcb)
    {
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

    UIStretchForm ZayExtend::Payload::ParamToUIStretchForm(sint32 i) const
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

    UIFontAlign ZayExtend::Payload::ParamToUIFontAlign(sint32 i) const
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

    UIFontElide ZayExtend::Payload::ParamToUIFontElide(sint32 i) const
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
        return (bool) mComponentCB;
    }

    bool ZayExtend::HasGlue() const
    {
        return (bool) mGlueCB;
    }

    void ZayExtend::ResetForComponent(ComponentCB cb)
    {
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
    // ZaySon
    ////////////////////////////////////////////////////////////////////////////////
    ZaySon::ZaySon()
    {
        mUIElement = nullptr;
        // 디버그정보
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

    void ZaySon::Load(const Context& context)
    {
        delete mUIElement;
        auto NewView = new ZayViewElement();
        NewView->Load(*this, context);
        mUIElement = NewView;
    }

    void ZaySon::AddComponent(chars name, ZayExtend::ComponentCB cb)
    {
        auto& NewFunction = mExtendMap(name);
        NewFunction.ResetForComponent(cb);
    }

    void ZaySon::AddGlue(chars name, ZayExtend::GlueCB cb)
    {
        auto& NewFunction = mExtendMap(name);
        NewFunction.ResetForGlue(cb);
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
