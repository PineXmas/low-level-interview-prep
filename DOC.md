# Low-Level Interview Prep: Project and Framework Guide

## 1. Project Overview

### Purpose

`low-level-interview-prep` is a C practice framework for low-level, embedded, and firmware interview problems. It emphasizes operations that are easy to under-specify or difficult to practice on ordinary hosted coding platforms, including bit manipulation, integer properties, byte order, and memory operations.

The repository is organized around a simple principle:

> The project is a reusable exercise framework. Each directory under `problems/` is an independent exercise module that plugs into that framework.

This distinction matters because adding a problem does not require changing the core build system. A problem supplies:

- a public C header describing its API;
- an implementation source to fill or revise;
- tests defined with Acutest; and
- a local Makefile defining the source list for that exercise.

The repository root coordinates the environment and all-problem build. Individual problem directories are the units of implementation and test execution.

### Intended users and workflow

The expected workflow is:

1. Open the repository in the Dev Container.
2. Choose a problem under `problems/`.
3. Implement or revise the declared API in that problem's source file.
4. Build it directly with `make` or build every problem from VS Code.
5. Run the generated `test.out` executable directly or through the Acutest Test Explorer panel.
6. Repeat until the problem-specific tests pass.

The repository also includes a `self_check` problem whose purpose is to confirm that the test UI itself is working. It is a framework diagnostic, not an interview exercise.

## 2. Current Status

The repository is a functioning scaffold, but it is intentionally not a completed solution set.

- The repository builds successfully with GCC 13 and GNU Make 4.3 in the Ubuntu 24.04 Dev Container.
- `problems/self_check/test.out` passes and validates test UI execution.
- The other current exercise suites are unfinished or stubs and fail intentionally:
  - `bit_manip`: implementations are empty.
  - `check_power_of_2`: implementation aborts.
  - `endianess_detection`: implementation aborts and the environment-endianness constant must be corrected.
  - `memcpy`: implementation is empty.
  - `memmove`: implementation is empty.
  - `swap_2_nums`: implementation is empty.

A clean build is therefore not a claim that every exercise passes. The build only verifies that source files compile and link.

## 3. Repository Structure

```text
.
|-- .devcontainer/
|   `-- devcontainer.json       # Container-based development environment
|-- .vscode/
|   |-- settings.json           # Test Explorer problem registration
|   `-- tasks.json              # Build-all and clean-all VS Code tasks
|-- problems/
|   |-- bit_manip/
|   |-- check_power_of_2/
|   |-- endianess_detection/
|   |-- memcpy/
|   |-- memmove/
|   |-- self_check/
|   `-- swap_2_nums/
|-- shared/
|   |-- acutest.h               # Vendored test framework
|   |-- common.mk               # Shared compile/link rules
|   |-- utils.c                 # Shared memory-comparison helper
|   `-- utils.h
|-- private/
|   |-- notes.txt               # Local integration notes and known TODOs
|   `-- extension_modifications/
|       |-- Acutest.js
|       `-- package.json
|-- README.md
|-- DOC.md
`-- .gitignore
```

Generated `test.out` binaries are ignored and should not be committed. They are artifacts produced from each problem's `Makefile`.

### Root and framework responsibilities

The repository root owns:

- the Linux development environment declaration;
- VS Code task orchestration;
- Test Explorer configuration;
- shared build flags;
- shared test infrastructure and utilities; and
- documentation and repository-wide guidance.

A problem directory owns:

- its public API;
- its implementation;
- its tests; and
- its exact build source list.

This separation keeps problem implementations independent and makes `problems/` suitable for future growth.

## 4. Architecture

### 4.1 Framework pipeline

```text
problems/<name>/test_runner.c
        |
        | defines TEST_LIST
        v
shared/acutest.h  +  problem test functions
        |
        | compiled with problem implementation and optional shared sources
        v
problems/<name>/test.out
        |
        +--> direct CLI execution
        `--> Acutest VS Code Test Explorer
```

The build and test systems meet at the problem Makefile:

```text
problem Makefile
  -> common.mk
  -> compile/link test_runner.c and problem sources
  -> problem-local test.out
  -> explicit VS Code registration
  -> Test Explorer discovery
```

### 4.2 Shared build abstraction

[shared/common.mk](shared/common.mk) defines the common build contract:

