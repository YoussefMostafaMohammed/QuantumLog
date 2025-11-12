from conan import ConanFile

class QuantumLogConan(ConanFile):
    name = "quantumlog"
    version = "1.0.0"

    # Declare dependencies
    requires = [
        "spdlog/1.11.0",
        "fmt/10.0.0"
    ]

    # The generators define what build system files Conan produces
    #generators = "CMakeToolchain", "CMakeDeps" # to build using cmake 
    generators = "BazelToolchain", "BazelDeps" # to build using bazel

    # Optional settings to make the package buildable on any platform
    settings = "os", "compiler", "build_type", "arch"

    # Optional: tell Conan what language standard you use
    default_options = {
        "spdlog/*:header_only": True,  # Speeds up build, since spdlog can be header-only
    }

    def layout(self):
        # Use Conan’s default CMake layout (source, build, package dirs)
        self.folders.source = "."
        self.folders.build = "build"

    def build_requirements(self):
        # Add build tools if you need any (optional)
        pass

    def build(self):
        # You can leave this empty if you just use Conan + CMake manually
        pass
