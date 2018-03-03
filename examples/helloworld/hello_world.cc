// Copyright 2018, OpenCensus Authors
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

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "absl/strings/string_view.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "opencensus/exporters/stats/stdout/stdout_exporter.h"
#include "opencensus/exporters/trace/stdout/stdout_exporter.h"
#include "opencensus/stats/stats.h"
#include "opencensus/trace/sampler.h"
#include "opencensus/trace/span.h"

ABSL_CONST_INIT const absl::string_view kFrontendKey = "my.org/keys/frontend";
ABSL_CONST_INIT const absl::string_view kVideoSizeViewName =
    "my.org/views/video_size";
ABSL_CONST_INIT const absl::string_view kVideoSizeMeasureName =
    "my.org/measure/video_size";

opencensus::stats::MeasureInt VideoSizeMeasure();

// The resource owner defines and registers a measure. A function exposing a
// function-local static is the recommended style, ensuring that the measure is
// only registered once.
opencensus::stats::MeasureInt VideoSizeMeasure() {
  static const opencensus::stats::MeasureInt video_size =
      opencensus::stats::MeasureRegistry::RegisterInt(
          kVideoSizeMeasureName, "MBy", "size of processed videos");
  return video_size;
}

// Simple program that collects data for video size.
int main(int argc, char **argv) {
  if (argc != 1) {
    std::cerr << "Usage: " << argv[0] << "\n";
    return 1;
  }
  srand(time(NULL));

  // Register stdout exporters.
  opencensus::exporters::stats::StdoutExporter::Register();
  opencensus::exporters::trace::StdoutExporter::Register();

  // Call measure so that it is initialized.
  VideoSizeMeasure();

  // Create view to see the processed video size distribution broken down by
  // frontend. The view has bucket boundaries (0, 256, 65536) that will group
  // measure values into histogram buckets.
  const opencensus::stats::ViewDescriptor video_size_view =
      opencensus::stats::ViewDescriptor()
          .set_name(kVideoSizeViewName)
          .set_description("processed video size over time")
          .set_measure(kVideoSizeMeasureName)
          .set_aggregation(opencensus::stats::Aggregation::Distribution(
              opencensus::stats::BucketBoundaries::Exponential(8, 1 << 8, 2)))
          .add_column(kFrontendKey);
  opencensus::stats::View view(video_size_view);
  video_size_view.RegisterForExport();

  opencensus::trace::AlwaysSampler sampler;
  auto span = opencensus::trace::Span::StartSpan("my.org/ProcessVideo", nullptr,
                                                 {&sampler});

  // Process video.
  // Record the processed video size.
  span.AddAnnotation("Start processing video.");
  // Sleep for [1,10] milliseconds to fake work.
  absl::SleepFor(absl::Milliseconds(rand() % 10 + 1));
  opencensus::stats::Record({{VideoSizeMeasure(), 25648}},
                            {{kFrontendKey, "video size"}});
  span.AddAnnotation("Finished processing video.");
  span.End();

  // Sleep for ~5 seconds to ensure that exporters will process the span.
  std::cout << "Wait longer than the reporting duration...\n";
  absl::SleepFor(absl::Milliseconds(5100));

  // Sleep while exporters run in the background.
  std::cout << "Views:\n" << video_size_view.DebugString() << "\n";
  const auto data = view.GetData();
  if (data.type() == opencensus::stats::ViewData::Type::kDistribution) {
    for (auto &it : data.distribution_data()) {
      for (auto &name : it.first) std::cout << name << " : ";
      std::cout << it.second.DebugString() << "\n";
    }
  }
  std::cout << "Client sleeping, ^C to exit.\n";
  while (true) {
    absl::SleepFor(absl::Seconds(10));
  }
}
