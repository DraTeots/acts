acts_add_library(
    ExamplesPrinters
    SHARED
    ActsExamples/Printers/ParticlesPrinter.cpp
    ActsExamples/Printers/TrackParametersPrinter.cpp
)
target_include_directories(
    ActsExamplesPrinters
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(
    ActsExamplesPrinters
    PUBLIC Acts::Core Acts::Fatras Acts::ExamplesFramework
)