- compiler: `gcc`;
- flags: `-Wall -Wextra`;
- shared include path: `../../shared`;
- default target: `$(TARGET)`;
- target recipe: compile and link all `$(SOURCES)` into one executable; and
- clean rule: remove `$(TARGET)`.

Each problem defines at least:

```make
PROJ_ROOT := $(abspath ../..)
TARGET    := test.out
SOURCES   := test_runner.c <problem sources>

include $(PROJ_ROOT)/shared/common.mk
```

A problem can add `../../shared/utils.c` or another shared source to `SOURCES` when required. The common include path allows `#include "acutest.h"`, `#include "utils.h"`, and other headers from `shared/`.

This is a deliberately lightweight build framework. It does not install a package, create a static library, or produce a reusable binary artifact. The executable is the test runner for one exercise.

### 4.3 Shared test abstraction

[shared/acutest.h](shared/acutest.h) is a vendored single-header C/C++ test framework. It provides:

- `TEST_LIST`, the test suite registration macro;
- test assertions such as `TEST_CHECK`;
- a generated `main()` implementation; and
- CLI output and exit-status handling.

Each test runner contains one `TEST_LIST` terminator:

```c
TEST_LIST = {
    { "test name", test_function },
    { NULL, NULL }
};
```

The final `{ NULL, NULL }` entry is required. Test functions take no arguments and return `void`; failures are reported through Acutest assertion macros.

### 4.4 Shared utility abstraction

[shared/utils.h](shared/utils.h) and [shared/utils.c](shared/utils.c) currently provide `is_same_content_memory()`, a byte-wise `uint8_t` comparison helper used by memory-operation tests.

This is the only reusable source module outside the test framework. Shared utilities are compiled only into problems that include `../../shared/utils.c` in `SOURCES`; they are not globally linked to every executable.

## 5. Problem Modules

The following is a brief map of the current problem set. It describes each module's place in the framework, not a full solution analysis.

| Module | Public API | Test contract | Shared framework dependency |
|---|---|---|---|
| `bit_manip` | register get/set/clear/toggle operations | 8 focused bit-operation tests | Acutest only |
| `check_power_of_2` | `is_power_of_2()` | 8 boundary and representative values | Acutest only |
| `endianess_detection` | `is_little_endian()` | compare against a host-endianness constant | Acutest only |
| `memcpy` | `my_memcpy()` | arrays, strings, and randomized words | Acutest and `utils.c` |
| `memmove` | `my_memmove()` | overlapping ranges, boundaries, strings, and identical pointers | Acutest and `utils.c` |
| `self_check` | none | test adapter/UI execution diagnostic | Acutest only |
| `swap_2_nums` | `swap()` | zero, ordinary values, and `uint32_t` maximum | Acutest only |

The special `test_bit_manip.h` file demonstrates that tests may be split across more than one source/header file. In that module, `test_runner.c` includes the file containing test function definitions and then publishes `TEST_LIST`.

Each exercise is an independent object of the framework, not a package or runtime service. Problems do not communicate with one another and do not share state.

## 6. Build and Test Lifecycle

### 6.1 Open the workspace

The primary supported environment is VS Code open in the repository Dev Container. The container configuration is [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json).

It uses:

```text
mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04
```

and installs these VS Code extensions:

- `ms-vscode.cpptools` for C/C++ editing and debugging;
- `matepek.vscode-catch2-testadapter` for test UI integration.

The Test Adapter configuration is in [.vscode/settings.json](.vscode/settings.json). A potentially confusing name in that file is historical: the configured test framework is Acutest, while the VS Code extension is distributed under a Catch2-oriented name.

### 6.2 Build all problems

From the repository root:

```bash
make -C problems/bit_manip
make -C problems/check_power_of_2
make -C problems/endianess_detection
make -C problems/memcpy
make -C problems/memmove
make -C problems/self_check
make -C problems/swap_2_nums
```

Or use the repository-wide VS Code task:

```text
Terminal > Run Task... > Build all
```

The task iterates over `problems/*/` and calls `make` for every directory that contains a Makefile. It is equivalent in intent to:

```bash
for d in problems/*/; do
    if [ -f "$d/Makefile" ]; then
        (cd "$d" && make)
    fi
done
```

A local build normally succeeds even for empty implementations because the compiler validates syntax and type contracts but does not execute test logic.

### 6.3 Build one problem

```bash
cd problems/<name>
make
```

