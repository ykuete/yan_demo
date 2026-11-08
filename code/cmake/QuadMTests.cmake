# quad_add_test(<name> SOURCES <files...> LIBS <targets...>)
# Builds one QtTest executable and registers it with CTest. Tests run with the offscreen
# platform plugin so they work on machines (and CI runners) without a display.
function(quadm_add_test name)
    cmake_parse_arguments(T "" "" "SOURCES;LIBS" ${ARGN})
    add_executable(${name} ${T_SOURCES})
    target_link_libraries(${name} PRIVATE Qt6::Test quadm_warning ${T_LIBS})
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()