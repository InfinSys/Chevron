
#include <iostream>
#include <string>

#include "chevron/function.hpp"
#include "chevron/units.hpp"
#include "chevron/common/macro_defs.h"
#include "chevron/memory/chunk_descriptor.hpp"
#include "chevron/memory/region.hpp"
#include "chevron/process/thread/threading_core.hpp"
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

    chevron::process::ProcessMemoryPool pool{config};
    chevron::memory::MemoryRegion blockAlloc = pool.allocate();

    auto* ffw = static_cast<chevron::process::MemoryPoolConfig*>(blockAlloc.base());
    new (ffw) chevron::process::MemoryPoolConfig{};
    *ffw = config;
    *(ffw + 1) = config;

    pool.deallocate(blockAlloc);

    constexpr bool conceptTest1 = chevron::units::concepts::lossless_digital_size_conversion<chevron::GiB, chevron::MiB>;
    constexpr bool conceptTest2 = chevron::units::concepts::matching_digital_size_systems<chevron::MiB, chevron::GB>;
    constexpr bool conceptTest3 = chevron::units::concepts::decimal_digital_size_units<chevron::MB>;
    constexpr bool conceptTest4 = chevron::units::concepts::binary_digital_size_units<chevron::TB>;
    constexpr bool conceptTest5 = chevron::units::concepts::digital_size_unit_api<chevron::KB>;
    constexpr bool conceptTest6 = chevron::units::concepts::digital_size_unit_arithmetic<chevron::MiB>;
    constexpr bool conceptTest7 = chevron::units::concepts::digital_size_units<chevron::TiB>;

    constexpr uint64_t maxPow2  = chevron::bits::max_power_of_two_exponent<uint64_t>;
    constexpr uint32_t maxPow10 = chevron::bits::max_power_of_ten_exponent<uint32_t>;
    constexpr uint64_t align12  = chevron::bits::alignUp(13ULL, 12ULL);
    constexpr uint64_t nPOfTwo  = chevron::bits::powerOfTwoCeil(3ULL);
    constexpr uint64_t pPOfTwo  = chevron::bits::powerOfTwoFloor(1ULL);
    constexpr uint64_t nPOfTen  = chevron::bits::powerOfTenCeil(101ULL);
    constexpr uint64_t pPOfTen  = chevron::bits::powerOfTenFloor(999ULL);
    constexpr double sizeDiv    = 1_MiB / 1_KiB;
    constexpr double sizeDiv2   = 1_GiB / 1_KiB;
    constexpr bool pOfTwo       = chevron::bits::isPowerOfTwo(128ULL);
    constexpr bool pOfTen       = chevron::bits::isPowerOfTen(1000ULL);

    constexpr chevron::MiB twoGiB    = 1024_MiB * 2;
    constexpr chevron::MiB qtrTwoGB  = twoGiB / 4;
    constexpr chevron::GiB noGiB     = 12_GiB - 12_GiB;
    constexpr chevron::MiB addMiB    = twoGiB + qtrTwoGB;
    constexpr chevron::Bytes mixUp   = 1_MiB + 1_MB + 24_Bytes;
    constexpr chevron::MiB modTiB    = 2_MiB % 11_MiB;
    constexpr chevron::MiB mixUpCast = chevron::size_cast<chevron::MiB>(mixUp);
    constexpr chevron::GiB initDiff  = chevron::GiB{ 2048_MiB };
    constexpr chevron::MiB absDiff   = noGiB.absoluteDifference(4096_MiB);

    chevron::MiB trz{ 12_MiB };
    chevron::MiB* gv  = &trz;
    chevron::MiB* gvz = chevron::bits::alignUp(gv, alignof(chevron::MiB));
    gvz               = chevron::bits::alignDown(gv, alignof(chevron::MiB));
    trz               %= 12_GiB;

    constexpr uint64_t expon = chevron::math::exponent_v<2, 8>;

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
