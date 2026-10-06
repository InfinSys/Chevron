
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file scope.hpp
 *
 * @brief
 * // TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 05-26-2026
 */

#ifndef CHEVRON_LIB_MEMORY_HIERARCHY_SCOPE_H_
#define CHEVRON_LIB_MEMORY_HIERARCHY_SCOPE_H_

#include "chevron/memory/region.hpp"
#include "chevron/memory/free_region_chain.hpp"
#include "chevron/memory/atomic_free_list.hpp"
#include "chevron/utility/function/callable.hpp"
#include "chevron/common/export.h"

namespace chevron::process
{

class MemoryCore;

};

namespace chevron::memory
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class CHEVRON_API MemoryScope {
    friend chevron::process::MemoryCore;

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PRIVATE] NESTED TYPES
    // ===================================================================================== //
    
    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    struct RootAcquisition {
    Callable<MemoryRegion, FuncArgs<>> acquire;
    };

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PRIVATE] STATIC VARIABLES
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    static inline RootAcquisition* root_acquire_ = nullptr;

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PUBLIC] NESTED TYPES
    // ===================================================================================== //
public:
    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    struct Config {
    units::Bytes acquire;
    units::Bytes distribute;
    };

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //

    /*! @brief Construct root-level memory hierarchy node. */
    explicit MemoryScope(Config scope_config);

    /*! @brief Construct nested memory hierarchy node. */
    MemoryScope(MemoryScope& parent, Config scope_config);

    ~MemoryScope() noexcept;

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    // TODO: INCOMPLETE IMPLEMENTATION!!!
    // Start...

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] bool isChildOfRoot() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes acquisitionSize() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes distributionSize() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void extendMemory(const units::Bytes bytes);

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] MemoryRegion allocateBlock();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void releaseBlock(MemoryRegion& block);

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void releaseUnused();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void releaseMemory();

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | OPERATORS
    // ===================================================================================== //

    MemoryScope& operator=(const MemoryScope&) = delete;

    MemoryScope& operator=(MemoryScope&&) = delete;

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PROTECTED] MEMBER METHODS
    // ===================================================================================== //
protected:
    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes freeBytesEstimate() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes totalBytesEstimate() const noexcept;

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    AtomicFreeList free_list_;
    MemoryScope* parent_scope_;
    AtomicFreeList* parent_free_list_;
    Config config_;
    // TODO: INCOMPLETE IMPLEMENTATION!!!

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PRIVATE] MEMBER METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void valid_configuration_or_throw() const;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void carve_and_link(const MemoryRegion& chunk) noexcept;

    // ===================================================================================== //
    //      <> chevron::memory::MemoryScope | [PRIVATE] STATIC METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    static void set_global_root_acquisition(
    const Callable<MemoryRegion, FuncArgs<>>& acquisition_call) noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    static void push_batch(AtomicFreeList& free_list, FreeRegionChain& batch);

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] static FreeRegionChain pop_batch(AtomicFreeList& free_list, const size_t batch_size);
};

}

#endif // CHEVRON_LIB_MEMORY_HIERARCHY_SCOPE_H_
