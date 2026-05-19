
#include <iostream>
#include <string>

#include "chevron/function.hpp"
#include "chevron/units.hpp"
#include "chevron/common/macro_defs.h"
#include "chevron/memory/chunk_descriptor.hpp"
#include "chevron/memory/region.hpp"
#include "chevron/process/thread/thread_engine.hpp"
#include "chevron/process/memory/memory_core.hpp"
#include "chevron/process/memory/proc_allocator.hpp"
#include "chevron/utility/bits/alignment.hpp"
#include "chevron/utility/bits/powers.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    //\\//
    //

    // Start...
    using namespace chevron::digital_size::literals;

    constexpr chevron::MiB blockSize = 16_MiB;
    constexpr size_t blocksInChunk   = 32;
    constexpr chevron::MiB chunkSize = blockSize * blocksInChunk;
    constexpr size_t maxOSAllocs     = 8;
    constexpr chevron::GiB budgetMax = chevron::size_cast<chevron::GiB>(
        chunkSize * maxOSAllocs
    );

    chevron::process::MemoryPoolConfig config{};

    config.block_size            = blockSize;
    config.chunk_size            = chunkSize;
    config.block_alignment       = sizeof(std::max_align_t);
    config.budget_ceiling        = budgetMax;
    config.initial_thread_blocks = 2;
    config.max_thread_blocks     = 16;
    config.max_thread_batch_size = 8;

    try {
        config.isValidOrThrow();
    }
    catch (std::exception& e) {
        std::cout
            << "\nERROR: Process memory configuration validation failed.\n"
            << "\n\t" << e.what() << std::endl;
        return 1;
    }

    //

    //
    //\\//

    return 0;
}


/*
 * :: Chevron Notes ::
 * 
 * [ TO-DO ]
 * -> Start applying export macros to appropriate symbols
 * -> Figure out how exception and error codes will be integrated
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