The exact executable is always `test.out` under the selected problem directory. Rebuild after changes by rerunning `make`; source timestamps determine which artifacts are relinked.

Clean one problem:

```bash
make clean
```

Clean every problem:

```text
Terminal > Run Task... > Clean all
```

or use the equivalent loop in a shell.

### 6.4 Run tests from the terminal

```bash
./problems/self_check/test.out
./problems/bit_manip/test.out
```

A passing Acutest executable normally exits with status `0`; a failing suite exits with a nonzero status. `endianess_detection` is currently not only a normal assertion failure: its unfinished implementation aborts, so it exits by signal.

An example baseline is:

| Suite | Current result |
|---|---|
| `self_check` | pass |
| `bit_manip` | fail: 8 tests |
| `check_power_of_2` | abort: 8 tests |
| `endianess_detection` | abort |
| `memcpy` | fail: 3 tests |
| `memmove` | fail: 3 tests |
| `swap_2_nums` | fail: 4 tests |

### 6.5 Run tests from VS Code

The Test Explorer discovers explicitly configured groups from [.vscode/settings.json](.vscode/settings.json):

```json
"acutestExplorer.testGroups": [
  "problems/self_check",
  "problems/bit_manip",
  "problems/swap_2_nums",
  "..."
]
```

The shared settings name the runner source and output executable:

```text
acutestExplorer.testGroupsExeFile = test.out
acutestExplorer.testGroupsSrcFile = test_runner.c
```

The extension therefore expects each registered problem to produce:

- `test_runner.c` containing `TEST_LIST`; and
- `test.out` in that problem directory.

Build the suite before refreshing or running tests in the panel. [private/notes.txt](private/notes.txt) records that automatic test discovery after build is a TODO.

## 7. Deployment and Dev Container Model

### Clarification: this project is not a conventional deployed application

There is no server, package deployment, release artifact, Docker Compose stack, or standalone Dockerfile in the current repository. The project is prepared for local practice inside a containerized development environment.

The current deployment-like approach is:

1. A developer clones or opens the repository.
2. VS Code detects [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json).
3. VS Code + the Dev Containers extension pulls and starts `mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04`.
4. The repository is mounted in the container.
5. GCC, Make, and project dependencies are available without host-level setup.
6. The developer uses VS Code tasks and tests inside that environment.

This is technically Docker-compatible, but the exact prerequisite is not simply “any environment that supports Linux.” It requires Docker plus a compatible container workflow:

- Docker Engine on a Linux host;
- Docker Desktop or another Docker-compatible runtime on macOS or Windows, with Dev Container support enabled;
- VS Code and the Dev Containers extension, for the documented workflow;
- or a separate tool that consumes `devcontainer.json` and presents the repository similarly.

On Windows, WSL 2 is a valid and common architecture, but only if Docker Desktop is configured to expose containers to WSL and VS Code is opened inside or attached to the WSL-compatible environment. WSL alone is not sufficient. Native Docker and the Dev Containers extension are the relevant technologies; Linux support is an implementation detail, not the complete requirement.

The statement can be corrected to:

> Users can clone the repository and open it in VS Code's Dev Container support, where Docker supplies a consistent Ubuntu 24.04 C/C++ toolchain. Windows users commonly accomplish this with Docker Desktop and WSL 2. Docker or another container runtime is still required; no host GCC or Make installation is needed for the documented Dev Container path.

### Important current gaps

- There is no pinned digest or digest policy for the base image. Tag `1-ubuntu-24.04` can move over time.
- There is no `Dockerfile`, so no repository-specific image is built or versioned.
- No multi-stage build, release image, or runtime image exists.
- The container does not run tests automatically.
- `docker run ...` instructions are not provided and would not reproduce the VS Code test adapter automatically.

## 8. How a New Problem Is Plugged In

Adding a problem is currently a five-file-style, explicit-registration workflow. A small directory-level scaffold is sufficient:

```text
problems/<new_problem>/
|-- Makefile
|-- <problem>.h
|-- <problem>.c
`-- test_runner.c
```

Additional shared test headers may be used, but the runner remains the discovery point.

### Step 1: Create the module directory

Create `problems/<new_problem>/` and keep the module self-contained unless it intentionally uses `shared/` code.

### Step 2: Define the API and implementation

Define the public contract in `<problem>.h`, then implement it in `<problem>.c`.

Keep types explicit and include only dependencies the module actually uses. This project uses C99-compatible fixed-width types in several APIs, especially `stdint.h`, and compiles with warnings enabled.

### Step 3: Define tests

Create `test_runner.c` and include:

- `acutest.h`;
- the problem header; and
- any test helper headers.

Define test functions with the Acutest contract:

```c
void test_example(void) {
    TEST_CHECK(/* condition */);
}

