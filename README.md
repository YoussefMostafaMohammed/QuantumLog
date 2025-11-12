# QuantumLog: A Telemetry & Logging System

QuantumLog is a scalable, multithreaded telemetry and logging system written in modern C++17. It can be use **Docker** as a container which uses **CMake** or **bazel** to build and uses **conan** to provide the needed libraries and supports **continuous integration** through **Jenkins**. Designed with best practices in both architecture and implementation, QuantumLog leverages the power of modern C++ along with industry-standard libraries like **vSOMEIP** and **DLT** to deliver robust, high-performance logging and tracing capabilities.

---

## Features
- Supports multiple data sources: files, sockets, or Ethernet/Wi-Fi via vSOMEIP.
- Provides intuitive data formatting.
- Can display or store logs to multiple sinks: console, file, or AUTOSAR-compliant DLT Viewer.
- Thread-safe architecture designed for scalability and real-time telemetry.
- use Docker as container.
- use conan to provide the needed c++ libs.
- docker uses bazel or cmake to build the project.
- Continuous integration using Jenkins.

---

## Topics

### Modern C++
- Dynamic Dispatch (Virtual Functions + Virtual Destructors)
- Casting (Static & Dynamic + RTTI)
- Copy Semantics & References
- Move Semantics & Ownership Transfer
- Threading, Concurrency, & Functors
- Smart Pointers
- Special Member Functions & Rules of 0/3/5
- Deep Dive into Virtual Tables & Name Mangling (Itanium ABI)
- Templates & Generic Programming

---

### Libraries
- magic_enum  
- vsomeip  
- dlt  

---

### Design Patterns
- Strategy  
- Factory  
- Builder  
- Observer  
- Producer-Consumer  
- RAII  
- Bridge  
- Singleton  
- Composite  
- Dependency Injection  
- Decorator  
- Adapter  
- Facade  
