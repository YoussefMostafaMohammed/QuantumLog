# QuantumLog – Docker + Conan Setup

This guide explains how to build and run **QuantumLog** using **Docker** and **Conan 2.x** without installing dependencies on your host system.

---

## 1. Prerequisites

Make sure you have the following installed:

* **Docker** (version 20+)
* **Git** (optional, if cloning the repo)

> Conan and build tools are installed inside the Docker container, so nothing else is needed on the host.

---

## 2. Build the Docker Image

1. Open a terminal in the root of the `QuantumLog` project.
2. Run the following command to build the Docker image:

```bash
docker build -t quantumlog:latest .
```

* `-t quantumlog:latest` names the image `quantumlog` with the `latest` tag.
* Docker will install all dependencies and Conan packages inside the image.

---

## 3. Run the Container

After building the image, run the project with:

```bash
docker run --rm quantumlog:latest
```

* `--rm` removes the container after it exits.
* The container will execute the compiled `QuantumLog` binary located in `/app/build`.

---

## 4. How It Works

Inside the Dockerfile:

1. **Base Image:** `ubuntu:22.04` – a lightweight Ubuntu Linux.
2. **Install Dependencies:** `build-essential`, `cmake`, `git`, `python3-pip`.
3. **Install Conan 2.x:** `pip3 install conan`.
4. **Create Build Folder:** `mkdir -p build`.
5. **Detect Conan Profile:** `conan profile detect`.
6. **Install Conan Packages:** `conan install .. -if . --build=missing`

   * `-if .` tells Conan to install packages inside the `build` folder.
7. **Configure with CMake:** `cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake`
8. **Build the Project:** `cmake --build . -- -j$(nproc)`
   Uses all CPU cores for faster compilation.

---

## 5. Adding/Updating Dependencies

If you modify `conanfile.txt`:

```bash
docker build -t quantumlog:latest .
```

* Docker will rebuild the image with updated dependencies.
* If you want to force rebuild dependencies, you can also delete the `build` folder inside the project or add a `--no-cache` flag:

```bash
docker build --no-cache -t quantumlog:latest .
```

---

## 6. Notes

* All build artifacts stay inside the Docker container unless you mount a volume.
* If you want to persist builds outside Docker:

```bash
docker run --rm -v $(pwd)/build:/app/build quantumlog:latest
```

This maps the container’s `/app/build` folder to your host `build` folder.

* The Docker environment ensures consistent builds across different machines.
