
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file export.h
 *
 * @brief
 * Library symbol visibility macro definitions.
 */

#if defined(_WIN32) || defined(__CYGWIN__)
    /*!
     * @brief (Building Dynamic Link Library) Chevron public
     * API macro for inline functions/variables, templates,
     * and header bound entities.
     */
    #define CHEVRON_HEADER_API
    #ifdef chevron_EXPORTS
        /*!
         * @brief (Building Dynamic Link Library) Chevron public
         * API macro for classes, functions, and extern variables.
         */
        #define CHEVRON_API __declspec(dllexport)
    #else
        /*!
         * @brief Chevron public API macro for classes, functions,
         * and extern variables.
         */
        #define CHEVRON_API __declspec(dllimport)
    #endif
#else
    #ifdef chevron_EXPORTS
        /*!
         * @brief (Building Shared Object) Chevron public API macro
         * for inline functions/variables, templates, and header
         * bound entities.
         */
        #define CHEVRON_HEADER_API __attribute__((visibility("default")))
        /*!
         * @brief (Building Shared Object) Chevron public API macro
         * for classes, functions, and extern variables.
         */
        #define CHEVRON_API __attribute__((visibility("default")))
    #else
        #define CHEVRON_API
        #define CHEVRON_HEADER_API
    #endif
#endif
