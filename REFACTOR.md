# Architecture Refactoring Plan: CMake, CTest, and GoogleTest Transition

## 1. Executive Summary & Vision

### 1.1 Purpose
This document provides the definitive architectural specification and step-by-step migration blueprint for refactoring `low-level-interview-prep` from its initial, Makefile-and-Acutest structure into a scalable, zero-configuration framework powered by **CMake**, **CTest**, and **GoogleTest (GTest)** inside a hermetic **Dev Container**.

### 1.2 Core Objectives
* **Zero-Configuration Scalability:** Support scaling to 50+ isolated exercises without manual registry edits or maintaining path configurations in `.vscode/settings.json`.
* **Industry Standard Alignment:** Align candidate practice directly with modern C/C++ toolchains, testing paradigms, and memory sanitizers utilized across semiconductor, systems, and embedded firmware companies.
* **Hermetic & Cross-OS Consistency:** Guarantee identical build results, pathing, and line endings across macOS (Apple Silicon & Intel), Windows (WSL 2 & Docker Desktop), Linux, and GitHub Codespaces.
* **Modular Dependency Support:** Support higher-level exercises that rely on pre-implemented foundational structures by providing reference libraries in `shared/provided/`.
* **Atomic & Risk-Free Migration:** Execute the refactoring in 5 testable, git-commit-able phases to ensure continuous repository integrity.

---

## 2. Target System Architecture

The refactored architecture isolates environment, build, testing, and exercise concerns into five decoupled layers:

```text
┌────────────────────────────────────────────────────────────────────────┐
│                   VS Code / Test Explorer / Codespaces                 │
└──────────────────────────────────┬─────────────────────────────────────┘
                                   │
┌──────────────────────────────────▼─────────────────────────────────────┐
│ 1. Hermetic Environment Layer (.devcontainer / Ubuntu 24.04 Toolchain) │
│    • GCC 13, CMake, Ninja, CTest pre-installed                         │
│    • Workspace settings enforce clean "Problems-Only" view             │
└──────────────────────────────────┬─────────────────────────────────────┘
                                   │
┌──────────────────────────────────▼─────────────────────────────────────┐
│ 2. Meta-Build & Auto-Discovery Layer (Root CMakeLists.txt)             │
│    • Fetches GoogleTest (v1.14.0) via FetchContent                     │
│    • Dynamic globbing auto-registers any problem with CMakeLists.txt   │
└──────────────────────────────────┬─────────────────────────────────────┘
                                   │
┌──────────────────────────────────┼─────────────────────────────────────┐
│ 3. Exercise & Dependency Layer   │ 4. Testing & Framework Layer        │
│    • problems/<name>/            │    • tests/framework/               │
│      ├── CMakeLists.txt          │      └── (Self-tests for repo)      │
│      ├── <name>.h                │    • CTest Labeling:                │
│      ├── <name>.c                │      ├── -L problem                 │
│      └── test_runner.cpp         │      └── -L framework               │
│    • shared/provided/<module>/   │    • Optional Memory Sanitizers     │
│      └── Reference solution libs │      (ASan / UBSan)                 │
└──────────────────────────────────┴─────────────────────────────────────┘
                                   │
┌──────────────────────────────────▼─────────────────────────────────────┐
│ 5. Automation & Scaffolding Engine (scripts/add_problem.py)            │
│    • Single CLI command to generate complete problem directory         │
└────────────────────────────────────────────────────────────────────────┘
```

---

## 3. Detailed Component Specifications

### 3.1 Hermetic Environment (`.devcontainer/devcontainer.json` & `.gitattributes`)
* **Base Image:** `mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04` (provides multi-arch support for `linux/amd64` and `linux/arm64`).
* **Toolchain:** GCC 13, GDB, CMake, Ninja, CTest, Python 3.
* **Cross-OS Line-Ending Protection:** A root `.gitattributes` file enforces `* text eol=lf` to prevent Windows `CRLF` execution errors inside Linux containers.
* **GitHub Codespaces Integration:** Zero-install web access works out of the box by consuming `.devcontainer/devcontainer.json`.

### 3.2 Root Build Engine (`CMakeLists.txt`)
* **Language Standards:** C99 for user source code (`CMAKE_C_STANDARD 99`), C++17 for GoogleTest runners (`CMAKE_CXX_STANDARD 17`).
* **Dependency Management:** GoogleTest is fetched automatically at configure time using `FetchContent`. No host GTest installation is required.
* **Auto-Discovery:** Root CMake scans `problems/*` dynamically. Adding a new problem folder with a valid local build manifest automatically exposes it to CTest and VS Code on reload.

### 3.3 Exercise Specification & Directory Layout
Each problem directory under `problems/<name>/` is self-contained and standardized:
* `<name>.h`: Public API contract implemented by the user.
* `<name>.c`: Pure C solution file containing candidate logic.
* `test_runner.cpp`: C++ GoogleTest wrapper including `<name>.h` inside an `extern "C"` block.
* `CMakeLists.txt`: Standardized 3-to-5 line CMake build manifest linking `gtest_main` and registering CTest discovery.

