load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "hello-lib",
    srcs = glob(["lib/*.cpp"]),
    hdrs = glob(["lib/*.h"]),
    deps = ["@fmt"],
)

cc_binary(
    name = "hello-world",
    srcs = ["main/hello-world.cpp"],
    deps = [":hello-lib"],
)
