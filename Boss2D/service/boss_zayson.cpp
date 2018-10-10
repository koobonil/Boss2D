#include <boss.hpp>
#include "boss_zayson.hpp"

namespace BOSS
{
    ZayExtend::ZayExtend(Component com)
    {
        mCom = com;
    }

    ZayExtend::~ZayExtend()
    {
    }

    void ZayExtend::Reset(Component com)
    {
        mCom = com;
    }

    ZayExtend::Params::Params(Component com, id_cloned_share param)
    {
        mCom = com;
        if(param) AddParam(param);
    }

    ZayExtend::Params::~Params()
    {
    }

    ZayExtend::Params& ZayExtend::Params::operator()(sint32 value)
    {AddParam(Remote::IntParam((sint64) value)); return *this;}

    ZayExtend::Params& ZayExtend::Params::operator()(sint64 value)
    {AddParam(Remote::IntParam(value)); return *this;}

    ZayExtend::Params& ZayExtend::Params::operator()(float value)
    {AddParam(Remote::DecParam((double) value)); return *this;}

    ZayExtend::Params& ZayExtend::Params::operator()(double value)
    {AddParam(Remote::DecParam(value)); return *this;}

    ZayPanel::StackBinder ZayExtend::Params::operator>>(ZayPanel& panel) const
    {return mCom(panel, *this);}

    sint32 ZayExtend::Params::Count() const
    {return mParams.Count();}

    id_cloned_share ZayExtend::Params::Take(sint32 i) const
    {return mParams[i].Drain();}

    void ZayExtend::Params::AddParam(id_cloned_share param)
    {mParams.AtAdding().Store(param);}

    ZayExtend::Params ZayExtend::operator()() const
    {return ZayExtend::Params(mCom);}

    ZayExtend::Params ZayExtend::operator()(sint32 value) const
    {return ZayExtend::Params(mCom, Remote::IntParam((sint64) value));}

    ZayExtend::Params ZayExtend::operator()(sint64 value) const
    {return ZayExtend::Params(mCom, Remote::IntParam(value));}

    ZayExtend::Params ZayExtend::operator()(float value) const
    {return ZayExtend::Params(mCom, Remote::DecParam((double) value));}

    ZayExtend::Params ZayExtend::operator()(double value) const
    {return ZayExtend::Params(mCom, Remote::DecParam(value));}

    ////////////////////////////////////////////////////////////////////////////////
    // ZayUIElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayUIElement
    {
    public:
        BOSS_DECLARE_STANDARD_CLASS(ZayUIElement)

    public:
        enum class Type {Unknown, Condition, Data, Value, Param, Renderer, View};

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
        virtual void Render(ZayPanel& panel, const ZaySon& zayson, sint32& compmax) const 
        {
        }
        virtual sint32 GetRenderCount() const
        {
            return 0;
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
        enum class ConditionType {Unknown, If, Elif, Else};

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
            jump(!ConditionText.Compare("if"))
                mConditionType = ConditionType::If;
            jump(!ConditionText.Compare("elif"))
                mConditionType = ConditionType::Elif;
            jump(!ConditionText.Compare("else"))
                mConditionType = ConditionType::Else;

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

    public:
        ConditionType mConditionType;
        String mConditionSolver;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZayResourceElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayResourceElement : public ZayUIElement
    {
    public:
        enum class DataType {Unknown, ViewScript, ImageMap};

    public:
        ZayResourceElement() : ZayUIElement(Type::Data)
        {mDataType = DataType::Unknown;}
        ~ZayResourceElement() override {}

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
            mResource = context("resource").GetString();
            mUrl = context("url").GetString();
        }

    public:
        String mDataName;
        DataType mDataType;
        String mResource;
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
    // ZayRendererElement
    ////////////////////////////////////////////////////////////////////////////////
    class ZayRendererElement : public ZayUIElement
    {
    public:
        ZayRendererElement() : ZayUIElement(Type::Renderer) {}
        ~ZayRendererElement() override {}

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
                Object<ZayRendererElement> NewRenderer(ObjectAllocType::Now);
                ((ZayUIElement*) NewRenderer.Ptr())->Load(root, fish[i]);
                mChildren.AtAdding() = (id_share) NewRenderer;
            }
        }
        void Render(ZayPanel& panel, const ZaySon& zayson, sint32& compmax) const override
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
                    // 마지막 밸류를 수집
                    const ZayParamElement* Param = nullptr;
                    for(sint32 i = 0, iend = mCompValues.Count(); i < iend; ++i)
                        if(mCompValues[i].ConstValue().mType == ZayUIElement::Type::Param)
                            Param = (const ZayParamElement*) mCompValues[i].ConstPtr();

                    // 패널정보
                    Solvers LocalSolvers;
                    ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.width", String::FromFloat(panel.w()));
                    ZayUIElement::SetSolver(LocalSolvers.AtAdding(), "p.height", String::FromFloat(panel.h()));

                    // 컴포넌트 호출을 위한 파라미터수집
                    ZayExtend::Params& ParamCollector = (*CurComponent)();
                    for(sint32 i = 0, iend = (Param)? Param->mParamFormulas.Count() : 0; i < iend; ++i)
                        ParamCollector(ZayUIElement::GetResult(Param->mParamFormulas[i]));

                    ZAY_EXTEND(ParamCollector >> panel)
                    for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                        if(mChildren[i]->mType == ZayUIElement::Type::Renderer)
                            mChildren[i]->Render(panel, zayson, compmax);

                    // 패널정보 PULL
                    /////////////////////////////////////////
                    // 여기서부터~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!
                    /////////////////////////////////////////
                }
            }
        }
        sint32 GetRenderCount() const override
        {
            sint32 Result = 1;
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                Result += mChildren[i]->GetRenderCount();
            return Result;
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
                Object<ZayResourceElement> NewResource(ObjectAllocType::Now);
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
                Object<ZayRendererElement> NewLayer(ObjectAllocType::Now);
                ((ZayUIElement*) NewLayer.Ptr())->Load(root, fish[i]);
                mChildren.AtAdding() = (id_share) NewLayer;
            }
        }
        void Render(ZayPanel& panel, const ZaySon& zayson, sint32& compmax) const override
        {
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                if(mChildren[i]->mType == ZayUIElement::Type::Renderer)
                    mChildren[i]->Render(panel, zayson, compmax);
        }
        sint32 GetRenderCount() const override
        {
            sint32 Result = 0;
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
                Result += mChildren[i]->GetRenderCount();
            return Result;
        }

    public:
        ZayUIs mAssets;
        ZayUIs mCreateCodes;
        ZayUIs mChildren;
    };

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

    void ZaySon::AddComponent(chars name, ZayExtend::Component com)
    {
        auto& NewFunction = mComponentMap(name);
        NewFunction.Reset(com);
    }

    const ZayExtend* ZaySon::FindComponent(chars name) const
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
            mView->Render(panel, *this, compmax);
            return mView->GetRenderCount();
        }
        return 0;
    }
}
