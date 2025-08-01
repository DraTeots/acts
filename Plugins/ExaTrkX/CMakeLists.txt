acts_add_library(
    PluginExaTrkX
    SHARED
    src/ExaTrkXPipeline.cpp
    src/Tensor.cpp
    src/BoostTrackBuilding.cpp
    src/TruthGraphMetricsHook.cpp
    src/GraphStoreHook.cpp
    ACTS_INCLUDE_FOLDER include/Acts
)

if(ACTS_EXATRKX_ENABLE_CUDA)
    target_compile_definitions(ActsPluginExaTrkX PUBLIC ACTS_EXATRKX_WITH_CUDA)
    target_sources(
        ActsPluginExaTrkX
        PRIVATE src/CudaTrackBuilding.cu src/JunctionRemoval.cu src/Tensor.cu
    )
endif()

if(ACTS_EXATRKX_ENABLE_MODULEMAP)
    target_compile_definitions(
        ActsPluginExaTrkX
        PUBLIC ACTS_EXATRKX_WITH_MODULEMAP
    )

    target_link_libraries(
        ActsPluginExaTrkX
        PRIVATE ModuleMapGraph::CPU ModuleMapGraph::GPU
    )
    target_sources(ActsPluginExaTrkX PRIVATE src/ModuleMapCuda.cu)
endif()

if(ACTS_EXATRKX_ENABLE_ONNX)
    target_sources(ActsPluginExaTrkX PRIVATE src/OnnxEdgeClassifier.cpp)
    target_link_libraries(ActsPluginExaTrkX PRIVATE onnxruntime::onnxruntime)
    target_compile_definitions(
        ActsPluginExaTrkX
        PUBLIC ACTS_EXATRKX_ONNX_BACKEND
    )
endif()

if(ACTS_EXATRKX_ENABLE_TORCH)
    target_sources(
        ActsPluginExaTrkX
        PRIVATE
            src/TorchEdgeClassifier.cpp
            src/TorchMetricLearning.cpp
            src/buildEdges.cpp
    )
endif()

if(ACTS_EXATRKX_ENABLE_TENSORRT)
    find_package(TensorRT REQUIRED)
    message(STATUS "Found TensorRT ${TensorRT_VERSION}")
    target_link_libraries(
        ActsPluginExaTrkX
        PUBLIC trt::nvinfer trt::nvinfer_plugin
    )
    target_sources(ActsPluginExaTrkX PRIVATE src/TensorRTEdgeClassifier.cpp)
    target_compile_definitions(
        ActsPluginExaTrkX
        PUBLIC ACTS_EXATRKX_WITH_TENSORRT
    )
endif()

target_include_directories(
    ActsPluginExaTrkX
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ActsPluginExaTrkX PUBLIC Acts::Core Boost::boost)

target_link_libraries(ActsPluginExaTrkX PRIVATE std::filesystem)

if(ACTS_EXATRKX_ENABLE_CUDA)
    target_compile_features(ActsPluginExaTrkX PUBLIC cuda_std_20)
    set_target_properties(
        ActsPluginExaTrkX
        PROPERTIES CUDA_STANDARD_REQUIRED ON CUDA_SEPARABLE_COMPILATION OFF
    )
    target_compile_options(
        ActsPluginExaTrkX
        PRIVATE
            $<$<COMPILE_LANGUAGE:CUDA>:-g
            --generate-line-info
            --expt-relaxed-constexpr
            --extended-lambda>
    )
    target_compile_definitions(
        ActsPluginExaTrkX
        PUBLIC CUDA_API_PER_THREAD_DEFAULT_STREAM
    )
    target_link_libraries(ActsPluginExaTrkX PUBLIC CUDA::cudart)
else()
    target_compile_definitions(ActsPluginExaTrkX PUBLIC ACTS_EXATRKX_CPUONLY)
endif()

if(ACTS_EXATRKX_ENABLE_TORCH)
    target_compile_definitions(
        ActsPluginExaTrkX
        PUBLIC ACTS_EXATRKX_TORCH_BACKEND
    )
    target_link_libraries(ActsPluginExaTrkX PRIVATE ${TORCH_LIBRARIES} frnn)
    find_package(TorchScatter QUIET)
    if(NOT TARGET TorchScatter::TorchScatter)
        message(
            WARNING
            "Torch scatter not found, models that rely on torch-scatter will not work"
        )
    else()
        target_link_libraries(
            ActsPluginExaTrkX
            PRIVATE TorchScatter::TorchScatter
        )
        # Should not discard TorchScatter even if its not needed at this point
        # since we need the scatter_max operation in the torch script later
        target_link_options(ActsPluginExaTrkX PUBLIC "-Wl,-no-as-needed")
    endif()
endif()
