acts_add_library(ExamplesIoSvg SHARED src/SvgTrackingGeometryWriter.cpp)
target_include_directories(
    ActsExamplesIoSvg
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(
    ActsExamplesIoSvg
    PUBLIC
        Acts::Core
        Acts::ExamplesFramework
        Acts::PluginActSVG
        Threads::Threads
)
