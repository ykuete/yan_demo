# quadm_core: the application's vocabulary. Qt Core only - no widgets, no processes, no files.
add_library(quadm_core STATIC
    fieldpolicy.h
    fieldpolicy.cpp
    metadatadocument.h
    metadatadocument.cpp
    result.h
    tags.h
    tags.cpp
    version.h
    version.cpp
)
target_include_directories(quadm_core PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_definitions(quadm_core PRIVATE QUADM_VERSION="${PROJECT_VERSION}")
target_link_libraries(quadm_core PUBLIC Qt6::Core PRIVATE quadm_warnings)