TEST_LIST = {
    { "test_example", test_example },
    { NULL, NULL }
};
```

Tests should make the API contract observable: normal cases, edge cases, boundary values, invalid assumptions, and overlap behavior where relevant.

### Step 4: Define the local build

Create a local Makefile following the current pattern:

```make
PROJ_ROOT := $(abspath ../..)

TARGET  := test.out
SOURCES := test_runner.c <problem>.c

include $(PROJ_ROOT)/shared/common.mk
```

Include a shared source when it is actually required:

```make
SOURCES := test_runner.c $(PROJ_ROOT)/shared/utils.c <problem>.c
```

Build and clean behavior is inherited from [shared/common.mk](shared/common.mk). The problem name does not need to become a build variable elsewhere.

### Step 5: Register the test group

Add `problems/<new_problem>` to `acutestExplorer.testGroups` in [.vscode/settings.json](.vscode/settings.json).

Current defaults are:

```json
"acutestExplorer.testGroupsExeFile": "test.out",
"acutestExplorer.testGroupsSrcFile": "test_runner.c"
```

Those defaults are inherited by every registered problem. Use different names only when the new module follows a deliberately different convention, and update both the Makefile and settings accordingly.

### Step 6: Build and verify

```bash
make -C problems/<new_problem>
./problems/<new_problem>/test.out
```

Also run the repository build loop to ensure the new module coexists with every existing problem:

```bash
for d in problems/*/; do
    if [ -f "$d/Makefile" ]; then
        (cd "$d" && make) || exit
    fi
done
```

Finally refresh tests in the Test Explorer and confirm that the suite appears and runs.

### Extension contract summary

A future problem is compatible with the framework when it satisfies all of these properties:

1. It lives under `problems/<name>/`.
2. It has a `Makefile` that sets `TARGET`, `SOURCES`, and includes `shared/common.mk`.
3. It produces `test.out`.
4. It compiles `test_runner.c` containing `TEST_LIST`.
5. It is registered in `.vscode/settings.json` for Test Explorer use.
6. Its tests can be executed independently from every other problem.

The framework can scale horizontally as more directories are added. What does not currently scale automatically is discovery: the all-problem build scans directories dynamically, while test registration is manual.

## 9. Recommended Evolution for a Larger Problem Set

For a larger collection, the framework should remain simple but remove duplication and manual drift.

### 9.1 Automatic Test Explorer discovery

The private notes already identify this gap. The strongest target is to discover problem directories dynamically from `problems/*/test_runner.c` and `problems/*/test.out`, while retaining the ability to opt a module out.

An automatic discovery implementation should define one documented filename contract and fail clearly when:

- `test.out` is absent;
- `test_runner.c` is absent;
- `TEST_LIST` is missing or malformed;
- a problem is registered but no longer exists; or
- generated artifacts are stale.

[private/extension_modifications/Acutest.js](private/extension_modifications/Acutest.js) contains local customizations for flexible paths, test names, output, and multiple test groups. It is not installed as an image build step or declared as a normal project dependency, so this integration is fragile. Any future solution should keep it out of `private/` or document a reproducible extension packaging workflow.

### 9.2 Generated or consistent problem metadata

A future Makefile template could keep only problem-specific sources while deriving standard targets and conventions. Options include:

- a root Makefile that discovers every child Makefile;
- a per-problem manifest; or
- a one-line problem registration file consumed by both Make and VS Code configuration.

The goal should be a single source of truth. Currently, adding a problem requires coordinating the directory, Makefile, and settings list independently.

### 9.3 Build dependency tracking

The current rule treats `$(SOURCES)` as prerequisites, but headers are not dependencies. A header-only change can therefore leave a stale executable. A future change could add compiler-generated dependency files, for example:

```make
CFLAGS += -MMD -MP
DEPS := $(SOURCES:.c=.d)
-include $(DEPS)
```

This is especially valuable as shared headers evolve.

### 9.4 Automated CI

A future CI workflow should:

- build all problem directories;
- run all test executables;
- distinguish intentionally unimplemented exercises from actual regressions; and
- ensure documentation/configuration remains synchronized.

A practical first step is to mark exercise progress explicitly. Until then, CI should not run “all tests and require all pass” because that would fail the entire repository by design while exercises remain unfinished.

### 9.5 Environment reproducibility

The environment would become more reproducible with:

- a local Dockerfile or devcontainer feature for repository-specific setup;
- a pinned base-image digest;
- pinned VS Code extension versions where relevant;
- explicit shell and compiler version checks; and
- an optional portable test command that does not depend on VS Code.

These changes are enhancements, not requirements for the current learning workflow.

## 10. Current Known Gaps and Risks

### Framework gaps

- Adding a problem requires manual `.vscode/settings.json` registration.
- Test discovery does not happen automatically after build.
- There is no root Makefile.
- Header dependencies are not tracked.
- There is no CI configuration.
- Build flags do not enable strict C standards or sanitizers.
- Test helpers and exercise naming are inconsistent in places.

### Exercise gaps

- Most problem implementations are empty or abort.
- `endianess_detection` hardcodes a host-endianness marker that must match the executing environment.
- The test suite does not prove every API edge case.
- Random testing in `memcpy` uses runtime randomness and only checks one randomized allocation per run.
- Several test sources are small and would benefit from overflow, invalid-range, and in-place behavior cases.

### Development risks

- `shared/acutest.h` is vendored and should be reviewed before upgrading.
- Private extension modifications are outside normal build packaging and may be lost or overwritten during extension updates.
- The `matepek` extension name does not describe Acutest in the repository, so the role of the extension is easy to misunderstand.
- The misspelling `endianess_detection` is used consistently in the directory, API include guard, function, and settings. It is public current structure rather than a harmless internal typo. Renaming it would require a compatibility and repository migration plan.

## 11. Conventions for Future Development

For consistent future changes:

1. Keep one problem per directory.
2. Keep public API declarations in a header adjacent to the implementation.
3. Keep test registration in `test_runner.c` unless a deliberate split convention is established.
4. Keep `test.out` as the executable name unless the framework settings are changed together.
5. Include `shared/common.mk` rather than duplicating build flags.
6. Reuse `shared/` only for genuinely cross-problem behavior.
7. Add tests before or alongside implementation changes.
8. Run both the individual suite and repository-wide build loop.
9. Update [.vscode/settings.json](.vscode/settings.json) whenever a discoverable test group is added or removed.
10. Do not commit generated binaries or core dumps.
11. Keep private tooling changes reproducible and separate from public framework configuration.
12. Separate a failing exercise from a broken build in issue reports and review notes.

## 12. Quick Reference

### Build everything

```bash
for d in problems/*/; do
    if [ -f "$d/Makefile" ]; then
        (cd "$d" && make) || exit
    fi
