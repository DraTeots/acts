add_unittest(ConvertTrackEDM4hep ConvertTrackEDM4hepTest.cpp)

target_link_libraries(
    ActsUnitTestConvertTrackEDM4hep
    PUBLIC Acts::PluginEDM4hep
)
