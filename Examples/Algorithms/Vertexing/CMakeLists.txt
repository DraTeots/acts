acts_add_library(
    ExamplesVertexing
    SHARED
    src/AdaptiveMultiVertexFinderAlgorithm.cpp
    src/HoughVertexFinderAlgorithm.cpp
    src/IterativeVertexFinderAlgorithm.cpp
    src/VertexFitterAlgorithm.cpp
    src/TruthVertexSeeder.cpp
)
target_include_directories(
    ActsExamplesVertexing
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(
    ActsExamplesVertexing
    PUBLIC Acts::Core Acts::ExamplesFramework
    PRIVATE Acts::ExamplesTruthTracking
)
