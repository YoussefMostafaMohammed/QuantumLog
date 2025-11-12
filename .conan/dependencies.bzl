# This Bazel module should be loaded by your WORKSPACE file.
# Add these lines to your WORKSPACE one (assuming that you're using the "bazel_layout"):
# load("@//conan:dependencies.bzl", "load_conan_dependencies")
# load_conan_dependencies()

def load_conan_dependencies():
    native.new_local_repository(
        name="spdlog",
        path="/home/youssef/.conan2/p/spdloc328dfc1eb52d/p",
        build_file="/home/youssef/modernC++/QuantumLog/.conan/spdlog/BUILD.bazel",
    )
    native.new_local_repository(
        name="fmt",
        path="/home/youssef/.conan2/p/fmt1cd708a4e8cfe/p",
        build_file="/home/youssef/modernC++/QuantumLog/.conan/fmt/BUILD.bazel",
    )
