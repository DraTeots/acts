acts_add_library(
    ExamplesGenerators
    SHARED
    ActsExamples/Generators/EventGenerator.cpp
)
target_include_directories(
    ActsExamplesGenerators
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(
    ActsExamplesGenerators
    PUBLIC
        Acts::Core
        Acts::ExamplesFramework
        Acts::ExamplesIoHepMC3
        HepMC3::HepMC3
)
