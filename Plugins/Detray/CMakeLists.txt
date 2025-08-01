acts_add_library(
    PluginDetray
    SHARED
    src/DetrayConversionUtils.cpp
    src/DetrayConverter.cpp
    src/DetrayGeometryConverter.cpp
    src/DetrayMaterialConverter.cpp
    src/DetraySurfaceGridsConverter.cpp
    ACTS_INCLUDE_FOLDER include/Acts
)

add_dependencies(ActsPluginDetray detray::core covfie::core vecmem::core)

target_include_directories(
    ActsPluginDetray
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ActsPluginDetray
    PUBLIC
        Acts::Core
        Acts::PluginJson
        detray::core
        detray::core_array
        detray::io
        detray::detectors
        detray::test_utils
        vecmem::core
)
