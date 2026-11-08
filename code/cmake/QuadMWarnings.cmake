# One interface target that every QuadM target links, so warnings are configured in a single place.
add_library(quadm_warnings INTERFACE)
if(MSVC)
    target_compile_options(quadm_warnings INTERFACE /w4 $<$<BOOL:${QUADM_WARNINGS_AS_ERRORS}>:/WX>)
else()
    target_compile_options(quadm_warning INTERFACE -Wall -Wextra -Wpedantic
        $<$<BOOL:${QUADM_WARNINGS_AS_ERRORS}>:Werror>)
endif()