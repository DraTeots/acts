// This file is part of the ACTS project.
//
// Copyright (C) 2016 CERN for the benefit of the ACTS project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "Acts/Geometry/BlueprintNode.hpp"
#include "Acts/Geometry/CuboidVolumeBounds.hpp"
#include "Acts/Utilities/ProtoAxis.hpp"

namespace Acts {
class HomogeneousSurfaceMaterial;

namespace Experimental {

namespace detail {
class MaterialDesignatorBlueprintNodeImpl;
}

/// This node type registers material proxies into its child volume during the
/// blueprint construction. It is configured ahead of time which volume faces
/// to mark up, and how do to so.
/// @note This node can only have a single child. This is not an error during
///       tree building, but during geometry construction.
class MaterialDesignatorBlueprintNode final : public BlueprintNode {
 public:
  /// Main constructor for the material designator node.
  /// @param name The name of the node (for debug only)
  explicit MaterialDesignatorBlueprintNode(const std::string& name);

  ~MaterialDesignatorBlueprintNode() override;

  /// @copydoc BlueprintNode::name
  const std::string& name() const override;

  /// @copydoc BlueprintNode::toStream
  void toStream(std::ostream& os) const override;

  /// This method participates in the geometry construction.
  /// It checks that this node only has a single child, is correctly
  /// configured, and forwards the call.
  /// @param options The global blueprint options
  /// @param gctx The geometry context (nominal usually)
  /// @param logger The logger to use
  /// @return The child volume
  Volume& build(const BlueprintOptions& options, const GeometryContext& gctx,
                const Logger& logger = Acts::getDummyLogger()) override;

  /// This method participates in the geometry construction.
  /// It receives the populated portal shell from its only child and attaches
  /// material proxies by consulting the configuration stored in the node.
  /// @note Currently, this node will unconditionally attach
  ///       @ref Acts::ProtoGridSurfaceMaterial
  ///
  /// @param options The global blueprint options
  /// @param gctx The geometry context (nominal usually)
  /// @param logger The logger to use
  /// @return The portal shell with material proxies attached
  PortalShellBase& connect(
      const BlueprintOptions& options, const GeometryContext& gctx,
      const Logger& logger = Acts::getDummyLogger()) override;

  /// This method participates in the geometry construction.
  /// Passes through the call to its only child.
  /// @param options The global blueprint options
  /// @param gctx The geometry context (nominal usually)
  /// @param parent The parent volume
  /// @param logger The logger to use during construction
  void finalize(const BlueprintOptions& options, const GeometryContext& gctx,
                TrackingVolume& parent, const Logger& logger) override;

  /// Configure the designator with a cylinder face and corresponding binning
  /// information.
  /// @note This method can be called multiple times to configure different faces.
  /// @param face The face of the cylinder to configure
  /// @param loc0 The first binning configuration along local axis 0
  /// @param loc1 The first binning configuration along local axis 1
  /// @return The material designator node
  /// @note If this node has previously been configured with a different volume
  ///       shape, this will throw an exception.
  MaterialDesignatorBlueprintNode& configureFace(
      CylinderVolumeBounds::Face face, const DirectedProtoAxis& loc0,
      const DirectedProtoAxis& loc1);

  /// Configure the designator with a cuboid face and corresponding binning
  /// information.
  /// @param face The face of the cylinder to configure
  /// @param material The material to use
  /// @return The material designator node
  /// @note If this node has previously been configured with a different volume
  ///       shape, this will throw an exception.
  MaterialDesignatorBlueprintNode& configureFace(
      CylinderVolumeBounds::Face face,
      std::shared_ptr<const Acts::ISurfaceMaterial> material);

  /// Configure the designator with a cuboid face and corresponding binning
  /// information.
  /// @note This method can be called multiple times to configure different faces.
  /// @param face The face of the cuboid to configure
  /// @param loc0 The first binning configuration along local axis 0
  /// @param loc1 The second binning configuration along local axis 1
  /// @return The material designator node
  /// @note If this node has previously been configured with a different volume
  ///       shape, this will throw an exception.
  MaterialDesignatorBlueprintNode& configureFace(CuboidVolumeBounds::Face face,
                                                 const DirectedProtoAxis& loc0,
                                                 const DirectedProtoAxis& loc1);

  /// Configure the designator with a cuboid face and a homogeneous surface
  /// material.
  /// @param face The face of the cuboid to configure
  /// @param material The material to use
  /// @return The material designator node
  /// @note If this node has previously been configured with a different volume
  ///       shape, this will throw an exception.
  MaterialDesignatorBlueprintNode& configureFace(
      CuboidVolumeBounds::Face face,
      std::shared_ptr<const Acts::ISurfaceMaterial> material);

 private:
  /// @copydoc BlueprintNode::addToGraphviz
  void addToGraphviz(std::ostream& os) const override;

  detail::MaterialDesignatorBlueprintNodeImpl& impl();
  const detail::MaterialDesignatorBlueprintNodeImpl& impl() const;

  std::unique_ptr<detail::MaterialDesignatorBlueprintNodeImpl> m_impl;
};

}  // namespace Experimental
}  // namespace Acts