### 3.4 Shared Reference Library Strategy (`shared/provided/`)
For advanced exercises requiring prerequisite data structures or support utilities (e.g., a producer/consumer problem requiring a pre-built ring buffer):
* Reference implementations live inside `shared/provided/<module_name>/`.
* The candidate's exercise `CMakeLists.txt` includes the reference header path and compiles the provided reference `.c` file alongside the candidate's solution.
* Candidates consume reference data structures as opaque API contracts without re-implementing or debugging foundational primitives.

### 3.5 Memory Safety & Sanitizers Support
The root build engine provides an optional sanitizer flag for catching silent low-level memory corruptions (buffer overflows, double frees, invalid bit-shifts, use-after-free):
* CMake flag: `-DENABLE_SANITIZERS=ON`.
* Enables AddressSanitizer (`-fsanitize=address`) and UndefinedBehaviorSanitizer (`-fsanitize=undefined`).

### 3.6 Testing Engine & Categorization
Testing is strictly partitioned using CTest labels to avoid confusing candidate problem tests with framework maintainer tests:
* **Problem Unit Tests (`-L problem`):** Tests defined under `problems/<name>/test_runner.cpp`. Practicers run these through the Test Explorer or via `ctest -L problem`.
* **Framework Self-Tests (`-L framework`):** Infrastructure tests stored in a dedicated `tests/framework/` directory (e.g., verifying `scripts/add_problem.py` output or reference libraries in `shared/provided/`). Maintainers execute these via `ctest -L framework`.
* **CI Readiness:** This label structure makes future CI integration (e.g., GitHub Actions) a zero-refactor toggle (`ctest --output-on-failure`).

### 3.7 Scaffolding Engine (`scripts/add_problem.py`)
A workspace CLI script automates exercise creation. Executing `python3 scripts/add_problem.py <problem_name>` instantly creates:
1. `problems/<problem_name>/CMakeLists.txt`
2. `problems/<problem_name>/<problem_name>.h`
3. `problems/<problem_name>/<problem_name>.c`
4. `problems/<problem_name>/test_runner.cpp`

### 3.8 Developer User Experience (UX)
To present a clean, "problems-focused" UI, workspace settings in `.vscode/settings.json` exclude non-essential boilerplate (`build/`, `scripts/`, internal CMake manifests) from the File Explorer view while leaving them active in the background. Practicers navigate exercises primarily via VS Code's **Test Explorer** panel.

---

## 4. Testable Step-by-Step Migration Plan

This plan is structured into **5 sequential, atomic phases**. Each phase must pass its specific **Verification Gate** and result in a **Git Commit** before proceeding to the next phase.

```text
  Phase 1: Environment & Repository Baseline Setup
    │
    ▼ [Gate: Dev Container builds & git line endings verified] -> COMMIT
  Phase 2: Root CMake & GoogleTest Fetch Configuration
    │
    ▼ [Gate: CMake fetches GTest & builds dummy target] -> COMMIT
  Phase 3: Incremental Problem-by-Problem Migration
    │
    ├── Migrate problem 1 -> [Gate: CTest -L problem passes] -> COMMIT
    ├── Migrate problem 2 -> [Gate: CTest -L problem passes] -> COMMIT
    └── ... (Repeat per exercise)
    │
    ▼
  Phase 4: Shared Reference Modules & Scaffolding Script
    │
    ▼ [Gate: Scaffolding script generates working exercise] -> COMMIT
  Phase 5: Framework Self-Tests & Legacy Cleanup
    │
    ▼ [Gate: Global CTest pass & zero legacy Acutest/Make files remaining] -> COMMIT
```

---

### Phase 1: Environment & Repository Baseline Setup
**Goal:** Configure line-ending protections, workspace settings, and Dev Container extensions.

1. **Create `.gitattributes`:** Add `* text eol=lf` at repository root.
2. **Update `.devcontainer/devcontainer.json`:**
   * Ensure standard C++ extension pack and CMake Tools (`ms-vscode.cmake-tools`, `twxs.cmake`, `ms-vscode.cpptools`) are listed.
   * Verify base image is set to `mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04`.
3. **Configure Workspace Exclusions:** Update `.vscode/settings.json` to exclude build artifacts and system directories from the file explorer.

* **Verification Gate:** Rebuild the container in VS Code. Verify GCC 13, CMake, and Python 3 are present in the terminal.
* **Git Action:** `git commit -m "refactor(env): configure devcontainer, line endings, and workspace settings"`

---

### Phase 2: Root CMake Engine & GoogleTest Integration
**Goal:** Establish the root build system and enable GoogleTest downloading.

