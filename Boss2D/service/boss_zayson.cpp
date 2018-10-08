#include <boss.hpp>
#include "boss_zayson.hpp"

namespace BOSS
{
    ZayExtend::ZayExtend(CallBack cb)
    {
        mCB = cb;
    }

    ZayExtend::~ZayExtend()
    {
    }

    void ZayExtend::Reset(CallBack cb)
    {
        mCB = cb;
    }

    ZayExtend::ZayParam::ZayParam(CallBack cb)
    {
        mCB = cb;
    }

    ZayExtend::ZayParam::~ZayParam()
    {
    }

    ZayPanel::StackBinder ZayExtend::ZayParam::operator>>(ZayPanel& panel) const
    {
        return mCB(panel);
    }

    const ZayExtend::ZayParam ZayExtend::operator()() const
    {
        //////////////////////// 여기서부터!!
    }

    const ZayExtend::ZayParam ZayExtend::operator()(sint32 value) const
    {
        //////////////////////// 여기서부터!!
    }

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
        ZayUIElement(Type type = Type::Unknown) : mType(type) {}
        virtual ~ZayUIElement() {}

    public:
        virtual void Load(const Context& context)
        {
            hook(context("comment"))
                mComment = fish.GetString();
        }
        virtual void Render(ZayPanel& panel, const ZaySon& zayson) const 
        {
        }

    public:
        Type mType;
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
        void Load(const Context& context) override
        {
            const String ConditionText = context.GetString();
            branch;
            jump(!ConditionText.Compare("if"))
                mConditionType = ConditionType::If;
            jump(!ConditionText.Compare("Elif"))
                mConditionType = ConditionType::Elif;
            jump(!ConditionText.Compare("Else"))
                mConditionType = ConditionType::Else;

            if(sint32 PosB = ConditionText.Find(0, "(") + 1)
            {
                sint32 PosE = ConditionText.Find(PosB + 1, ")");
                if(PosE != -1)
                    mConditionSolver = String(((chars) ConditionText) + PosB, PosE - PosB);
            }
        }

    public:
        static bool Test(ZayUIs& dest, const Context& src)
        {
            if(src.GetString(nullptr))
            {
                Object<ZayConditionElement> NewCondition(ObjectAllocType::Now);
                ((ZayUIElement*) NewCondition.Ptr())->Load(src);
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
        void Load(const Context& context) override
        {
            ZayUIElement::Load(context);

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
        void Load(const Context& context) override
        {
            chararray GetName;
            const auto& GetValue = context(0, &GetName);
            mValueName = &GetName[0];
            mValueSolver = GetValue.GetString();
        }

    public:
        String mValueName;
        String mValueSolver;
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
        void Load(const Context& context) override
        {
            for(sint32 i = 0, iend = context.LengthOfIndexable(); i < iend; ++i)
                mParamSolvers.AtAdding() = context[i].GetString();
        }

    public:
        Strings mParamSolvers;
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
        void Load(const Context& context) override
        {
            ZayUIElement::Load(context);

            hook(context("funcname"))
                mFuncName = fish.GetString();

            hook(context("funcvalues"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(mFuncValues, fish[i]))
                    continue;
                Object<ZayParamElement> NewParam(ObjectAllocType::Now);
                ((ZayUIElement*) NewParam.Ptr())->Load(fish[i]);
                mFuncValues.AtAdding() = (id_share) NewParam;
            }

            hook(context("onclick"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(mClickCodes, fish[i]))
                    continue;
                Object<ZayVariableElement> NewVariable(ObjectAllocType::Now);
                ((ZayUIElement*) NewVariable.Ptr())->Load(fish[i]);
                mClickCodes.AtAdding() = (id_share) NewVariable;
            }

            hook(context("ui"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(mChildren, fish[i]))
                    continue;
                Object<ZayRendererElement> NewRenderer(ObjectAllocType::Now);
                ((ZayUIElement*) NewRenderer.Ptr())->Load(fish[i]);
                mChildren.AtAdding() = (id_share) NewRenderer;
            }
        }
        void Render(ZayPanel& panel, const ZaySon& zayson) const override
        {
            if(auto CurFunction = zayson.FindFunction(mFuncName))
            ZAY_EXTEND((*CurFunction)(10)(20)(50)(50) >> panel)
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
            {
                if(mChildren[i]->mType == ZayUIElement::Type::Renderer)
                {
                    mChildren[i]->Render(panel, zayson);
                }
            }
        }

    public:
        String mFuncName;
        ZayUIs mFuncValues;
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
        void Load(const Context& context) override
        {
            ZayUIElement::Load(context);

            hook(context("asset"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(mAssets, fish[i]))
                    continue;
                Object<ZayResourceElement> NewResource(ObjectAllocType::Now);
                ((ZayUIElement*) NewResource.Ptr())->Load(fish[i]);
                mAssets.AtAdding() = (id_share) NewResource;
            }

            hook(context("oncreate"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(mCreateCodes, fish[i]))
                    continue;
                Object<ZayVariableElement> NewVariable(ObjectAllocType::Now);
                ((ZayUIElement*) NewVariable.Ptr())->Load(fish[i]);
                mCreateCodes.AtAdding() = (id_share) NewVariable;
            }

            hook(context("ui"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(mChildren, fish[i]))
                    continue;
                Object<ZayRendererElement> NewLayer(ObjectAllocType::Now);
                ((ZayUIElement*) NewLayer.Ptr())->Load(fish[i]);
                mChildren.AtAdding() = (id_share) NewLayer;
            }
        }
        void Render(ZayPanel& panel, const ZaySon& zayson) const override
        {
            for(sint32 i = 0, iend = mChildren.Count(); i < iend; ++i)
            {
                if(mChildren[i]->mType == ZayUIElement::Type::Renderer)
                {
                    mChildren[i]->Render(panel, zayson);
                }
            }
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
        mView->Load(context);
    }

    void ZaySon::AddFunction(chars name, ZayExtend::CallBack cb)
    {
        auto& NewFunction = mFunctionMap(name);
        NewFunction.Reset(cb);
    }

    const ZayExtend* ZaySon::FindFunction(chars name) const
    {
        if(auto FindedFunc = mFunctionMap.Access(name))
            return FindedFunc;
        return nullptr;
    }

    void ZaySon::Render(ZayPanel& panel)
    {
        if(mView)
            mView->Render(panel, *this);
    }
}
