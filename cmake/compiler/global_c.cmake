
#==========================================
#     Global C COMPILER CONFIGURATION
#==========================================

# Preprocessor definitions target
add_library(CHEVRON_global_c_defines INTERFACE)

# Define preprocessor definitions
target_compile_definitions(
    CHEVRON_global_c_defines

    INTERFACE
        # Unconditional preprocessor definitions
        CHEVRON_APP

        # Preprocessor definitions on debug
        $<$<CONFIG:Debug>:
            _CHEVRON_DEBUG
        >

        # Preprocessor definitions on release
        $<$<CONFIG:Release>:
            CHEVRON_RELEASE
        >

        $<$<BOOL:${Chevron_SHARED_BUILD}>:
            CHEVRON_SHARED
        >

        $<$<BOOL:${chevron_EXPORTS}>:
            chevron_EXPORTS=1
        >
)
