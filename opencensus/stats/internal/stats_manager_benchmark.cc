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

#include <memory>

#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "absl/time/time.h"
#include "benchmark/benchmark.h"
#include "opencensus/stats/measure.h"
#include "opencensus/stats/measure_registry.h"
#include "opencensus/stats/recording.h"
#include "opencensus/stats/view.h"

namespace opencensus {
namespace stats {
namespace {

// Generates unique measure names. Since the registry does not support
// unregistering, all measure names must be different across test cases.
// In the present implementation, the number of measures should not affect the
// performance of other measures--in the absence of the ability to reset state,
// that is the best we can do.
std::string MakeUniqueName() {
  static int counter;
  return absl::StrCat("name", counter++);
}

// A set of utility types that allow "templating" a benchmark by
// Aggregation/AggregationWindow.
struct SumAggregation {
  Aggregation operator()(const BucketBoundaries& bucketer) {
    return Aggregation::Sum();
  }
};
struct CountAggregation {
  Aggregation operator()(const BucketBoundaries& bucketer) {
    return Aggregation::Count();
  }
};
struct DistributionAggregation {
  Aggregation operator()(const BucketBoundaries& bucketer) {
    return Aggregation::Distribution(bucketer);
  }
};
struct CumulativeWindow {
  AggregationWindow operator()(absl::Duration interval) {
    return AggregationWindow::Cumulative();
  }
};
struct IntervalWindow {
  AggregationWindow operator()(absl::Duration interval) {
    return AggregationWindow::Interval(interval);
  }
};

// Benchmarks recording against various numbers of views on a single measure
// with a small number of tags.
template <class AggregationFactory, class AggregationWindowFactory>
void BM_Record(benchmark::State& state) {
  const std::string tag_key_1 = "tag_key_1";
  const std::string tag_key_2 = "tag_key_2";
  const std::string measure_name = MakeUniqueName();
  MeasureDouble measure = MeasureRegistry::RegisterDouble(measure_name, "", "");
  std::vector<std::unique_ptr<View>> views;
  for (int i = 0; i < state.range(0); ++i) {
    // The view_key_* tag key is necessary to prevent these from being merged by
    // the StatsManager.
    const ViewDescriptor descriptor =
        ViewDescriptor()
            .set_measure(measure_name)
            .set_name("count")
            .set_aggregation(
                AggregationFactory()(BucketBoundaries::Exponential(10, 10, 2)))
            .set_aggregation_window(AggregationWindowFactory()(absl::Hours(1)))
            .add_column(tag_key_1)
            .add_column(absl::StrCat("view_key_", i));
    views.push_back(absl::make_unique<View>(descriptor));
  }
  std::vector<std::string> tag_values(100);
  for (int i = 0; i < 100; ++i) {
    tag_values[i] = absl::StrCat("value", i);
  }
  int iteration = 0;
  while (state.KeepRunning()) {
    Record({{measure, static_cast<double>(iteration)}},
           {{tag_key_1, tag_values[iteration % tag_values.size()]},
            {tag_key_2, ""}});
    ++iteration;
  }
}
BENCHMARK_TEMPLATE2(BM_Record, SumAggregation, CumulativeWindow)->Range(1, 16);
BENCHMARK_TEMPLATE2(BM_Record, SumAggregation, IntervalWindow)->Range(1, 16);
BENCHMARK_TEMPLATE2(BM_Record, CountAggregation, CumulativeWindow)
    ->Range(1, 16);
BENCHMARK_TEMPLATE2(BM_Record, CountAggregation, IntervalWindow)->Range(1, 16);
BENCHMARK_TEMPLATE2(BM_Record, DistributionAggregation, CumulativeWindow)
    ->Range(1, 16);
BENCHMARK_TEMPLATE2(BM_Record, DistributionAggregation, IntervalWindow)
    ->Range(1, 16);

// TODO: Other useful benchmarks:
//  - Multithreaded recording against one/different measures.
//  - Recording with parameterized numbers of tag keys.

}  // namespace
}  // namespace stats
}  // namespace opencensus
BENCHMARK_MAIN();
