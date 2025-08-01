acts_add_library(
    ExamplesTrackFinding
    SHARED
    src/SeedingAlgorithm.cpp
    src/SeedingOrthogonalAlgorithm.cpp
    src/SpacePointMaker.cpp
    src/TrackFindingAlgorithm.cpp
    src/TrackFindingAlgorithmFunction.cpp
    src/HoughTransformSeeder.cpp
    src/TrackParamsEstimationAlgorithm.cpp
    src/MuonHoughSeeder.cpp
    src/GbtsSeedingAlgorithm.cpp
    src/TrackParamsLookupEstimation.cpp
    src/GridTripletSeedingAlgorithm.cpp
)

target_include_directories(
    ActsExamplesTrackFinding
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
    ActsExamplesTrackFinding
    PUBLIC Acts::Core Acts::ExamplesFramework Acts::ExamplesMagneticField
    PRIVATE ROOT::Core ROOT::Geom ROOT::Graf ROOT::Hist ROOT::Gpad
)

acts_compile_headers(ExamplesTrackFinding GLOB "include/**/*.hpp")

# If Hashing examples are enabled, add them to the build
if(ACTS_BUILD_EXAMPLES_HASHING)
    target_sources(
        ActsExamplesTrackFinding
        PRIVATE src/SeedingAlgorithmHashing.cpp
    )

    target_link_libraries(ActsExamplesTrackFinding PUBLIC Acts::PluginHashing)
endif()
