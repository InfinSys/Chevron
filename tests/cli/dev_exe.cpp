
#include <iostream>
#include <string>
#include"chevron/common/export.h"
#include"chevron/function.hpp"

int doNothing()
{
    return 15;
}

int main(int argc, char* argv[])
{
    //\\//
    //

    // Start...
    chevron::IntReturnNoArgs ggg{doNothing};
    chevron::NoArgsReturn<int> ddd{doNothing};

    //
    //\\//

    return 0;

    // FIX: Need specific constructor for lambda/functor using `::operator()`!!!
    // (`FuncPtr` class)

    // struct CallableModel --> ReturnType & Arguments --> FuncPtr or Func
    // Static callable type `Func`? (No heap alloc)
}
