
#include <iostream>
#include <string>

#include "chevron/function.hpp"
#include "chevron/units.hpp"
#include "chevron/process/thread/thread_engine.hpp"
#include "chevron/process/memory/memory_core.hpp"
#include "chevron/utility/bits/alignment.hpp"
#include "chevron/utility/bits/powers.hpp"

int main(int argc, char* argv[])
{
    //\\//
    //

    // Start...
    using namespace chevron::digital_size::literals;

    constexpr chevron::MiB blockSize = 16_MiB;
    constexpr chevron::MiB chunkSize = blockSize * 24;
    constexpr chevron::GiB budgetMax = 3_GiB;

    chevron::process::MemoryPoolConfig config{};

    config.block_size = chevron::size_cast<chevron::Bytes>(blockSize);
    config.chunk_size = chevron::size_cast<chevron::Bytes>(chunkSize);
    config.block_alignment = sizeof(std::max_align_t);
    config.budget_ceiling = chevron::size_cast<chevron::Bytes>(budgetMax);
    config.initial_thread_blocks = 2;
    config.max_thread_blocks = 12;

    //

    //
    //\\//

    return 0;
}


/*
 * :: Chevron Notes ::
 * 
 * [ Function Utilities Notes ]
 * -> Static callable type `Func`? (No heap alloc)
 * 
 * [ Units Facility Notes ]
 * -> Length measure units? (i.e. milimeters, centimeters, meters, inches, feet, yards)
 *    -> (these can use device display info to determine pixel counts?)
 * -> Time measure units? (i.e. miliseconds, seconds, minutes, hours, days, months, years)
 */

// Potential Debug Mode Bad Address Signal:
// 0xDEAD2BAD == 3,735,890,861
// 0xDEA110C == 233,443,596
// 0xDEA110CA7ED
// 0xDEA110CA7ED64B (a.k.a -> DEA110CA7ED 64 B [Deallocated 64 Bit])
// == 62,664,538,318,624,331
