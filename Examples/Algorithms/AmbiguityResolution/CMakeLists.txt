acts_add_library(
    ExamplesAmbiguityResolution
    SHARED
    src/GreedyAmbiguityResolutionAlgorithm.cpp
    src/ScoreBasedAmbiguityResolutionAlgorithm.cpp
)

target_include_directories(
    ActsExamplesAmbiguityResolution
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
    ActsExamplesAmbiguityResolution
    PUBLIC Acts::Core Acts::ExamplesFramework Acts::PluginJson
)
