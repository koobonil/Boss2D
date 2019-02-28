﻿#include <boss.hpp>
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
        static SolverValue GetResult(chars viewname, chars formula)
        {
            Solver NewSolver;
            NewSolver.Link(viewname);
            NewSolver.Parse(formula);
            NewSolver.Execute();
            return NewSolver.result();
        }

    protected:
        virtual void Load(const ZaySon& root, const Context& context)
        {
            mRefRoot = &root;
            hook(context("uiname"))
                mUINameSolver.Link(root.ViewName()).Parse(fish.GetString());
            hook(context("uiloop"))
                mUILoopSolver.Link(root.ViewName()).Parse(fish.GetString());
            hook(context("comment"))
                mComment = fish.GetString();
        }

    public:
        class DebugLog
        {
        public:
            Rect mRect;
            bool mFill;
            String mUIName;
        };
        typedef Array<DebugLog> DebugLogs;

    public:
        virtual void Render(ZayPanel& panel, const String& defaultname, sint32& compmax, DebugLogs& logs) const
        {
        }
        virtual void OnTouch(chars uiname)
        {
        }

    public:
        static void TouchCore(chars uiname, const ZayUIElement* payload)
        {
            ((ZayUIElement*) payload)->OnTouch(uiname);
        }

    public:
        Type mType;
        const ZaySon* mRefRoot;
        Solver mUINameSolver;
        Solver mUILoopSolver;
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
        {
            mConditionType = ZaySonInterface::ConditionType::Unknown;
            mWithElse = false;
        }
        ~ZayConditionElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            const String ConditionText = context.GetString();
            mConditionType = ZaySonUtility::ToCondition(ConditionText, &mWithElse);
            if(mConditionType == ZaySonInterface::ConditionType::Unknown)
                root.AddDebugError(String::Format("알 수 없는 조건문입니다(%s, Load)", (chars) ConditionText));

            if(sint32 PosB = ConditionText.Find(0, "(") + 1)
            {
                sint32 PosE = ConditionText.Find(PosB + 1, ")");
                if(PosE != -1)
                {
                    mConditionSolver.Link(root.ViewName());
                    mConditionSolver.Parse(String(((chars) ConditionText) + PosB, PosE - PosB));
                }
            }
        }

    public:
        static bool Test(const ZaySon& root, ZayUIs& dest, const Context& src)
        {
            if(ZaySonUtility::ToCondition(src.GetString("")) != ZaySonInterface::ConditionType::Unknown) // oncreate, onclick, compvalues의 경우
            {
                Object<ZayConditionElement> NewCondition(ObjectAllocType::Now);
                NewCondition->Load(root, src);
                dest.AtAdding() = (id_share) NewCondition;
                return true;
            }
            else if(ZaySonUtility::ToCondition(src("compname").GetString("")) != ZaySonInterface::ConditionType::Unknown) // compname의 경우
            {
                Object<ZayConditionElement> NewCondition(ObjectAllocType::Now);
                NewCondition->Load(root, src("compname"));
                dest.AtAdding() = (id_share) NewCondition;
                return true;
            }
            return false;
        };
        static sint32s Collect(chars viewname, const ZayUIs& uis, const ZayPanel* panel = nullptr)
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
                    if(CurCondition->mConditionType == ZaySonInterface::ConditionType::If)
                        IsTrue = (CurCondition->mConditionSolver.ExecuteOnly().ToInteger() != 0);
                    // 포커스확인
                    else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::IfFocused)
                    {
                        if(panel)
                        {
                            const String UIName = CurCondition->mConditionSolver.ExecuteOnly().ToText();
                            IsTrue = !!(panel->state(viewname + ('.' + UIName)) & PS_Focused);
                        }
                    }
                    // 호버확인
                    else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::IfHovered)
                    {
                        if(panel)
                        {
                            const String UIName = CurCondition->mConditionSolver.ExecuteOnly().ToText();
                            IsTrue = !!(panel->state(viewname + ('.' + UIName)) & PS_Hovered);
                        }
                    }
                    // 프레스확인
                    else if(CurCondition->mConditionType == ZaySonInterface::ConditionType::IfPressed)
                    {
                        if(panel)
                        {
                            const String UIName = CurCondition->mConditionSolver.ExecuteOnly().ToText();
                            IsTrue = !!(panel->state(viewname + ('.' + UIName)) & PS_Dragging);
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
                                else if(!CurCondition->mWithElse && // 새로운 조건그룹을 만나면 수락
                                    (CurCondition->mConditionType == ZaySonInterface::ConditionType::If ||
                                    CurCondition->mConditionType == ZaySonInterface::ConditionType::IfFocused ||
                                    CurCondition->mConditionType == ZaySonInterface::ConditionType::IfHovered ||
                                    CurCondition->mConditionType == ZaySonInterface::ConditionType::IfPressed))
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
        bool mWithElse;
        Solver mConditionSolver;
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
                mParamSolvers.AtAdding().Link(root.ViewName()).Parse(context[i].GetString());
        }

    public:
        Solvers mParamSolvers;
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
            mGlueFunction = nullptr;
        }
        ~ZayRequestElement() override {}

    public:
        void Load(const ZaySon& root, const Context& context) override
        {
            ZayUIElement::Load(root, context);

            const String TextTest = context.GetString("");
            sint32 PosB, PosE;
            if(ZaySonUtility::IsFunctionCall(TextTest, &PosB, &PosE)) // 함수호출
            {
                mRequestType = ZaySonInterface::RequestType::Function;
                mGlueFunction = root.FindGlue(TextTest.Left(PosB - 1));
                if(!mGlueFunction)
                    root.AddDebugError(String::Format("글루함수를 찾을 수 없습니다(%s, Load)", (chars) TextTest.Left(PosB - 1)));
                auto Params = ZaySonUtility::GetCommaStrings(TextTest.Middle(PosB, PosE - PosB));
                for(sint32 i = 0, iend = Params.Count(); i < iend; ++i)
                    mRSolvers.AtAdding().Link(root.ViewName()).Parse(Params[i]); // 파라미터들
            }
            else // 변수입력
            {
                chararray GetName;
                const auto& GetValue = context(0, &GetName);
                BOSS_ASSERT("함수콜이 아닌 Request는 좌우항으로 나누어져야 합니다", 0 < GetName.Count());
                if(0 < GetName.Count())
                {
                    mRequestType = ZaySonInterface::RequestType::Variable;
                    mLSolver.Parse(&GetName[0]); // 좌항
                    mRSolvers.AtAdding().Parse(GetValue.GetString()); // 우항
                }
            }
        }

    public:
        void InitForCreate()
        {
            if(mRequestType == ZaySonInterface::RequestType::Variable)
            {
                mLSolver.Link(mRefRoot->ViewName());
                mRSolvers.At(0).Link(mRefRoot->ViewName(), mLSolver.ExecuteVariableName());
                mRSolvers.At(0).Execute();
            }
        }
        void InitForClick(Map<String>& collector)
        {
            if(mRequestType == ZaySonInterface::RequestType::Variable)
            {
                mLSolver.Link(mRefRoot->ViewName());
                mRSolvers.At(0).Link(mRefRoot->ViewName());

                // 변수때는 변수명에서도 캡쳐 목록화
                const Strings Variables = mLSolver.GetTargetlessVariables();
                for(sint32 i = 0, iend = Variables.Count(); i < iend; ++i)
                    collector(Variables[i]) = Variables[i];
            }
            // 변수와 함수 모두 캡쳐가 필요한 변수의 목록화
            for(sint32 i = 0, iend = mRSolvers.Count(); i < iend; ++i)
            {
                const Strings Variables = mRSolvers[i].GetTargetlessVariables();
                for(sint32 j = 0, jend = Variables.Count(); j < jend; ++j)
                    collector(Variables[j]) = Variables[j];
            }
        }
        void Transaction()
        {
            if(mRequestType == ZaySonInterface::RequestType::Variable)
            {
                if(auto FindedSolver = Solver::Find(mRefRoot->ViewName(), mLSolver.ExecuteVariableName()))
                {
                    FindedSolver->Parse(mRSolvers[0].ExecuteOnly().ToText(true));
                    FindedSolver->Execute();
                }
                else mRefRoot->AddDebugError(String::Format("변수를 업데이트하는데 실패하였습니다(%s, Transaction)",
                    (chars) mLSolver.ExecuteVariableName()));
            }
            else if(mRequestType == ZaySonInterface::RequestType::Function)
            {
                if(mGlueFunction)
                {
                    ZayExtend::Payload ParamCollector = mGlueFunction->MakePayload();
                    for(sint32 i = 0, iend = mRSolvers.Count(); i < iend; ++i)
                        ParamCollector(mRSolvers[i].ExecuteOnly());
                    // ParamCollector가 소멸되면서 Glue함수가 호출됨
                }
            }
        }

    public:
        ZaySonInterface::RequestType mRequestType;
        Solver mLSolver;
        Solvers mRSolvers; // 변수, 함수파라미터들 겸용
        const ZayExtend* mGlueFunction;
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

            Map<String> CaptureCollector;
            hook(context("onclick"))
            for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
            {
                if(ZayConditionElement::Test(root, mClickCodes, fish[i]))
                    continue;
                Object<ZayRequestElement> NewRequest(ObjectAllocType::Now);
                NewRequest->Load(root, fish[i]);
                NewRequest->InitForClick(CaptureCollector);
                mClickCodes.AtAdding() = (id_share) NewRequest;
            }
            // 캡쳐목록 정리
            for(sint32 i = 0, iend = CaptureCollector.Count(); i < iend; ++i)
                mClickCodeCaptures.AtAdding() = *CaptureCollector.AccessByOrder(i);

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
        void Render(ZayPanel& panel, const String& defaultname, sint32& compmax, DebugLogs& logs) const override
        {
            if(0 < compmax)
            {
                // Debug정보구성
                if(--compmax == 0)
                    mRefRoot->SetDebugCompName(mCompName, mComment);

                if(auto CurComponent = mRefRoot->FindComponent(mCompName))
                {
                    // 디버깅 정보수집
                    auto AddDebugLog = [](DebugLogs& logs, ZayPanel& panel, bool fill, chars uiname)->void
                    {
                        auto& NewLog = logs.AtAdding();
                        NewLog.mRect = Rect(panel.toview(0, 0), panel.toview(panel.w(), panel.h()));
                        NewLog.mFill = fill;
                        NewLog.mUIName = uiname;
                    };

                    chars ViewName = mRefRoot->ViewName();
                    const String UIName((mUINameSolver.is_blank())? "" : (chars) mUINameSolver.ExecuteVariableName());
                    if(mCompValues.Count() == 0)
                    {
                        if(mUILoopSolver.is_blank())
                        {
                            String UINameTemp;
                            chars ComponentName = nullptr;
                            if(0 < UIName.Length()) ComponentName = UINameTemp = ViewName + ('.' + UIName);
                            else if(0 < mClickCodes.Count()) ComponentName = defaultname;

                            // 파라미터 없음
                            ZayExtend::Payload ParamCollector = CurComponent->MakePayload(ComponentName, this);
                            ZAY_EXTEND(ParamCollector >> panel)
                            {
                                if(mCompID == mRefRoot->debugFocusedCompID())
                                    AddDebugLog(logs, panel, CurComponent->HasContentComponent(), ComponentName);
                                RenderChildren(panel, ComponentName, defaultname, compmax, logs);
                            }
                        }
                        else // 반복문
                        {
                            const sint32 LoopCount = Math::Max(0, (sint32) mUILoopSolver.ExecuteOnly().ToInteger());
                            for(sint32 i = 0; i < LoopCount; ++i)
                            {
                                // 지역변수 수집
                                Solvers LocalSolvers;
                                if(0 < UIName.Length())
                                {
                                    LocalSolvers.AtAdding().Link(ViewName, UIName + "V").Parse(String::FromInteger(i)).Execute();
                                    LocalSolvers.AtAdding().Link(ViewName, UIName + "N").Parse(String::FromInteger(LoopCount)).Execute();
                                }
                                RenderChildren(panel, nullptr, defaultname + String::Format("_%d", i), compmax, logs);
                            }
                        }
                    }
                    else // CompValue항목이 존재할 경우
                    {
                        // 조건문에 의해 살아남은 유효한 CompValue를 모두 실행
                        sint32s CollectedCompValues = ZayConditionElement::Collect(ViewName, mCompValues, &panel);
                        for(sint32 i = 0, iend = CollectedCompValues.Count(); i < iend; ++i)
                        {
                            const String DefaultName(defaultname + ((1 < iend)? (chars) String::Format("_%d", i) : ""));
                            String UINameTemp;
                            chars ComponentName = nullptr;
                            if(0 < UIName.Length())
                                ComponentName = UINameTemp = ViewName + ('.' + UIName) + ((1 < iend)? (chars) String::FromInteger(i) : "");
                            else if(0 < mClickCodes.Count()) ComponentName = DefaultName;

                            // 지역변수 수집
                            Solvers LocalSolvers;
                            const Point ViewPos = panel.toview(0, 0);
                            LocalSolvers.AtAdding().Link(ViewName, "p.x").Parse(String::FromFloat(ViewPos.x)).Execute();
                            LocalSolvers.AtAdding().Link(ViewName, "p.y").Parse(String::FromFloat(ViewPos.y)).Execute();
                            LocalSolvers.AtAdding().Link(ViewName, "p.w").Parse(String::FromFloat(panel.w())).Execute();
                            LocalSolvers.AtAdding().Link(ViewName, "p.h").Parse(String::FromFloat(panel.h())).Execute();
                            if(1 < iend && 0 < UIName.Length())
                            {
                                LocalSolvers.AtAdding().Link(ViewName, UIName + "V").Parse(String::FromInteger(i)).Execute();
                                LocalSolvers.AtAdding().Link(ViewName, UIName + "N").Parse(String::FromInteger(iend)).Execute();
                            }

                            // 파라미터 수집
                            ZayExtend::Payload ParamCollector = CurComponent->MakePayload(ComponentName, this);
                            if(auto CurCompValue = (const ZayParamElement*) mCompValues[CollectedCompValues[i]].ConstPtr())
                            {
                                for(sint32 j = 0, jend = CurCompValue->mParamSolvers.Count(); j < jend; ++j)
                                    ParamCollector(CurCompValue->mParamSolvers[j].ExecuteOnly());
                            }
                            ZAY_EXTEND(ParamCollector >> panel)
                            {
                                if(mCompID == mRefRoot->debugFocusedCompID())
                                    AddDebugLog(logs, panel, CurComponent->HasContentComponent(), ComponentName);
                                RenderChildren(panel, ComponentName, DefaultName, compmax, logs);
                            }
                        }
                    }
                }
                else mRefRoot->AddDebugError(String::Format("컴포넌트함수를 찾을 수 없습니다(%s, Render)", (chars) mCompName));
            }
            else mRefRoot->AddDebugError("Debug모드의 컴포넌트 수량제한으로 부분출력중(→키를 눌러 해소)");
        }
        void RenderChildren(ZayPanel& panel, chars uiname, const String& defaultname, sint32& compmax, DebugLogs& logs) const
        {
            // 클릭코드를 위한 변수를 사전 캡쳐
            if(uiname != nullptr && 0 < mClickCodeCaptures.Count())
            {
                const Point ViewPos = panel.toview(0, 0);
                hook(mClickCodeCaptureValues(uiname))
                for(sint32 i = 0, iend = mClickCodeCaptures.Count(); i < iend; ++i)
                {
                    chars CurVariable = mClickCodeCaptures[i];
                    branch;
                    jump(!String::Compare(CurVariable, "p.x")) fish(CurVariable) = String::FromFloat(ViewPos.x);
                    jump(!String::Compare(CurVariable, "p.y")) fish(CurVariable) = String::FromFloat(ViewPos.y);
                    jump(!String::Compare(CurVariable, "p.w")) fish(CurVariable) = String::FromFloat(panel.w());
                    jump(!String::Compare(CurVariable, "p.h")) fish(CurVariable) = String::FromFloat(panel.h());
                    else fish(CurVariable) = Solver().Link(mRefRoot->ViewName()).Parse(CurVariable).ExecuteOnly().ToText(true);
                }
            }

            // 자식으로 재귀
            if(0 < mChildren.Count())
            {
                sint32s CollectedChildren = ZayConditionElement::Collect(mRefRoot->ViewName(), mChildren, &panel);
                for(sint32 i = 0, iend = CollectedChildren.Count(); i < iend; ++i)
                {
                    auto CurChildren = (const ZayUIElement*) mChildren[CollectedChildren[i]].ConstPtr();
                    CurChildren->Render(panel, defaultname + String::Format(".%d", i), compmax, logs);
                }
            }
        }
        void OnTouch(chars uiname) override
        {
            if(0 < mClickCodes.Count())
            {
                // 사전 캡쳐된 변수를 지역변수화
                Solvers LocalSolvers;
                hook(mClickCodeCaptureValues(uiname))
                for(sint32 i = 0, iend = fish.Count(); i < iend; ++i)
                {
                    chararray Variable;
                    auto& Value = *fish.AccessByOrder(i, &Variable);
                    LocalSolvers.AtAdding().Link(mRefRoot->ViewName(), &Variable[0]).Parse(Value).Execute();
                }

                // 클릭코드의 실행
                sint32s CollectedClickCodes = ZayConditionElement::Collect(mRefRoot->ViewName(), mClickCodes);
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
        Strings mClickCodeCaptures;
        typedef Map<String> CaptureValue;
        mutable Map<CaptureValue> mClickCodeCaptureValues;
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
                NewRequest->InitForCreate();
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
        void Render(ZayPanel& panel, const String& defaultname, sint32& compmax, DebugLogs& logs) const override
        {
            sint32s CollectedChildren = ZayConditionElement::Collect(mRefRoot->ViewName(), mChildren, &panel);
            for(sint32 i = 0, iend = CollectedChildren.Count(); i < iend; ++i)
            {
                auto CurChildren = (const ZayUIElement*) mChildren[CollectedChildren[i]].ConstPtr();
                CurChildren->Render(panel, defaultname + String::Format(".%d", i), compmax, logs);
            }
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
        return ZAY_GESTURE_NT(n, t, UIElement)
            {
                if(t == GT_InReleased)
                    ZayUIElement::TouchCore(n, UIElement);
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
        mViewName = ToReference(rhs.mViewName);
        delete mUIElement; mUIElement = rhs.mUIElement; rhs.mUIElement = nullptr;
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

    void ZaySon::Load(chars viewname, const Context& context)
    {
        mViewName = viewname;
        Reload(context);
    }

    void ZaySon::Reload(const Context& context)
    {
        BOSS_ASSERT("Reload는 Load후 호출가능합니다", 0 < mViewName.Length());
        delete mUIElement;
        auto NewView = new ZayViewElement();
        NewView->Load(*this, context);
        mUIElement = NewView;
    }

    const String& ZaySon::ViewName() const
    {
        return mViewName;
    }

    ZaySonInterface& ZaySon::AddComponent(ZayExtend::ComponentType type, chars name, ZayExtend::ComponentCB cb, chars paramcomment)
    {
        String OneName = name;
        const sint32 Pos = OneName.Find(0, ' ');
        if(Pos != -1) OneName = OneName.Left(Pos);

        auto& NewFunction = mExtendMap(OneName);
        NewFunction.ResetForComponent(type, cb);
        return *this;
    }

    ZaySonInterface& ZaySon::AddGlue(chars name, ZayExtend::GlueCB cb)
    {
        String OneName = name;
        const sint32 Pos = OneName.Find(0, ' ');
        if(Pos != -1) OneName = OneName.Left(Pos);

        auto& NewFunction = mExtendMap(OneName);
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
            ZayUIElement::DebugLogs LogCollector;
            mUIElement->Render(panel, mViewName, compmax, LogCollector);

            // 수집된 디버그로그(GUI툴에 의한 포커스표현)
            const Point ViewPos = panel.toview(0, 0);
            ZAY_MOVE(panel, -ViewPos.x, -ViewPos.y)
            for(sint32 i = 0, iend = LogCollector.Count(); i < iend; ++i)
            {
                hook(LogCollector[i])
                ZAY_RECT(panel, fish.mRect)
                ZAY_FONT(panel, Math::MaxF(0.8, (fish.mRect.r - fish.mRect.l) * 0.005))
                {
                    // 영역표시
                    ZAY_RGBA(panel, 255, 0, 0, 128)
                    if(fish.mFill)
                        panel.fill();
                    else ZAY_INNER(panel, 5)
                        panel.rect(10);
                    // UI명칭
                    if(0 < fish.mUIName.Length())
                    {
                        ZAY_RGB(panel, 0, 0, 0)
                        for(sint32 y = -1; y <= 1; ++y)
                        for(sint32 x = -1; x <= 1; ++x)
                            ZAY_MOVE(panel, x, y)
                                panel.text(panel.w() / 2, panel.h() / 2, fish.mUIName);
                        ZAY_RGB(panel, 255, 0, 0)
                            panel.text(panel.w() / 2, panel.h() / 2, fish.mUIName);
                    }
                }
            }
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

    ////////////////////////////////////////////////////////////////////////////////
    // ZaySonUtility
    ////////////////////////////////////////////////////////////////////////////////
    String ZaySonUtility::GetSafetyString(chars text)
    {
        String Collector;
        for(sint32 c = 0, cold = 0; true; ++c)
        {
            if(text[c] == '\0')
            {
                Collector += String(text + cold, c - cold).Trim();
                break;
            }
            else if(text[c] == ',')
            {
                Collector += String(text + cold, c - cold).Trim() + ", ";
                cold = c + 1;
            }
            else if(text[c] == '\'' || text[c] == '\"')
            {
                Collector += String(text + cold, c - cold).Trim();
                cold = c + 1;

                const char EndCode = text[c];
                while(text[++c] != '\0' && (text[c] != EndCode || text[c - 1] == '\\'));

                String LiteralText(text + cold, c - cold); // 따옴표안쪽
                LiteralText.Replace("\\\\", "([****])");
                LiteralText.Replace("\\\'", "\'");
                LiteralText.Replace("\\\"", "\"");
                LiteralText.Replace("\\", "\\\\"); // 슬래시기호는 쌍슬래시로
                LiteralText.Replace("([****])", "\\\\");
                LiteralText.Replace("\'", "\\\'"); // 내부 따옴표는 쌍슬래시+따옴표로
                LiteralText.Replace("\"", "\\\"");
                Collector += '\'' + LiteralText + '\''; // 외부 따옴표는 단따옴표만 인정
                cold = c + 1;
            }
        }
        return Collector;
    }

    Strings ZaySonUtility::GetCommaStrings(chars text)
    {
        Strings SubStrings;
        for(sint32 c = 0, cold = 0; true; ++c)
        {
            if(text[c] == '\0')
            {
                SubStrings.AtAdding() = String(text + cold, c - cold).Trim();
                break;
            }
            else if(text[c] == ',')
            {
                SubStrings.AtAdding() = String(text + cold, c - cold).Trim();
                cold = c + 1;
            }
            else if(text[c] == '\'' || text[c] == '\"')
            {
                const char EndCode = text[c];
                while(text[++c] != '\0' && (text[c] != EndCode || text[c - 1] == '\\'));
            }
        }
        return SubStrings;
    }

    bool ZaySonUtility::IsFunctionCall(chars text, sint32* prmbegin, sint32* prmend)
    {
        chars Focus = text;
        // 공백스킵
        while(*Focus == ' ') Focus++;
        // 함수명의 탈락조건
        if(boss_isalpha(*Focus) == 0)
            return false; // 첫글자가 영문이 아닐 경우 탈락
        while(*(++Focus) != '(' && *Focus != '\0')
            if(boss_isalnum(*Focus) == 0 && *Focus != '_')
                return false; // 영문, 숫자가 아닐 경우 함수탈락

        // 파라미터 발라내기
        if(*Focus == '(')
        {
            const sint32 ParamBeginPos = (Focus + 1) - text;
            sint32 DeepLevel = 1;
            while(*(++Focus) != '\0')
            {
                if(*Focus == '(') DeepLevel++;
                else if(*Focus == ')')
                {
                    if(--DeepLevel == 0)
                    {
                        if(prmbegin) *prmbegin = ParamBeginPos;
                        if(prmend) *prmend = Focus - text;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    ZaySonInterface::ConditionType ZaySonUtility::ToCondition(chars text, bool* withelse)
    {
        if(withelse) *withelse = false;
        branch;
        jump(!String::Compare(text, "if(", 3))
            return ZaySonInterface::ConditionType::If;
        jump(!String::Compare(text, "iffocused(", 10))
            return ZaySonInterface::ConditionType::IfFocused;
        jump(!String::Compare(text, "ifhovered(", 10))
            return ZaySonInterface::ConditionType::IfHovered;
        jump(!String::Compare(text, "ifpressed(", 10))
            return ZaySonInterface::ConditionType::IfPressed;
        jump(!String::Compare(text, "elif(", 5))
        {
            if(withelse) *withelse = true;
            return ZaySonInterface::ConditionType::If;
        }
        jump(!String::Compare(text, "eliffocused(", 12))
        {
            if(withelse) *withelse = true;
            return ZaySonInterface::ConditionType::IfFocused;
        }
        jump(!String::Compare(text, "elifhovered(", 12))
        {
            if(withelse) *withelse = true;
            return ZaySonInterface::ConditionType::IfHovered;
        }
        jump(!String::Compare(text, "elifpressed(", 12))
        {
            if(withelse) *withelse = true;
            return ZaySonInterface::ConditionType::IfPressed;
        }
        jump(!String::Compare(text, "else"))
            return ZaySonInterface::ConditionType::Else;
        jump(!String::Compare(text, "endif"))
            return ZaySonInterface::ConditionType::Endif;
        return ZaySonInterface::ConditionType::Unknown;
    }
}
