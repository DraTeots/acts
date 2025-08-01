acts_add_library(
    ExamplesDetectorDD4hep
    SHARED
    src/DD4hepDetector.cpp
    src/AlignedDD4hepDetectorElement.cpp
)

target_include_directories(
    ActsExamplesDetectorDD4hep
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(
    ActsExamplesDetectorDD4hep
    PUBLIC
        Acts::Core
        Acts::PluginDD4hep
        Acts::ExamplesFramework
        Acts::ExamplesDetectorsCommon
)

if(ACTS_BUILD_EXAMPLES_GEANT4)
    if(${DD4hep_VERSION} VERSION_LESS 1.11)
        target_link_libraries(
            ActsExamplesDetectorDD4hep
            PUBLIC Acts::ExamplesGeant4 ${DD4hep_DDG4_LIBRARY}
        )
    else()
        target_link_libraries(
            ActsExamplesDetectorDD4hep
            PUBLIC Acts::ExamplesGeant4 DD4hep::DDG4
        )
    endif()

    target_sources(
        ActsExamplesDetectorDD4hep
        PUBLIC src/DDG4DetectorConstruction.cpp src/DD4hepDetectorGeant4.cpp
    )
else()
    target_sources(
        ActsExamplesDetectorDD4hep
        PUBLIC src/DD4hepDetectorGeant4Stub.cpp
    )
endif()

if(${DD4hep_VERSION} VERSION_LESS 1.11)
    target_include_directories(
        ActsExamplesDetectorDD4hep
        PUBLIC ${DD4hep_INCLUDE_DIRS}
    )
    target_link_libraries(
        ActsExamplesDetectorDD4hep
        PUBLIC ${DD4hep_DDCORE_LIBRARY}
    )
else()
    target_link_libraries(ActsExamplesDetectorDD4hep PUBLIC DD4hep::DDCore)
endif()

# not sure why this needs to be set, but dd4hep fails to compile otherwise
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(
    ActsExamplesDetectorDD4hep
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
dd4hep_set_version(ActsExamplesDetectorDD4hep MAJOR 1 MINOR 0 PATCH 0)
dd4hep_generate_rootmap(ActsExamplesDetectorDD4hep)
