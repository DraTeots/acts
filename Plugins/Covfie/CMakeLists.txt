acts_add_library(
    PluginCovfie
    SHARED
    src/FieldConversion.cpp
    ACTS_INCLUDE_FOLDER include/Acts
)

target_include_directories(
    ActsPluginCovfie
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(ActsPluginCovfie PUBLIC Acts::Core covfie::core)
