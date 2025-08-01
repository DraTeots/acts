# get the path of the test data directory
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Data" acts_test_data_dir)

add_library(
    ActsTestsCommonHelpers
    SHARED
    src/DataDirectory.cpp
    src/CylindricalDetector.cpp
)

add_library(Acts::TestsCommonHelpers ALIAS ActsTestsCommonHelpers)

target_compile_definitions(
    ActsTestsCommonHelpers
    PRIVATE
        "ACTS_TEST_DATA_DIR=\"${acts_test_data_dir}\""
        BOOST_FILESYSTEM_NO_DEPRECATED
)
target_include_directories(
    ActsTestsCommonHelpers
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(
    ActsTestsCommonHelpers
    PUBLIC Acts::Core Acts::ExamplesFramework
    PRIVATE std::filesystem
)

acts_compile_headers(TestsCommonHelpers GLOB "include/Acts/**/*.hpp")
