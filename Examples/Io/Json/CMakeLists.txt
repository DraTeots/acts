acts_add_library(
    ExamplesIoJson
    SHARED
    src/JsonGeometryList.cpp
    src/JsonMaterialWriter.cpp
    src/JsonSurfacesWriter.cpp
    src/JsonDigitizationConfig.cpp
)
target_include_directories(
    ActsExamplesIoJson
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(
    ActsExamplesIoJson
    PUBLIC
        Acts::Core
        Acts::PluginJson
        Acts::ExamplesDigitization
        Acts::ExamplesFramework
        Acts::ExamplesMaterialMapping
        Acts::ExamplesTrackFinding
)

acts_compile_headers(ExamplesIoJson GLOB "include/**/*.hpp")