1. **Create Root `CMakeLists.txt`:**
   * Set minimum CMake version to 3.22.
   * Define project `low_level_interview_prep`.
   * Configure C99 and C++17 standards.
   * Integrate GoogleTest (v1.14.0) via `FetchContent`.
   * Include `enable_testing()` and `include(GoogleTest)`.
   * Add dynamic directory globbing for subdirectories under `problems/*/` containing `CMakeLists.txt`.
2. **Add Memory Sanitizer Option:** Add CMake flag `-DENABLE_SANITIZERS=OFF` (toggleable to `ON`).

* **Verification Gate:** Run `cmake -B build -G Ninja` in terminal. Confirm GoogleTest downloads, configures cleanly, and creates `build/`.
* **Git Action:** `git commit -m "refactor(build): implement root CMakeLists.txt with GTest FetchContent"`

---

### Phase 3: Incremental Problem-by-Problem Migration
**Goal:** Migrate existing legacy exercises from Acutest/Make to CMake/GTest **one exercise at a time**.

**Target Legacy Problems:** `bit_manip`, `check_power_of_2`, `memcpy`, `memmove`, `swap_2_nums`, `self_check`, `endianess_detection`.

**Per-Problem Migration Protocol:**
For each problem directory `problems/<name>/`:
1. Create `problems/<name>/CMakeLists.txt` referencing standard executable target `prob_<name>`.
2. Refactor `test_runner.c` to `test_runner.cpp`:
   * Replace `#include "acutest.h"` with `#include <gtest/gtest.h>`.
   * Wrap `#include "<name>.h"` in `extern "C" { ... }`.
   * Translate `TEST_CHECK` / `TEST_LIST` definitions into GoogleTest `TEST(SuiteName, TestName)` blocks.
3. Apply CTest label `problem` to the discoverable test target.
4. Remove the legacy problem-local `Makefile`.
5. Execute test verification.

* **Verification Gate (Per Problem):** Run `ctest --test-dir build -R prob_<name>` and verify compilation and test runner execution.
* **Git Action (Per Problem):** `git commit -m "refactor(problems): migrate <name> to CMake and GoogleTest"`

---

### Phase 4: Shared Reference Modules & Automation Scaffolding
**Goal:** Establish directory architecture for pre-compiled reference libraries and scaffolding tooling.

1. **Establish `shared/provided/` Directory:**
   * Create `shared/provided/` directory structure with README documentation detailing how future support libraries/modules should be placed and consumed.
2. **Implement Scaffolding Script (`scripts/add_problem.py`):**
   * Write Python script that accepts a problem name as an argument.
   * Script creates `problems/<name>/` with standardized `CMakeLists.txt`, `<name>.h`, `<name>.c`, and `test_runner.cpp`.
   * Make script executable (`chmod +x scripts/add_problem.py`).

* **Verification Gate:** Run `python3 scripts/add_problem.py dummy_test`. Run `cmake -B build`. Verify `prob_dummy_test` is registered in CTest. Delete `problems/dummy_test/` after verification.
* **Git Action:** `git commit -m "feat(scaffolding): add problem creation CLI script and shared provided structure"`

---

### Phase 5: Framework Self-Tests & Legacy Cleanup
**Goal:** Create dedicated framework self-test layout and remove legacy Acutest artifacts.

1. **Establish Framework Self-Test Directory (`tests/framework/`):**
   * Create `tests/framework/CMakeLists.txt` registered under CTest label `framework`.
   * Migrate framework sanity check from `problems/self_check` into `tests/framework/test_framework_integrity.cpp`.
2. **Remove Legacy Plumbing:**
   * Delete `shared/acutest.h`.
   * Delete `private/extension_modifications/` and obsolete Acutest notes.
   * Remove old task scripts or references to Acutest test adapters in `.vscode/tasks.json`.

* **Global Verification Gate:** Run `ctest --test-dir build --output-on-failure`. Verify all registered `problem` and `framework` tests execute properly with zero build errors or missing file errors.
* **Git Action:** `git commit -m "refactor(cleanup): establish framework self-tests and remove legacy Acutest artifacts"`

---

## 5. Summary of Deliverables & Source Map

| Concern | Owning Path / Tool | Status After Refactor |
|---|---|---|
| Development Container | `.devcontainer/devcontainer.json` | Active (Ubuntu 24.04, GCC 13, CMake) |
| Line Ending Enforcement | `.gitattributes` | Active (`* text eol=lf`) |
| Root Build System | `CMakeLists.txt` | Active (Auto-discovery, GTest FetchContent) |
| Exercise Manifests | `problems/<name>/CMakeLists.txt` | Active (3-5 lines per problem) |
| User Code | `problems/<name>/<name>.c`, `.h` | Active (Pure C99) |
| Test Code | `problems/<name>/test_runner.cpp` | Active (C++17 GoogleTest) |
| Shared Reference Libraries | `shared/provided/<module>/` | Active (Pre-built support modules) |
| Framework Self-Tests | `tests/framework/` | Active (CTest label: `framework`) |
| Exercise Generator CLI | `scripts/add_problem.py` | Active (Python scaffolding CLI) |