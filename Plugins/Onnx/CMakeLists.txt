acts_add_library(
    PluginOnnx
    SHARED
    # source files
    src/OnnxRuntimeBase.cpp
    src/MLTrackClassifier.cpp
    ACTS_INCLUDE_FOLDER include/Acts
)

target_include_directories(
    ActsPluginOnnx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ActsPluginOnnx PUBLIC Acts::Core onnxruntime::onnxruntime)
