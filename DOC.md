# Technical Reference

This document is for developers and agents working on the repository. The
user-facing workflow is intentionally kept brief in [`README.md`](README.md).

## Project model

Each directory under `problems/` is an independent exercise. Candidate
implementations are C source files and are intentionally incomplete. Test
runners are C++ and use the vendored GoogleTest framework. The framework must
build and discover unfinished exercises without requiring their assertions to
pass.

Current problem directories:

| Directory | Build manifest | Notes |
|---|---|---|
| `000_example` | standard | Framework integration example |
| `001_swap_2_nums` | standard | C implementation and C++ tests |
| `002_check_power_of_2` | standard | Intentionally incomplete |
| `003_endianess_detection` | standard | Public spelling is preserved |
| `004_bit_manip` | standard | Intentionally incomplete |
| `005_memcpy` | special | Uses `my_memcpy.c` and support utilities |
| `006_memmove` | special | Uses `my_memmove.c` and support utilities |

Do not complete or repair an exercise unless that is explicitly the task.
When an unfinished value-returning function needs a stub, it must return a
deterministic value in its valid return domain; unfinished `void` functions
may remain empty. Missing returns, uninitialized reads, and undefined
behavior are not acceptable.

## Repository layout

```text
.
|-- .devcontainer/devcontainer.json   # Development container and extensions
|-- .vscode/settings.json             # CMake editor settings
|-- infra/
|   |-- CMakeLists.txt                # GoogleTest and problem registration
|   |-- cmake/templates/              # Standard and special problem templates
|   |-- support/                      # Reusable framework C/C headers and code
|   `-- third_party/googletest/       # Offline GoogleTest source
|-- problems/                         # Independent exercises
|-- CMakeLists.txt                    # Root CMake project
|-- DOC.md                            # This technical reference
`-- README.md                         # Brief user guide
```

Generated files belong in the out-of-source `build/` directory, which is
ignored by Git.

## Build system

The root [`CMakeLists.txt`](CMakeLists.txt) enables C and C++, requires C11 for
candidate code, enables CTest, and adds `infra/`. The infrastructure CMake
file:

1. configures vendored GoogleTest with GoogleMock and installation disabled;
2. discovers `problems/*` directories with `CONFIGURE_DEPENDS`;
3. adds only directories containing `CMakeLists.txt`; and
4. builds and registers the `framework_smoke_test`.

GoogleTest is compiled from
[`infra/third_party/googletest/`](infra/third_party/googletest/), so
configuration must not download dependencies.

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

For a clean verification:

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

The generated target for a problem directory is always
`prob_<folder_name>`. List targets or tests from the build directory when
debugging discovery:

```bash
cmake --build build --target help
ctest --test-dir build -N
```

## Problem CMake contract

Every migrated problem must:

- live in `problems/NNN_problem_name/`;
- contain a `CMakeLists.txt`;
- use `test_runner.cpp` as its C++ test runner;
- keep candidate implementation files as `.c`;
- expose problem headers beside their implementations; and
- produce a target named `prob_<folder_name>`.

### Standard problems

Include [`standard-problem.cmake`](infra/cmake/templates/standard-problem.cmake)
when the problem has the conventional layout. The template:

- derives the problem name and target from `CMAKE_CURRENT_SOURCE_DIR`;
- collects every local `*.c` file with `CONFIGURE_DEPENDS`;
- compiles those C files with `test_runner.cpp`;
- adds the problem and `infra/support` include directories;
- links `gtest_main`; and
- calls `gtest_discover_tests()` with the problem directory as a test prefix
  and the `problem` label.

The standard template must not accidentally compile a legacy runner. Only
current `.c` implementation files should be present in a standard problem.

### Special problems

