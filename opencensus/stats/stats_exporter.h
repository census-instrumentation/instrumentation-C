// Copyright 2017, OpenCensus Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPENCENSUS_STATS_STATS_EXPORTER_H_
#define OPENCENSUS_STATS_STATS_EXPORTER_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

#include "absl/strings/string_view.h"
#include "opencensus/stats/view.h"
#include "opencensus/stats/view_data.h"
#include "opencensus/stats/view_descriptor.h"

namespace opencensus {
namespace stats {

// StatsExporter manages views for export, and export handlers.
// StatsExporter is thread-safe.
class StatsExporter final {
 public:
  // Inserts a new view, replacing any existing view with the same name.
  static void AddView(const ViewDescriptor& view);
  // Removes the view with 'name' from the registry, if one is registered.
  static void RemoveView(absl::string_view name);

  // StatsExporter::Handler is the interface for exporters that export recorded
  // data for registered views. The exporter should provide a static Register()
  // method that takes any arguments needed by the exporter (e.g. a URL to
  // export to) and calls StatsExporter::RegisterHandler itself.
  class Handler {
   public:
    virtual ~Handler() = default;
    virtual void ExportViewData(const ViewDescriptor& descriptor,
                                const ViewData& data) = 0;
  };

  // This should only be called by Handler's Register() methods.
  static void RegisterHandler(std::unique_ptr<Handler> handler);

 private:
  friend class StatsExporterTest;

  // Forces immediate export of data.
  static void ExportForTesting();
  static void ClearHandlersForTesting();
};

}  // namespace stats
}  // namespace opencensus

#endif  // OPENCENSUS_STATS_STATS_EXPORTER_H_
