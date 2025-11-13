from conan import ConanFile # type: ignore
import os

class QuantumLogConan(ConanFile):
    name = "quantumlog"
    version = "1.0.0"

    requires = ["spdlog/1.11.0", "fmt/10.0.0"]

    build_system = os.getenv("BUILD_SYSTEM", "cmake")
    if build_system == "bazel":
        generators = ["BazelToolchain", "BazelDeps"]
    else:
        generators = ["CMakeToolchain", "CMakeDeps"]

    settings = "os", "compiler", "build_type", "arch"

    default_options = {"spdlog/*:header_only": True}

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"

    def build_requirements(self):
        pass

    def build(self):
        pass