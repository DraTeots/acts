acts_add_library(
    ExamplesDetectorGeant4
    SHARED
    src/Geant4Detector.cpp
    src/GdmlDetectorConstruction.cpp
    src/GdmlDetector.cpp
)

target_include_directories(
    ActsExamplesDetectorGeant4
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
    ActsExamplesDetectorGeant4
    PUBLIC
        Acts::Core
        Acts::ExamplesFramework
        Acts::ExamplesGeant4
        Acts::PluginGeant4
        Acts::ExamplesDetectorsCommon
)
