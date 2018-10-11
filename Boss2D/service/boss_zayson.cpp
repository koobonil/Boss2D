#include <boss.hpp>
#include "boss_zayson.hpp"

namespace BOSS
{
    ////////////////////////////////////////////////////////////////////////////////
    // ZayUIElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayUIElement
    {
    public:
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
        static double GetResult(chars formula)
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
            bool HasCondition = false;
            for(sint32 i = 0, iend = uis.Count(); i < iend; ++i)
            {
                if(uis[i].ConstValue().mType == ZayUIElement::Type::Condition)
                {
                    HasCondition = true;
                    auto CurCondition = (const ZayConditionElement*) uis[i].ConstPtr();
                    bool IsTrue = true;
                    if(CurCondition->mConditionType == ZayConditionElement::ConditionType::If ||
                        CurCondition->mConditionType == ZayConditionElement::ConditionType::Elif)
                        IsTrue = (ZayUIElement::GetResult(CurCondition->mConditionSolver) != 0);
                    if(IsTrue)
                    {
                        while(i + 1 < iend && uis[i + 1].ConstValue().mType != ZayUIElement::Type::Condition)
                            Collector.AtAdding() = ++i;
                        // 다음 조건문으로 포커싱이동
                        while(i + 1 < iend)
                        {
                            if(uis[++i].ConstValue().mType == ZayUIElement::Type::Condition)
                            {
                                CurCondition = (const ZayConditionElement*) uis[i].ConstPtr();
                                if(CurCondition->mConditionType == ZayConditionElement::ConditionType::If)
                                {
                                    i--;
                                    break;
                                }
                                if(CurCondition->mConditionType == ZayConditionElement::ConditionType::Endif)
                                    break;
                            }
                        }
                    }
                }
            }

            // 조건문이 없으면 모두 유효한 CompValue로 인정
            if(!HasCondition)
            for(sint32 i = 0, iend = uis.Count(); i < iend; ++i)
                Collector.AtAdding() = i;
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
            mSolver.Parse(String::FromFloat(ZayUIElement::GetResult(mValueFormula)));
            mSolver.Execute();
        }
        void Transform()
        {
            if(auto FindedSolver = Solver::Find("chain", mValueName))
            {
                FindedSolver->Parse(String::FromFloat(ZayUIElement::GetResult(mValueFormula)));
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
                        for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                            if(mChildren[i]->mType == ZayUIElement::Type::Component)
                                mChildren[i]->Render(panel, zayson, uiname + String::FromInteger(-1 - i), compmax);
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
                            for(sint32 j = 0, jend = mChildren.Count(); j < jend; ++j)
                                if(mChildren[j]->mType == ZayUIElement::Type::Component)
                                    mChildren[j]->Render(panel, zayson, UINameSub + String::FromInteger(-1 - j), compmax);
                        }
                    }
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

    ZayComponent::Payload::Payload(CallBack cb, chars uiname, const ZayUIElement* uielement, id_cloned_share param)
    {
        mCB = cb;
        mUIName = uiname;
        mUIElement = uielement;
        if(param) AddParam(param);
    }

    ZayComponent::Payload::~Payload()
    {
    }

    ZayComponent::Payload& ZayComponent::Payload::operator()(sint32 value)
    {AddParam(Remote::IntParam((sint64) value)); return *this;}

    ZayComponent::Payload& ZayComponent::Payload::operator()(sint64 value)
    {AddParam(Remote::IntParam(value)); return *this;}

    ZayComponent::Payload& ZayComponent::Payload::operator()(float value)
    {AddParam(Remote::DecParam((double) value)); return *this;}

    ZayComponent::Payload& ZayComponent::Payload::operator()(double value)
    {AddParam(Remote::DecParam(value)); return *this;}

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

    id_cloned_share ZayComponent::Payload::TakeParam(sint32 i) const
    {return mParams[i].Drain();}

    void ZayComponent::Payload::AddParam(id_cloned_share param)
    {mParams.AtAdding().Store(param);}

    const ZayComponent::Payload ZayComponent::operator()() const
    {return ZayComponent::Payload(mCB);}

    ZayComponent::Payload ZayComponent::operator()(sint32 value) const
    {return ZayComponent::Payload(mCB, nullptr, nullptr, Remote::IntParam((sint64) value));}

    ZayComponent::Payload ZayComponent::operator()(sint64 value) const
    {return ZayComponent::Payload(mCB, nullptr, nullptr, Remote::IntParam(value));}

    ZayComponent::Payload ZayComponent::operator()(float value) const
    {return ZayComponent::Payload(mCB, nullptr, nullptr, Remote::DecParam((double) value));}

    ZayComponent::Payload ZayComponent::operator()(double value) const
    {return ZayComponent::Payload(mCB, nullptr, nullptr, Remote::DecParam(value));}

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
