## 1️⃣ Install Conan 2.x

```bash
# Update system
sudo apt update
sudo apt install -y python3-pip git cmake build-essential

# Install Conan 2.x
pip3 install --upgrade conan
```

Check version:

```bash
conan --version
# Should output something like: Conan version 2.x.x
```

---

## Create the build folder

From your project root (`QuantumLog`):

```bash
mkdir -p build
cd build
```

---

## Detect a Conan profile (creates `default` profile)

```bash
conan profile detect
```

You should see something like:

```
Detected settings:
    os=Linux
    arch=x86_64
    compiler=gcc
    compiler.version=11
    build_type=Release
    ...
Profile saved to /home/youssef/.conan2/profiles/default
```

> This step is **mandatory**. Without it, Conan will complain about a missing profile.

---

## Install Conan dependencies

From **project root** (`QuantumLog`) or inside `build` folder:

```bash
conan install . -if build --build=missing
```

Explanation:

* `.` → points to `conanfile.txt` in project root.
* `-if build` → install files into the `build` folder.
* `--build=missing` → build any packages that aren’t available as binaries.

> After this, the folder `build/` will contain: `conan_toolchain.cmake` and all `.cmake` files for dependencies like `spdlog`.

---

## Configure CMake with Conan toolchain

Still in project root:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
```

Key points:

* `-DCMAKE_TOOLCHAIN_FILE` → points to Conan-generated toolchain.
* `-DCMAKE_BUILD_TYPE` → **must** be set (Debug, Release, etc.), otherwise `spdlog` and other Conan dependencies fail to configure.

---

## Build the project

```bash
cmake --build build --parallel $(nproc)
```

* `--parallel $(nproc)` → uses all CPU cores to speed up compilation.

---

## Run your program

```bash
./build/QuantumLog
```

---

## ⚡ Optional: Clean build

If you want a clean build:

```bash
rm -rf build
mkdir build
```

Then repeat steps 4 → 7.

---

## Recap

1. Install Conan 2.x
2. Detect profile: `conan profile detect`
3. Install dependencies: `conan install . -if build --build=missing`
4. Configure CMake: `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug`
5. Build: `cmake --build build --parallel $(nproc)`
6. Run executable: `./build/QuantumLog`
