
#include <iostream>
#include <string>
#include"chevron/model/function/func_args.hpp"
#include"chevron/model/function/func_pointer.hpp"

template <typename... ArgsT>
using FuncArgs = chevron::model::FuncArgs<ArgsT...>;

template <typename ReturnT, typename... ArgsT>
using FuncPtr = chevron::model::FuncPtr<ReturnT, ArgsT...>;

bool demoMethod(int val1, int val2)
{
    return val1 == val2;
}

char demoMethod2(bool good)
{
    return 'G';
}

int demoMethod3()
{
    return 100;
}

struct tester {
    tester(const int initVal) : value{ initVal } {}

    int getVal() const { return value; }
    void setValue(const int newVal) { value = newVal; }
    bool isMatch(const int val1, const int val2) const { return val1 == val2; }

    int operator()() const { return value; }

    int value;
};

int main(int argc, char* argv[])
{
    //\\//
    //

    FuncArgs cmdlArgs{ argc, argv };

    auto lambda = [&cmdlArgs](const int val, const char& te) -> bool {
        return val > 75;
    };

    tester ggg{ 12 };

    FuncArgs args{ 10, 10 };
    FuncPtr demoCall{ demoMethod };
    FuncPtr callPtr{ &ggg, &tester::isMatch };
    FuncPtr nCallPtr{ &ggg, &tester::setValue };
    FuncPtr freeCall{ demoMethod2 };
    chevron::model::FuncPtr lambCall{lambda};  ///< Deduction issue here...
    chevron::model::FuncPtr functCall{ ggg };  ///< Deduction issue here...
    FuncPtr<int> noArgCall;

    if (freeCall)
        std::cout << freeCall(false) << std::endl;

    if (callPtr)
        std::cout << callPtr(args) << std::endl;

    if (!noArgCall.bind(demoMethod3))
        return -1;

    if (!callPtr.bind(&ggg, &tester::isMatch))
        return -1;

    noArgCall();
    callPtr(25, 16);
    callPtr(args);
    lambCall(12, 'C');
    functCall();

    //
    //\\//

    return 0;

    // FIX: Need specific constructor for lambda/functor using `::operator()`!!!

    // struct FunctionModel --> ReturnType & Arguments --> FuncPtr or Func

    // Static callable type? (No heap alloc)
}
