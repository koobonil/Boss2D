﻿#pragma once
#include <boss.hpp>

namespace BOSS
{
    class Solver;
    enum class SolverValueType {Integer, Float, Text};
    enum class SolverOperandType {Unknown, Literal, Variable, Formula};
    enum class SolverOperatorType {Unknown,
        Addition, Subtract, Multiply, Divide, Remainder, // +, -, *, /, %
        Greater, GreaterOrEqual, Less, LessOrEqual, Equal, Different, // <, <=, >, >=, ==, !=
        Function_Min, Function_Max}; // [min], [max]

    // 업데이트체인
    class SolverChainPair
    {
        public: SolverChainPair() {mDest = nullptr;}
        public: ~SolverChainPair() {}

        // 인터페이스
        public: void ResetDest(Solver* solver, bool needupdate);
        public: bool RemoveDest();
        public: void AddObserver(Solver* solver);
        public: bool SubObserver(Solver* solver);
        public: void RenualAllObservers();
        public: Solver* dest() {return mDest;}
        public: const Solver* dest() const {return mDest;}

        // 멤버
        private: Solver* mDest;
        private: Array<Solver*, datatype_pod_canmemcpy_zeroset> mObservers;
    };
    typedef Map<SolverChainPair> SolverChain;

    // 연산값
    class SolverValue
    {
        public: SolverValue(SolverValueType type = SolverValueType::Integer);
        public: SolverValue(const SolverValue& rhs);
        public: SolverValue(SolverValue&& rhs);
        public: ~SolverValue();
        public: SolverValue& operator=(const SolverValue& rhs);
        public: SolverValue& operator=(SolverValue&& rhs);

        // 타입
        public: typedef sint64 Integer;
        public: typedef double Float;
        public: typedef String Text;

        public: static SolverValue MakeByInteger(Integer value);
        public: static SolverValue MakeByFloat(Float value);
        public: static SolverValue MakeByText(Text value);

        // 함수
        public: SolverValueType GetType() const;
        public: Integer ToInteger() const;
        public: Float ToFloat() const;
        public: Text ToText() const;
        private: SolverValueType GetMergedType(const SolverValue& rhs) const;
        public: SolverValue Addition(const SolverValue& rhs) const;
        public: SolverValue Subtract(const SolverValue& rhs) const;
        public: SolverValue Multiply(const SolverValue& rhs) const;
        public: SolverValue Divide(const SolverValue& rhs) const;
        public: SolverValue Remainder(const SolverValue& rhs) const;
        public: SolverValue Greater(const SolverValue& rhs) const;
        public: SolverValue GreaterOrEqual(const SolverValue& rhs) const;
        public: SolverValue Less(const SolverValue& rhs) const;
        public: SolverValue LessOrEqual(const SolverValue& rhs) const;
        public: SolverValue Equal(const SolverValue& rhs) const;
        public: SolverValue Different(const SolverValue& rhs) const;
        public: SolverValue Function_Min(const SolverValue& rhs) const;
        public: SolverValue Function_Max(const SolverValue& rhs) const;

        // 멤버
        private: SolverValueType mType;
        private: Integer mInteger;
        private: Float mFloat;
        private: Text mText;
    };
    typedef Array<SolverValue> SolverValues;

    // 피연산항
    class SolverOperand
    {
        BOSS_DECLARE_NONCOPYABLE_INITIALIZED_CLASS(SolverOperand, mOperandType(rhs.mOperandType))
        public: SolverOperand(SolverOperandType type = SolverOperandType::Unknown) : mOperandType(type) {}
        public: ~SolverOperand() {}

        // 인터페이스
        public: inline SolverOperandType type() const {return mOperandType;}
        // 가상 인터페이스
        public: virtual void PrintString(String& collector) const = 0; // 데이터출력
        public: virtual void UpdateChain(Solver* solver, SolverChain* chain) = 0; // 체인업데이트
        public: virtual float reliable() const = 0; // 신뢰도
        public: virtual SolverValue result(SolverValue zero) const = 0; // 결과값
        public: virtual buffer clone() const = 0; // 자기복제

        // 멤버
        protected: const SolverOperandType mOperandType;
    };
    class SolverOperandBlank : public SolverOperand
    {
        public: void PrintString(String& collector) const override {}
        public: void UpdateChain(Solver* solver, SolverChain* chain) override {}
        public: float reliable() const override {return 0;}
        public: SolverValue result(SolverValue zero) const override {return zero;}
        public: buffer clone() const override {return nullptr;}
    };
    typedef Object<SolverOperandBlank, datatype_class_nomemcpy, ObjectAllocType::Now> SolverOperandObject;
    typedef Array<SolverOperandObject> SolverOperandObjects;

    // 연립방정식
    class Solver
    {
        public: Solver();
        public: ~Solver();
        public: Solver(const Solver& rhs) {operator=(rhs);}
        public: Solver& operator=(const Solver& rhs);

        // 인터페이스
        public: void Link(chars chain, chars variable, bool needupdate);
        public: void Unlink();
        public: static Solver* Find(chars chain, chars variable);
        public: void Parse(chars formula);
        public: void Execute();
        public: inline const String& linked_variable() const {return mLinkedVariable;}
        public: inline const String& parsed_formula() const {return mParsedFormula;}
        public: inline float reliable() const {return mReliable;}
        public: inline SolverValue result() const {return mResult;}
        public: inline bool is_result_updated() const {return (mResultUpdateId != 0);}
        public: inline bool is_result_matched(uint32 id) const {return (mResultUpdateId == id);}
        public: inline void ClearUpdateMark() {mResultUpdateId = 0;}

        // 멤버
        private: SolverChain* mLinkedChain;
        private: String mLinkedVariable;
        private: String mParsedFormula;
        private: SolverOperandObject mOperandTop;
        private: float mReliable;
        private: SolverValue mResult;
        private: uint32 mResultUpdateId;
    };
    typedef Array<Solver> Solvers;
}