done
```

### Clean everything

```bash
for d in problems/*/; do
    if [ -f "$d/Makefile" ]; then
        (cd "$d" && make clean) || exit
    fi
done
```

### Build and run one exercise

```bash
make -C problems/<name>
./problems/<name>/test.out
```

### Clean one exercise

```bash
make -C problems/<name> clean
```

### Add a problem

1. Create `problems/<name>/`.
2. Add implementation and public header.
3. Add `test_runner.c` with `TEST_LIST`.
4. Add a Makefile that targets `test.out` and includes `shared/common.mk`.
5. Register `problems/<name>` in `.vscode/settings.json`.
6. Build and run the suite.

## 13. Source-of-Truth Map

| Concern | Source of truth |
|---|---|
| Practice purpose | [README.md](README.md) |
| Container environment | [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json) |
| Build tasks | [.vscode/tasks.json](.vscode/tasks.json) |
| Test discovery settings | [.vscode/settings.json](.vscode/settings.json) |
| Shared compile/link rules | [shared/common.mk](shared/common.mk) |
| Test framework | [shared/acutest.h](shared/acutest.h) |
| Shared memory utility | [shared/utils.c](shared/utils.c), [shared/utils.h](shared/utils.h) |
| Problem-specific build source lists | `problems/*/Makefile` |
| Problem test registrations | `problems/*/test_runner.c` |
| Repository status context | [private/notes.txt](private/notes.txt) |

This map should be used as the first point of reference when diagnosing or changing behavior. A future developer or agent should change the owning source of truth rather than adding special cases elsewhere.
