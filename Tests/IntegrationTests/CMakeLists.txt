# integration tests are different and should not be registered with the
# preconfigured `tests` target. manually add a separate one.
add_custom_target(integrationtests)
# add an integrationtest executable w/ default dependencies and register it.
#
# the common libraries which are linked to every integrationtest can be
# extended by setting the `integrationtest_extra_libraries` variables before
# calling the macro.
macro(add_integrationtest _name)
    # automatically prefix the target name
    set(_target "ActsIntegrationTest${_name}")
    add_executable(${_target} ${ARGN})
    # define required BOOST_TEST_... macros here to ensure consistent names
    target_compile_definitions(
        ${_target}
        PRIVATE "-DBOOST_TEST_DYN_LINK" "-DBOOST_TEST_MODULE=${_target}"
    )
    target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(
        ${_target}
        PRIVATE
            Acts::Core
            Acts::TestsCommonHelpers
            Boost::unit_test_framework
            ${integrationtest_extra_libraries}
    )
    # register as integration test executable
    set(_run "RunIntegrationTest${_name}")
    add_custom_target(${_run} COMMAND ${_target})
    add_dependencies(integrationtests ${_run})
endmacro()

add_integrationtest(InterpolatedSolenoidBField InterpolatedSolenoidBFieldTest.cpp)
add_integrationtest(NavigatorConsistency NavigatorConsistency.cpp)
add_integrationtest(PrintDataDirectory PrintDataDirectory.cpp)
add_integrationtest(PropagationAtlasConstant PropagationAtlasConstant.cpp)
add_integrationtest(PropagationBentTracks PropagationBentTracks.cpp)
add_integrationtest(PropagationCompareAtlasEigenConstant PropagationCompareAtlasEigenConstant.cpp)
add_integrationtest(PropagationCompareEigenStraightLine PropagationCompareEigenStraightLine.cpp)
add_integrationtest(PropagationEigenConstant PropagationEigenConstant.cpp)
add_integrationtest(PropagationEigenDenseConstant PropagationEigenDenseConstant.cpp)
add_integrationtest(PropagationStraightLine PropagationStraightLine.cpp)
add_integrationtest(PropagationSympyConstant PropagationSympyConstant.cpp)
add_integrationtest(PropagationSympyDenseConstant PropagationSympyDenseConstant.cpp)

add_subdirectory_if(Fatras ACTS_BUILD_FATRAS)
