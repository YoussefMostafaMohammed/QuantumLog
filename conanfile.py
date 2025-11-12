from conan import ConanFile
import os

class QuantumLogConan(ConanFile):
    name = "quantumlog"
    version = "1.0.0"

    # Declare dependencies
    requires = [
        "spdlog/1.11.0",
        "fmt/10.0.0"
    ]
    use_bazel = os.getenv("USE_BAZEL", "true").lower() == "true"
    if use_bazel:
        generators = ["BazelToolchain", "BazelDeps"]
    else:
        generators = ["CMakeToolchain", "CMakeDeps"]
    # Optional settings
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "spdlog/*:header_only": True,
    }

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"

    def build_requirements(self):
        pass

    def build(self):
        pass
