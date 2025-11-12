# QuantumLog Project - README

## Table of Contents

1. [Project Overview](#project-overview)
2. [Project Goals](#project-goals)
3. [Bazel Introduction](#bazel-introduction)
   - [How Bazel Works](#how-bazel-works)
   - [Why MODULE.bazel and BUILD.bazel?](#why-modulebazel-and-buildbazel)
4. [Project Structure](#project-structure)
5. [Conan Dependency Management](#conan-dependency-management)
6. [Bazel BUILD Files](#bazel-build-files)
7. [MODULE.bazel (Bzlmod)](#modulebazel-bzlmod)
8. [Building the Project Locally](#building-the-project-locally)
9. [Docker Integration (Bazel + Conan)](#docker-integration-bazel--conan)
10. [Common Issues & Solutions](#common-issues--solutions)
    - [Errors Encountered and Resolutions](#errors-encountered-and-resolutions)
11. [Diagram: Project Structure & Dependencies](#diagram-project-structure--dependencies)
12. [Optional Improvements](#optional-improvements)

---

## Project Overview

QuantumLog is a C++ logging library leveraging modern C++17 features such as templates for generic sinks, smart pointers for resource management, and polymorphism for extensible logging interfaces. It enables:

* Logging to console (stdout with colored output via spdlog) and file (appends timestamped entries to log.txt, handling concurrency with mutexes).
* Integration of third-party libraries: `fmt` for string formatting (efficient, type-safe interpolation), `spdlog` for high-performance sinks (asynchronous, thread-safe logging).
* Dependency management through Conan, which resolves, fetches, builds, and caches packages hermetically in ~/.conan2/, ensuring no system pollution.
* Reproducible builds via Bazel, which hashes all inputs (sources, deps, toolchains, flags) and sandboxes executions for identical outputs across environments.
* Dockerized environment for isolation and portability, where Conan installs deps and Bazel compiles within a container, supporting cross-platform consistency.

Overall Flow: Developer writes code → Conan resolves/ generates deps wrappers → Bazel parses configs/rules → Builds DAG → Executes hermetic actions → Produces binary → Runs logging demo, outputting to console/file.

This setup demonstrates a shift from traditional CMake-based builds to Bazel for scalability, especially in larger projects where incremental rebuilds and caching save time.

---

## Project Goals

* Modular architecture: Headers in inc/ declare abstract interfaces (e.g., ILogSink with virtual methods); sources in src/ provide concrete implementations (e.g., ConsoleSinkImpl overriding log methods), promoting separation of concerns and testability.
* Automated dependency handling: Conan pins exact versions, resolves transitives (e.g., spdlog requires fmt), and generates Bazel integrations, eliminating manual library downloads or path configurations.
* Build reproducibility: Bazel enforces hermeticity by isolating actions from host env (no leaking PATH/vars), hashing for cache keys, and supporting remote execution/caching for team collaboration.
* Containerization: Dockerfile creates a self-contained Ubuntu 22.04 env, running Conan/Bazel steps in layers, ensuring builds work identically on any machine with Docker.

Integration Details: Conan outputs (.conan/ with repos/flags) feed into Bazel (via MODULE.bazel extensions and BUILD deps), forming a unified build graph where changes propagate minimally.

This achieves enterprise-grade builds: Fast (caching), reliable (hermetic), and extensible (Starlark rules).

---

## Bazel Introduction

Bazel is Google's open-source build and test tool, designed for massive codebases with billions of lines (e.g., Google's monorepo). It excels in polyglot projects (C++, Java, Python) by automating dependency resolution, compilation, and testing with a focus on speed and correctness.

Key Features:
* Incremental: Only rebuilds affected targets.
* Hermetic & Reproducible: Isolates builds, hashes everything for consistency.
* Scalable: Parallel/distributed execution, remote caching.
* Extensible: Starlark (Python-like) for custom rules.

In this project, Bazel replaces CMake for better caching and integration with Conan.

### How Bazel Works

Bazel's execution flow is artifact-driven: Specify outputs (targets like binaries), and it computes minimal steps.

1. **Setup**: Define workspace (root dir with MODULE.bazel). Bazel parses configs (e.g., .bazelrc for flags).
2. **Loading Phase**: Loads MODULE.bazel (modules/extensions) → Fetches external deps (e.g., Conan repos) → Parses BUILD files (rules/targets).
3. **Analysis Phase**: Builds DAG (dependency graph): Targets (e.g., //src:main) depend on others (e.g., :log_lib → @spdlog). Resolves toolchains (compilers/flags from conan_bzl.rc).
4. **Execution Phase**: For each action (compile file, link lib):
   - Hash inputs (sources, deps, flags, toolchain).
   - Check cache (local/remote): Hit → Skip; Miss → Run in sandbox (isolated FS, no host leaks).
   - Parallelize actions; output to bazel-out/bin.
5. **Query/Tools**: Use `bazel query` for graph viz, `aquery` for action details.

Integration with Conan: Conan generators create extensions/repos; Bazel loads them in MODULE → Uses in BUILD deps → Hashes Conan's cache paths for hermeticity.

Flow Example: Change main.cpp → Hash changes → Recompile only main + link → Cache rest.

Vs. CMake: Bazel avoids full regenerates; CMake (via make/ninja) rebuilds more, lacks built-in caching/sandboxes.

### Why MODULE.bazel and BUILD.bazel?

* **MODULE.bazel (Bzlmod)**: Modern replacement for WORKSPACE.bazel (legacy). Defines project as module (name/version), declares external deps (bazel_dep for rules_cc, extensions for Conan). Why needed: Centralizes dep management, versions modules, loads repos hermetically. Without it, Bazel can't resolve @fmt/@spdlog. Flow: Bazel starts here → Evaluates extensions → Creates external repos pointing to Conan's cache.
  
* **BUILD.bazel Files**: Per-package (subdir) configs in Starlark. Define rules (cc_library/binary) and targets (named outputs). Why needed: Tell Bazel what to build (srcs, hdrs, deps), how (rules), and visibility. Without them, no actions/DAG. Flow: Loaded after MODULE → Builds internal graph → Executes rules (e.g., cc_library invokes g++ with flags).

Together: MODULE handles externals; BUILD internals. Enables modularity: Subdirs as packages, deps as labels (//path:target).

---

## Project Structure

```
.
├── .conan                # Conan outputs: Generated repos, RC file (conan_bzl.rc), env scripts
│   ├── BUILD.bazel       # Placeholder
│   ├── conan_bzl.rc      # Flags/configs (e.g., build:conan-config --copt=-std=gnu++17)
│   ├── conan_deps_module_extension.bzl  # Bzlmod ext for repos
│   ├── fmt/              # Wrapper for fmt: BUILD.bazel (cc_library)
│   └── spdlog/           # Wrapper for spdlog
├── inc                   # Headers: Interfaces and declarations
│   ├── BUILD.bazel       # filegroup for headers; visibility:public
│   ├── ConsoleSinkImpl.hpp  # Declares console sink (inherits ILogSink)
│   ├── FileSinkImpl.hpp     # Declares file sink (file open/write)
│   ├── ILogSink.hpp         # Pure virtual interface (log method)
│   ├── LogManger.hpp        # Singleton manager (addSink, log)
│   └── LogMessage.hpp       # Struct for messages (time, level, msg)
├── src                   # Sources: Implementations and entry
│   ├── BUILD.bazel       # cc_library (log_lib), cc_binary (main)
│   ├── ConsoleSinkImpl.cpp  # Impl: std::cout with spdlog::info
│   ├── FileSinkImpl.cpp     # Impl: std::ofstream append
│   ├── LogManger.cpp        # Impl: Vector of sinks, iterate/log
│   ├── LogMessage.cpp       # Impl: fmt::format for string
│   └── main.cpp             # Entry: Create manager, add sinks, log demo
├── MODULE.bazel          # Bzlmod: Module def, dep loads
├── conanfile.py          # Conan: Requires, generators
├── Dockerfile            # Container: Installs tools, Conan, Bazel, builds
├── README.md             # Documentation
└── log.txt               # Runtime output file (appended logs)
```

Flow: Bazel scans subdirs (packages) → Loads BUILDs → Resolves labels (e.g., //inc:log_headers) → Builds from src/ deps.

---

## Conan Dependency Management

Conan is a decentralized C++ package manager supporting binaries for various profiles (OS/compiler/build_type). It resolves graphs, builds from source if no binary matches, caches in ~/.conan2/p/.

### `conanfile.py` Explanation

```python
from conan import ConanFile

class QuantumLogConan(ConanFile):
    name = "quantumlog"
    version = "1.0.0"

    requires = ["spdlog/1.11.0", "fmt/10.0.0"]  # Pins versions; spdlog transitive on fmt

    generators = "BazelToolchain", "BazelDeps"  # Toolchain: RC with flags; Deps: Repos/BUILD wrappers

    settings = "os", "compiler", "build_type", "arch"  # E.g., Linux/gcc11/Release/x86_64

    default_options = {"spdlog/*:header_only": True}  # Spdlog as headers: No lib compile/link

    def layout(self):
        self.folders.source = "."  # Root as source
        self.folders.build = "build"  # Dep outputs

    def build_requirements(self):
        pass  # No tools like cmake (Bazel handles)

    def build(self):
        pass  # Delegate to Bazel
```

Detailed Flow: `conan install -of build --build=missing` → Read profile (gcc11/gnu17/Release) → Resolve graph (fmt binary check: 9 configs, pick cppstd=17) → Cache hit or build → Generators run: BazelDeps (conan_deps_module_extension.bzl, subdir BUILDs with cc_library for deps) → BazelToolchain (conan_bzl.rc with --copt/-define). Outputs in .conan/ for Bazel to load.

Integration: Bazel uses generated repos in MODULE, flags in --bazelrc; ensures deps are hermetic (paths hashed, no system libs).

---

## Bazel BUILD Files

BUILD files are Starlark scripts defining packages. Rules invoke tools (g++), produce artifacts.

### `inc/BUILD.bazel`

```starlark
filegroup(
    name = "log_headers",
    srcs = glob(["*.hpp"]),  # Dynamically includes all headers
    visibility = ["//visibility:public"],
)
```

Flow: No actions; just labels files for deps. Bazel resolves glob at analysis.

### `src/BUILD.bazel`

```starlark
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")  # From rules_cc module

cc_library(
    name = "log_lib",
    srcs = ["ConsoleSinkImpl.cpp", "FileSinkImpl.cpp", "LogManger.cpp", "LogMessage.cpp"],  # Explicit srcs for hashing
    hdrs = ["//inc:log_headers"],  # Cross-package ref
    includes = ["../inc"],  # -I flag for compiler
    deps = ["@fmt//:fmt", "@spdlog//:spdlog"],  # External labels
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [":log_lib"],  # Local label
)
```

Flow: cc_library → Compile each src (g++ with flags) → Archive to lib.a. cc_binary → Compile main → Link lib.a + deps libs. Bazel auto-adds -std=gnu++17 from RC.

---

## MODULE.bazel (Bzlmod)

```starlark
module(name = "quantum_log", version = "1.0.0")  # Unique ID for module

bazel_dep(name = "rules_cc", version = "0.0.9")  # External module: Provides cc_* rules

load_conan_dependencies = use_extension("//.conan:conan_deps_module_extension.bzl", "conan_extension")  # Path to Conan ext
use_repo(load_conan_dependencies, "fmt", "spdlog")  # Registers repos
```

Detailed Flow: Bazel reads MODULE first → Fetches rules_cc (if not cached) → Evaluates extension (uses conan_deps_repo_rules.bzl) → Creates @fmt/@spdlog (symlinks to Conan cache, loads their BUILD.bazel). Enables deps in BUILD files.

Without MODULE: Falls back to legacy WORKSPACE; Bzlmod is preferred for versioning.

---

## Building the Project Locally

Prerequisites: C++ compiler (g++11+), Python3, Conan (pip install conan), Bazel 7+.

1. **Install Bazel** (Ubuntu; for others, see bazel.build/install):

   ```bash
   curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > /usr/share/keyrings/bazel-archive-keyring.gpg
   echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
   sudo apt update && sudo apt install bazel
   sudo apt install g++ unzip zip  # Compiler/tools
   bazel --version  # Verify 7+
   ```

   Flow: Keyring → Repo add → Install binary.

2. **Install Conan Dependencies**:

   ```bash
   conan profile detect  # Auto-setup profile (gcc/Linux/Release)
   conan install . --output-folder=.conan --build=missing  # Use .conan/ for outputs
   ```

   Flow: Profile load → Dep resolve → Generate Bazel files.

3. **Apply Bzlmod Workaround** (If glob errors; see Issues):

   ```bash
   mv .conan/fmt/BUILD.bazel .conan/fmt/fmt.BUILD
   mv .conan/spdlog/BUILD.bazel .conan/spdlog/spdlog.BUILD
   # Edit .conan/conan_deps_module_extension.bzl: Change build_file to "fmt/fmt.BUILD" etc.
   ```

4. **Source Env**:

   ```bash
   . .conan/conanbuild.sh  # Sets PATH/vars for tools/flags
   ```

5. **Build**:

   ```bash
   bazel --bazelrc=.conan/conan_bzl.rc build --config=conan-config //src:main  # Startup: Load RC → Command: Apply config flags → Target build
   ```

   Flow: Startup opts first (--bazelrc) → Command (build) → Options (--config) → Target.

6. **Run**:

   ```bash
   ./bazel-bin/src/main  # Logs to console/log.txt
   ```

7. **Clean/Debug**:

   ```bash
   bazel clean  # Reset outputs
   bazel query //...  # List targets
   bazel aquery //src:main  # Action details
   ```

Flow: Full build: 1-2 mins first time; increments: seconds via cache.

---

## Docker Integration (Bazel + Conan)

Docker layers cache steps, isolates from host. Uses Ubuntu 22.04 for std env.

### Dockerfile Explanation

```dockerfile
FROM ubuntu:22.04  # Base with glibc/std tools

RUN apt-get update && apt-get install -y \  # Layer 1: Essentials
    build-essential git python3-pip curl unzip zip g++ \
    && pip3 install conan \
    && rm -rf /var/lib/apt/lists/*  # Minimize size

RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > /usr/share/keyrings/bazel.gpg && \  # Layer 2: Bazel
    echo "deb [signed-by=/usr/share/keyrings/bazel.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && \
    apt-get update && apt-get install -y bazel

WORKDIR /app
ENV IN_DOCKER=1  # Env var for scripts (e.g., skip ASan in old CMake)

COPY . .  # Layer 3: Code

RUN conan profile detect  # Layer 4: Profile
RUN conan install . -of build --build=missing  # Layer 5: Deps

RUN bazel --bazelrc=.conan/conan_bzl.rc build --config=conan-config //src:main  # Layer 6: Build

CMD ["./bazel-bin/src/main"]  # Runtime entry
```

Flow: Build layers cache (e.g., tools persist) → Runtime: Starts container → Executes CMD.

### Steps

1. Build Image:

   ```bash
   docker build -t quantumlog-bazel:latest .  # Builds layers sequentially
   ```

2. Run Container:

   ```bash
   docker run --rm -it quantumlog-bazel:latest  # --rm cleans; -it interactive
   ```

3. Debug Container:

   ```bash
   docker run --rm -it quantumlog-bazel:latest bash  # Shell access
   ```

Integration: Conan in container uses profile; Bazel builds with same hermeticity, outputs executable runnable in isolated env.

---

## Common Issues & Solutions

| Issue                                 | Cause & Flow                   | Solution & Fix Flow                                      |
| ------------------------------------- | ------------------------------ | -------------------------------------------------------- |
| `invalid glob pattern '../inc/*.hpp'` | Bazel restricts .. in paths; analysis fails. | Create inc/BUILD.bazel with filegroup; ref as //inc:log_headers → Re-parse. |
| `unknown repo 'rules_cc'`             | No bazel_dep; extension load errors. | Add bazel_dep in MODULE.bazel → Bazel fetches module.    |
| `missing input file`                  | File name mismatch/case; DAG resolution fails. | List srcs explicitly, check casing → Rebuild.            |
| Docker build fails                    | Missing pkg in RUN; layer exec errors. | Add to apt install (e.g., gpg for keys) → Rebuild image. |
| Glob error in .conan/ (e.g., include/** not found) | Bzlmod bug: Treats .conan subdirs as local pkgs, wrong paths. | Rename BUILD.bazel to {dep}.BUILD; update extension.bzl build_file → Reload MODULE. |

General Debug Flow: Error → bazel aquery/query → Edit files → bazel build.

### Errors Encountered and Resolutions

During setup, common errors arose from Bazel/Conan integration; here's how resolved for learning:

1. **Unrecognized option: --bazelrc=...**:
   - Cause: --bazelrc (startup opt) placed after build verb; parser treats as command opt → Fails startup.
   - Flow: Command line parse: Startup first → Verb → Command opts.
   - Resolution: Move --bazelrc before build (e.g., bazel --bazelrc=... build ...). Why works: Aligns with Bazel's phases.

2. **Bzlmod Bug: Glob failed / No such package .conan/fmt**:
   - Cause: Conan generates BUILD.bazel in .conan/fmt/; Bazel sees as local package → Wrong path resolution in extension.
   - Flow: MODULE eval → Extension creates repo → But local BUILD interferes → Glob looks in wrong dir.
   - Resolution: Rename to fmt.BUILD; edit conan_deps_module_extension.bzl build_file="fmt/fmt.BUILD". Why: Decouples local from external; repo_rule uses correct BUILD.

3. **Missing conan_toolchain.bzl**:
   - Cause: Conan 2.x uses conan_bzl.rc instead; old docs mismatch.
   - Flow: Generators: BazelToolchain → RC with configs, not .bzl.
   - Resolution: Use --bazelrc=conan_bzl.rc --config=conan-config. Why: RC defines build:conan-config with flags → Activated via --config.

These stem from Bazel's strict parsing/hermeticity; solving teaches flows (parse → analyze → execute). Test incrementally: conan install → bazel query //... → build.

---

## Diagram: Project Structure & Dependencies

```
          ┌───────────┐
          │  MODULE   │ <── Bzlmod: Loads rules_cc + Conan extension → Creates @fmt/@spdlog repos (from .conan/)
          └────┬──────┘
               │
        ┌──────┴─────────┐
        │    src/        │
        │  cc_library    │───┐  srcs compile (g++ -I../inc -std=gnu++17) → lib.a
        │   log_lib      │   │  deps: @spdlog → @fmt (transitive)
        │  cc_binary     │   │
        │    main        │   ▼  link lib.a + deps → binary
        └──────┬─────────┘  ┌─────────┐
               │            │  inc/   │ <── filegroup: Glob *.hpp → Target for visibility
               └───────────▶│ log_headers │
                            └─────────┘
Deps flow: main → log_lib (internal) → @spdlog (headers only, async sinks) → @fmt (formatting utils)
Conan flow: Install → Generate repos/flags → Fed to Bazel DAG
```