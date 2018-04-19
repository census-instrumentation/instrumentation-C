# Copyright 2017, OpenCensus Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

workspace(name = "io_opencensus_cpp")

# We depend on Abseil.
http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-master",
    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
)

# CCTZ (Time-zone framework). Used by absl.
http_archive(
    name = "com_googlesource_code_cctz",
    strip_prefix = "cctz-master",
    urls = ["https://github.com/google/cctz/archive/master.zip"],
)

# GoogleTest/GoogleMock framework.
http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-master",
    urls = ["https://github.com/google/googletest/archive/master.zip"],
)

# gRPC
http_archive(
    name = "com_github_grpc_grpc",
    urls = ["https://github.com/grpc/grpc/archive/master.tar.gz"],
    strip_prefix = "grpc-master"
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

# These bind()s are needed for the cc_grpc_library() rule to work.
bind(
    name = "grpc++_codegen_proto",
    actual = "@com_github_grpc_grpc//:grpc++_codegen_proto",
)

bind(
    name = "grpc_cpp_plugin",
    actual = "@com_github_grpc_grpc//:grpc_cpp_plugin",
)

# Google Benchmark library.
# Adapted from cctz's WORKSPACE.
# Upstream support for bazel is tracked in
#  - https://github.com/google/benchmark/pull/329
#  - https://github.com/google/benchmark/issues/191
new_http_archive(
    name = "com_google_benchmark",
    urls = ["https://github.com/google/benchmark/archive/master.zip"],
    strip_prefix = "benchmark-master",
    build_file_content =
"""
config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
    visibility = [ "//visibility:private" ],
)

cc_library(
    name = "benchmark",
    srcs = glob([
        "src/*.h",
        "src/*.cc",
    ]),
    hdrs = glob(["include/benchmark/*.h"]),
    copts = select({
        "//:windows": ["-DHAVE_STD_REGEX"],
        # HAVE_STD_REGEX didn't work.
        "//conditions:default": ["-DHAVE_POSIX_REGEX"],
    }),
    includes = ["include"],
    visibility = ["//visibility:public"],
)
"""
)

# TODO: Remove protobuf dependency and reset to head.
http_archive(
    name = "com_github_jupp0r_prometheus_cpp",
    strip_prefix = "prometheus-cpp-a97127c9aeec8decb76cf3bec1fb9c9a0dd4a9fc",
    urls = ["https://github.com/jupp0r/prometheus-cpp/archive/a97127c9aeec8decb76cf3bec1fb9c9a0dd4a9fc.zip"],
)

load("@com_github_jupp0r_prometheus_cpp//:repositories.bzl",
     "load_prometheus_client_model",
     "load_civetweb")

# Load dependencies individually since we load some of them above.
load_prometheus_client_model()
load_civetweb()

# Curl library - used by zipkin exporter.
new_http_archive(
    name = "com_github_curl",
    urls = ["https://github.com/curl/curl/archive/master.zip"],
    strip_prefix = "curl-master",
    build_file_content =
"""
load("@io_opencensus_cpp//opencensus:curl.bzl", "CURL_COPTS")
package(features = ['no_copts_tokenization'])

config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
    visibility = [ "//visibility:private" ],
)

config_setting(
    name = "osx",
    values = {"cpu": "darwin"},
    visibility = [ "//visibility:private" ],
)

cc_library(
    name = "curl",
    srcs = glob([
        "lib/**/*.c",
    ]),
    hdrs = glob([
        "include/curl/*.h",
        "lib/**/*.h",
    ]),
    includes = ["include/", "lib/"],
    copts = CURL_COPTS + [
        "-DOS=\\"os\\"",
        "-DCURL_EXTERN_SYMBOL=__attribute__((__visibility__(\\"default\\")))",
    ],
    visibility = ["//visibility:public"],
)
"""
)

# Rapidjson library - used by zipkin exporter.
new_http_archive(
    name = "com_github_rapidjson",
    urls = ["https://github.com/Tencent/rapidjson/archive/master.zip"],
    strip_prefix = "rapidjson-master",
    build_file_content =
"""
cc_library(
    name = "rapidjson",
    srcs = [],
    hdrs = glob([
        "include/rapidjson/*.h",
        "include/rapidjson/internal/*.h",
        "include/rapidjson/error/*.h",
    ]),
    includes = ["include/"],
    defines = ["RAPIDJSON_HAS_STDSTRING=1",],
    visibility = ["//visibility:public"],
)
"""
)
