
#include <iostream>
#include <string>

#include "chevron/function.hpp"
#include "chevron/process/thread/thread_engine.hpp"
#include "chevron/process/memory/memory_core.hpp"

int main(int argc, char* argv[])
{
    //\\//
    //

    // Start...

    //
    //\\//

    return 0;
}


/*
 * :: Chevron Notes ::
 * 
 * [ Function Utilities Notes ]
 * -> Static callable type `Func`? (No heap alloc)
 */

// Potential Debug Mode Bad Address Signal:
// 0xDEAD2BAD == 3,735,890,861
// 0xDEA110C == 233,443,596
// 0xDEA110CA7ED
// 0xDEA110CA7ED64B (a.k.a -> DEA110CA7ED 64 B [Deallocated 64 Bit])
// == 62,664,538,318,624,331
