cmake_minimum_required(VERSION 3.16)

project(SpiceLite VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(SpiceLite
    src/main.cpp
    src/Parser.cpp
    src/Solver.cpp
    src/components/Resistor.cpp
    src/components/VoltageSource.cpp
)

target_include_directories(SpiceLite PRIVATE include)
target_link_libraries(SpiceLite PRIVATE Eigen3::Eigen)

include(CTest)
if(BUILD_TESTING)
    add_test(NAME dc_resistor COMMAND SpiceLite ${CMAKE_SOURCE_DIR}/netlists/smoke.cir)
    set_tests_properties(dc_resistor PROPERTIES PASS_REGULAR_EXPRESSION "V\\(1\\) = 5")
    add_test(NAME dependent_source COMMAND SpiceLite ${CMAKE_SOURCE_DIR}/netlists/dependent.cir)
    set_tests_properties(dependent_source PROPERTIES PASS_REGULAR_EXPRESSION "V\\(2\\) = 6")
    add_test(NAME rc_transient COMMAND SpiceLite ${CMAKE_SOURCE_DIR}/netlists/rc-transient.cir)
    set_tests_properties(rc_transient PROPERTIES PASS_REGULAR_EXPRESSION "Time \\(s\\)")
    add_test(NAME rc_ac COMMAND SpiceLite ${CMAKE_SOURCE_DIR}/netlists/rc-ac.cir)
    set_tests_properties(rc_ac PROPERTIES PASS_REGULAR_EXPRESSION "Frequency \\(Hz\\)")
    add_test(NAME current_source COMMAND SpiceLite ${CMAKE_SOURCE_DIR}/netlists/current-source.cir)
    set_tests_properties(current_source PROPERTIES PASS_REGULAR_EXPRESSION "V\\(1\\) = -2")
    add_test(NAME inductor_dc COMMAND SpiceLite ${CMAKE_SOURCE_DIR}/netlists/inductor-dc.cir)
    set_tests_properties(inductor_dc PROPERTIES PASS_REGULAR_EXPRESSION "V\\(2\\) = 5")
endif()

if(MSVC)
    target_compile_options(SpiceLite PRIVATE /W4 /permissive-)
else()
    target_compile_options(SpiceLite PRIVATE -Wall -Wextra -Wpedantic)
endif()
