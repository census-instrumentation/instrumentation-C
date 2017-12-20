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

#include "opencensus/stats/distribution.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "opencensus/stats/bucket_boundaries.h"

namespace opencensus {
namespace stats {

class DistributionTest : public ::testing::Test {
 protected:
  Distribution MakeDistribution(const BucketBoundaries* buckets) {
    return Distribution(buckets);
  }

  void AddToDistribution(Distribution* distribution, double value) {
    distribution->Add(value);
  }
};

TEST_F(DistributionTest, Bucketing) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({0, 10});
  Distribution distribution = MakeDistribution(&buckets);

  EXPECT_EQ(distribution.bucket_counts(), std::vector<uint64_t>({0, 0, 0}));

  AddToDistribution(&distribution, -1);
  EXPECT_EQ(distribution.bucket_counts(), std::vector<uint64_t>({1, 0, 0}));

  AddToDistribution(&distribution, 2);
  EXPECT_EQ(distribution.bucket_counts(), std::vector<uint64_t>({1, 1, 0}));

  AddToDistribution(&distribution, 12);
  EXPECT_EQ(distribution.bucket_counts(), std::vector<uint64_t>({1, 1, 1}));

  // Buckets on boundaries go to the higher bucket
  AddToDistribution(&distribution, 0);
  EXPECT_EQ(distribution.bucket_counts(), std::vector<uint64_t>({1, 2, 1}));

  AddToDistribution(&distribution, 10);
  EXPECT_EQ(distribution.bucket_counts(), std::vector<uint64_t>({1, 2, 2}));
}

TEST_F(DistributionTest, SmallSequence) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({});
  Distribution distribution = MakeDistribution(&buckets);

  AddToDistribution(&distribution, 3);
  AddToDistribution(&distribution, 0);
  AddToDistribution(&distribution, 3);

  EXPECT_EQ(distribution.count(), 3);
  EXPECT_DOUBLE_EQ(distribution.mean(), 2);
  EXPECT_DOUBLE_EQ(distribution.sum_of_squared_deviation(), 6);
  EXPECT_DOUBLE_EQ(distribution.min(), 0);
  EXPECT_DOUBLE_EQ(distribution.max(), 3);
}

TEST_F(DistributionTest, LinearSequence) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({});
  Distribution distribution = MakeDistribution(&buckets);

  const int max = 100;
  const double expected_mean = max / 2.0;
  double expected_sum_of_squared_deviation = 0;
  for (int i = 0; i <= max; ++i) {
    AddToDistribution(&distribution, i);
    expected_sum_of_squared_deviation += pow(i - expected_mean, 2);
  }

  EXPECT_EQ(distribution.count(), max + 1);
  EXPECT_DOUBLE_EQ(distribution.mean(), expected_mean);
  EXPECT_DOUBLE_EQ(distribution.sum_of_squared_deviation(),
                   expected_sum_of_squared_deviation);
  EXPECT_DOUBLE_EQ(distribution.min(), 0);
  EXPECT_DOUBLE_EQ(distribution.max(), max);
}

TEST_F(DistributionTest, ArbitrarySequence) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({});
  Distribution distribution = MakeDistribution(&buckets);

  const std::vector<int> samples{91, 18, 63, 98, 87, 77, 14, 97, 10, 35,
                                 12, 5,  75, 41, 49, 38, 40, 20, 55, 83};
  const double expected_mean =
      static_cast<double>(std::accumulate(samples.begin(), samples.end(), 0)) /
      samples.size();
  double expected_sum_of_squared_deviation = 0;
  for (const auto sample : samples) {
    AddToDistribution(&distribution, sample);
    expected_sum_of_squared_deviation += pow(sample - expected_mean, 2);
  }

  EXPECT_EQ(distribution.count(), samples.size());
  EXPECT_DOUBLE_EQ(distribution.mean(), expected_mean);
  EXPECT_DOUBLE_EQ(distribution.sum_of_squared_deviation(),
                   expected_sum_of_squared_deviation);
  EXPECT_DOUBLE_EQ(distribution.min(),
                   *std::min_element(samples.begin(), samples.end()));
  EXPECT_DOUBLE_EQ(distribution.max(),
                   *std::max_element(samples.begin(), samples.end()));
}

// As with stackdriver, non-finite values corrupt statistics.
TEST_F(DistributionTest, NaN) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({});
  Distribution distribution = MakeDistribution(&buckets);

  AddToDistribution(&distribution, 1);
  AddToDistribution(&distribution, std::numeric_limits<double>::quiet_NaN());

  EXPECT_EQ(distribution.count(), 2);
  EXPECT_TRUE(std::isnan(distribution.mean()));
  EXPECT_TRUE(std::isnan(distribution.sum_of_squared_deviation()));
  EXPECT_TRUE(std::isnan(distribution.min()));
  EXPECT_TRUE(std::isnan(distribution.max()));
}

TEST_F(DistributionTest, PositiveInfinity) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({});
  Distribution distribution = MakeDistribution(&buckets);

  AddToDistribution(&distribution, 1);
  AddToDistribution(&distribution, std::numeric_limits<double>::infinity());

  EXPECT_EQ(distribution.count(), 2);
  EXPECT_FALSE(std::isfinite(distribution.mean()));
  EXPECT_FALSE(std::isfinite(distribution.sum_of_squared_deviation()));
  EXPECT_EQ(distribution.min(), 1);
  EXPECT_FALSE(std::isfinite(distribution.max()));
}

TEST_F(DistributionTest, NegativeInfinity) {
  BucketBoundaries buckets = BucketBoundaries::Explicit({});
  Distribution distribution = MakeDistribution(&buckets);

  AddToDistribution(&distribution, 1);
  AddToDistribution(&distribution, -std::numeric_limits<double>::infinity());

  EXPECT_EQ(distribution.count(), 2);
  EXPECT_FALSE(std::isfinite(distribution.mean()));
  EXPECT_FALSE(std::isfinite(distribution.sum_of_squared_deviation()));
  EXPECT_FALSE(std::isfinite(distribution.min()));
  EXPECT_EQ(distribution.max(), 1);
}

}  // namespace stats
}  // namespace opencensus
