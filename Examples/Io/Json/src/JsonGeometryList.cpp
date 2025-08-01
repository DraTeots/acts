// This file is part of the ACTS project.
//
// Copyright (C) 2016 CERN for the benefit of the ACTS project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "ActsExamples/Io/Json/JsonGeometryList.hpp"

#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <system_error>

void ActsExamples::from_json(const nlohmann::json& data,
                             Acts::GeometryIdentifier& geoId) {
  Acts::GeometryIdentifier::Value null(0u);
  geoId = geoId.withVolume(data.value("volume", null))
              .withBoundary(data.value("boundary", null))
              .withLayer(data.value("layer", null))
              .withApproach(data.value("approach", null))
              .withSensitive(data.value("sensitive", null))
              .withExtra(data.value("extra", null));
}

void ActsExamples::to_json(nlohmann::json& data,
                           const Acts::GeometryIdentifier& geoId) {
  if (geoId.volume() != 0u) {
    data["volume"] = geoId.volume();
  }
  if (geoId.boundary() != 0u) {
    data["boundary"] = geoId.boundary();
  }
  if (geoId.layer() != 0u) {
    data["layer"] = geoId.layer();
  }
  if (geoId.approach() != 0u) {
    data["approach"] = geoId.approach();
  }
  if (geoId.sensitive() != 0u) {
    data["sensitive"] = geoId.sensitive();
  }
  if (geoId.extra() != 0u) {
    data["extra"] = geoId.extra();
  }
}

void ActsExamples::from_json(const nlohmann::json& data,
                             std::vector<Acts::GeometryIdentifier>& geoIdList) {
  for (auto& entry : data) {
    Acts::GeometryIdentifier geoId;
    from_json(entry, geoId);
    geoIdList.push_back(geoId);
  }
}

void ActsExamples::to_json(
    nlohmann::json& data,
    const std::vector<Acts::GeometryIdentifier>& geoIdList) {
  for (auto& geoId : geoIdList) {
    nlohmann::json entry;
    to_json(entry, geoId);
    data.push_back(entry);
  }
}

std::vector<Acts::GeometryIdentifier> ActsExamples::readJsonGeometryList(
    const std::string& path) {
  nlohmann::json data;
  std::vector<Acts::GeometryIdentifier> geoIdList;
  std::ifstream infile(path, std::ifstream::in | std::ifstream::binary);
  if (!infile.good()) {
    throw std::filesystem::filesystem_error(
        path, std::make_error_code(std::errc::no_such_file_or_directory));
  }
  infile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  infile >> data;
  from_json(data, geoIdList);
  return geoIdList;
}

void ActsExamples::writeJsonGeometryList(
    const std::vector<Acts::GeometryIdentifier>& geoIdList,
    const std::string& path) {
  nlohmann::json data;
  to_json(data, geoIdList);
  std::ofstream outfile(path, std::ofstream::out | std::ofstream::binary);
  outfile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  outfile << data;
}