Include [`special-problem.cmake`](infra/cmake/templates/special-problem.cmake)
when source discovery is not appropriate or when explicit infrastructure
sources, libraries, or compile definitions are needed. Set
`PROBLEM_SOURCES` and, when needed, `PROBLEM_INCLUDE_DIRECTORIES`,
`PROBLEM_COMPILE_DEFINITIONS`, and `PROBLEM_EXTRA_LIBRARIES` before including
the template.

The memory problems use this form to compile their implementation and
[`infra/support/utils.c`](infra/support/utils.c), with declarations from
[`infra/support/utils.h`](infra/support/utils.h).

## Testing conventions

CTest names are prefixed with the problem directory, for example
`000_example/Example.SumEq`. Use that prefix for focused runs:

```bash
ctest --test-dir build -R '^000_example/' --output-on-failure
ctest --test-dir build -R '^005_memcpy/' --output-on-failure
```

The framework smoke test is labelled `framework`:

```bash
ctest --test-dir build -L framework --output-on-failure
```

The `problem` label covers discovered exercise tests. A full run may return a
nonzero status because unfinished implementations fail assertions. Classify
these separately from configuration, compilation, discovery, linker, and
framework smoke-test failures.

When converting or adding tests, preserve the intended inputs, assertions,
test names where practical, and coverage. Do not modify candidate
implementations merely to make framework tests pass.

## Development environment

The supported environment is the Ubuntu 24.04 image declared in
[`.devcontainer/devcontainer.json`](.devcontainer/devcontainer.json). It
provides the C/C++ toolchain and installs the CMake Test Explorer extension.
The repository currently relies on terminal CMake/CTest commands as the
portable fallback.

Required tools:

```bash
gcc --version
make --version
cmake --version
ctest --version
```

## Future work

The refactor is complete. The following work is intentionally deferred and
ordered by importance.

### Priority 1: Onboarding and core workflow

1. Automate Dev Container onboarding, including compiler selection, CMake
   configuration, repository settings, and unavoidable trust/reopen steps.
2. Add a friendly VS Code workflow that detects the currently active problem
   directory and provides a prebuilt button, task, or command to build only
   that problem and immediately run its tests. The normal practice loop should
   require no typed CMake or CTest commands: users should be able to edit an
   implementation, click or invoke the action, and see useful build/test
   failures. Also provide an equivalent action for building and testing all
   problems, refresh the test UI automatically, and keep the documented
   terminal commands as a fallback. New problems should not require
   per-problem editor configuration.
3. Add problem-directory validation and scaffolding for the
   `NNN_problem_name` layout, required files, and standard template output.
   Report invalid names instead of silently renaming directories.
4. Add framework diagnostics and problem status metadata. Define statuses such
   as framework-only, expected-to-pass, and intentionally unfinished; expose
   them through CMake/CTest labels or properties; and provide a diagnostic
   command or framework test that separates configuration, build, discovery,
   infrastructure, and expected exercise failures.

### Priority 2: Build and maintenance quality

1. Improve compiler diagnostics, warning flags, and dependency tracking.
2. Add optional AddressSanitizer and UndefinedBehaviorSanitizer configurations.
3. Add CI for configuration, build, discovery, and documented unfinished
   exercise failures.
4. Document reproducible offline GoogleTest dependency updates while
   preserving licenses and clean builds.

### Priority 3: Exercise framework expansion

1. Define and document the supported `infra/support/` layout.
2. Define a policy for optional reference implementations.
3. Add exercises such as ring buffers, producer/consumer systems, and static
   memory pools.
4. Add templates for multiple libraries, platform-specific sources, or
   generated data.
5. Consider GoogleMock only if a future exercise genuinely requires it.

### Priority 4: Portability and long-term maintenance

1. Validate the Dev Container workflow on Linux and Codespaces.
2. Pin or document Dev Container image versions and update policy.
3. Add line-ending policy and cross-platform shell guidance.
4. Improve documentation as the workflow evolves.
5. Consider a compatibility strategy for the public spelling of
   `endianess_detection`.
6. Optimize the vendored GoogleTest tree if repository size becomes a concern.
