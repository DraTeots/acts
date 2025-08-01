# plugins are manually ordered such that dependent plugins appear after their
# dependencies in the component list.
# independent plugins
add_component_if(ActSVG PluginActSVG ACTS_BUILD_PLUGIN_ACTSVG)
add_component_if(FastJet PluginFastJet ACTS_BUILD_PLUGIN_FASTJET)
add_component_if(FpeMonitoring PluginFpeMonitoring ACTS_BUILD_PLUGIN_FPEMON)
add_component_if(Geant4 PluginGeant4 ACTS_BUILD_PLUGIN_GEANT4)
add_component_if(GeoModel PluginGeoModel ACTS_BUILD_PLUGIN_GEOMODEL)
add_component_if(Json PluginJson ACTS_BUILD_PLUGIN_JSON)
add_component_if(Onnx PluginOnnx ACTS_BUILD_PLUGIN_ONNX)
add_component_if(ExaTrkX PluginExaTrkX ACTS_BUILD_PLUGIN_EXATRKX)
add_component_if(Detray PluginDetray ACTS_BUILD_PLUGIN_TRACCC)
add_component_if(Covfie PluginCovfie ACTS_BUILD_PLUGIN_TRACCC)
add_component_if(Hashing PluginHashing ACTS_BUILD_PLUGIN_HASHING)

# dependent plugins. depend either on a independent plugins or on one another
add_component_if(Root PluginRoot ACTS_BUILD_PLUGIN_ROOT)
add_component_if(DD4hep PluginDD4hep ACTS_BUILD_PLUGIN_DD4HEP)
add_component_if(
    Podio
    PluginPodio
    ACTS_BUILD_PLUGIN_PODIO
    OR
    ACTS_PLUGIN_EDM4HEP
)
add_component_if(EDM4hep PluginEDM4hep ACTS_BUILD_PLUGIN_EDM4HEP)

propagate_components_to_parent()
