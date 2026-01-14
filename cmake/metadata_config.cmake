
#==================================
#     PROJECT METADATA MODULE
#==================================

include("${CHEVRON_CMAKE_MODULES_DIR}/utility/metadata_tools.cmake")

create_temporary_cache_list()

set_metadata(PUBLISHER "InfinSys, LLC." DESCRIPTION "Product publisher")
set_metadata(PRODUCT_TYPE "Library" DESCRIPTION "Software type")
set_metadata(INTERFACE_TYPE "API" DESCRIPTION "Product interface")
set_metadata(UUID "ISJTB-CXX-XL20260108-000003" DESCRIPTION "Product unique identifier")
set_metadata(LICENSE_TYPE "GPLv3" DESCRIPTION "Product license type")
set_metadata(FULL_NAME "Chevron" DESCRIPTION "Product name")
set_metadata(SHORT_NAME "Chevron" DESCRIPTION "Product short name")
set_metadata(MAIN_BINARY_NAME "chevron" DESCRIPTION "Main binary")
set_metadata(META_NAMESPACE "CHEVRON" DESCRIPTION "Project metadata namespace")

# Temporarily cached variables for code generation
create_template_reference(SFTW_PUBLISHER    PUBLISHER)
create_template_reference(SFTW_TYPE         PRODUCT_TYPE)
create_template_reference(SFTW_INTERFACE    INTERFACE_TYPE)
create_template_reference(SFTW_UUID         UUID)
create_template_reference(SFTW_LICENSE_TYPE LICENSE_TYPE)
create_template_reference(SFTW_NAME         FULL_NAME)
create_template_reference(SFTW_SHORT_NAME   SHORT_NAME)
create_template_reference(SFTW_MAIN_BINARY  MAIN_BINARY_NAME)
create_template_reference(SFTW_META_PREFIX  META_NAMESPACE)

configure_template(
    "${CHEVRON_SOURCE_DIR}/docs/templ/README.md.in"
    "${CHEVRON_SOURCE_DIR}/README.md"
)

configure_template(
    "${CHEVRON_SOURCE_DIR}/docs/templ/Doxyfile.in"
    "${CHEVRON_SOURCE_DIR}/docs/Doxyfile"
)

configure_template(
    "${CHEVRON_SOURCE_DIR}/docs/templ/index.html.in"
    "${CHEVRON_SOURCE_DIR}/docs/ref/index.html"
)

configure_template(
    "${CHEVRON_SOURCE_DIR}/docs/templ/inaug.md.in"
    "${CHEVRON_SOURCE_DIR}/docs/prj/inaug.md"
)

configure_template(
    "${CHEVRON_SOURCE_DIR}/lib/include/chevron_meta/templ/info.h.in"
    "${CHEVRON_SOURCE_DIR}/lib/include/chevron_meta/info.h"
)

configure_template(
    "${CHEVRON_SOURCE_DIR}/lib/include/chevron_meta/templ/version.h.in"
    "${CHEVRON_SOURCE_DIR}/lib/include/chevron_meta/version.h"
)

clear_temporary_cache()
