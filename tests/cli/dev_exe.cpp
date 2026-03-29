
#include <iostream>
#include <string>

#include "chevron/function.hpp"

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
    // (`Callable` class)
    
    // Static callable type `Func`? (No heap alloc)
}
