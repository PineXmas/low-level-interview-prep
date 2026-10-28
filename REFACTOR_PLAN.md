# Refactor Plan: CMake, CTest, and GoogleTest Framework

## 1. Purpose

This repository is a framework for practicing low-level engineering and
embedded-systems interview problems.

The goal of this refactor is **not** to solve the exercises under
`problems/`. Those implementations are intentionally incomplete so that a
user can clone the repository and practice solving them independently.

The goal is to provide a framework that:

- hosts many independent low-level problems;
- lets a new user start quickly;
- builds consistently inside the development container;
- makes tests easy to discover and run;
- keeps each problem isolated;
- supports future framework growth without requiring repeated central
  configuration edits; and
- clearly separates framework failures from intentionally unfinished
  exercise implementations.

The existing Makefile/Acutest workflow will be replaced by CMake, CTest, and
GoogleTest in small, reviewable steps.

## 2. Current Repository Facts

The current repository contains:

- independent problem directories under `problems/`;
- C implementation files and public C headers;
- Acutest-based C test runners;
- one Makefile per problem;
- shared Acutest and Make helpers;
- shared utility code used by memory-related problems;
- a Dev Container based on Ubuntu 24.04; and
- VS Code settings and tasks tied to the Acutest workflow.

Current problem implementation files are intentionally unfinished:

- `bit_manip/bit_manip.c`;
- `check_power_of_2/check_power_of_2.c`;
- `endianess_detection/endianess_detection.c`;
- `memcpy/my_memcpy.c`;
- `memmove/my_memmove.c`; and
- `swap_2_nums/swap_2_nums.c`.

The refactor must preserve existing implementation names, including
`my_memcpy.c` and `my_memmove.c`.

## 3. Scope of This Refactor

This refactor includes:

1. Adding a root CMake build.
2. Adding CTest integration.
3. Vendoring GoogleTest into the repository.
4. Verifying that the Dev Container supplies the required build tools.
5. Migrating each existing test runner from Acutest C to GoogleTest C++.
6. Keeping candidate implementations as C.
7. Adding a reusable standard problem CMake template.
8. Adding a special-problem CMake template.
9. Automatically deriving each target name from its problem directory.
10. Automatically compiling all local `.c` files for standard problems.
11. Automatically registering problem subdirectories from the root CMake file.
12. Removing legacy files only after each migrated problem is verified.
13. Verifying the complete new framework workflow.

## 4. Explicit Non-Goals

This migration must not:

- solve or complete any problem implementation;
- require all problem tests to pass;
- rename existing problem implementation files;
- add GoogleMock as a dependency or build target;
- require Ninja;
- add reference implementations for future problems;
- add sanitizer support;
- add CI;
- redesign all project documentation;
- validate malformed problem directories;
- implement automatic scaffolding;
- implement automatic CTest/Test Explorer integration; or
- guarantee identical behavior on every host operating system.

Those items are recorded as future work below.

## 5. Target Architecture

The target repository structure is:

```text
.
|-- CMakeLists.txt
|-- cmake/
|   `-- templates/
|       |-- standard-problem.cmake
|       `-- special-problem.cmake
|-- problems/
|   |-- <problem_name>/
|   |   |-- CMakeLists.txt
|   |   |-- test_runner.cpp
|   |   |-- <implementation>.c
|   |   `-- <implementation>.h
|   `-- ...
|-- shared/
|   `-- ...
|-- third_party/
|   `-- googletest/
`-- build/
```

`build/` is an out-of-source generated directory and must remain ignored by
Git.

The architecture has these responsibilities:

| Layer | Responsibility |
|---|---|
| Dev Container | Provides GCC, Make, CMake, CTest, and the normal C/C++ tools |
| Root CMake | Adds GoogleTest and problem subdirectories |
| Problem manifest | Includes the standard or special problem template |
| Standard template | Defines the default target, sources, includes, and tests |
| Special template | Provides an explicit-source customization starting point |
| CTest | Discovers and runs individual GoogleTest cases |
| Problem implementation | Remains candidate-owned C code |

## 6. Tool and Dependency Ownership

### 6.1 Tools supplied by the Dev Container

The project should use the Dev Container image for:

- GCC;
- Make;
- CMake;
- CTest;
- Git; and
- the normal C/C++ development utilities.

CMake and CTest should not be copied into the repository. CTest is distributed
with CMake.

The first environment checkpoint must verify the installed tools:

```bash
gcc --version
make --version
cmake --version
ctest --version
```

If a required tool is absent, update the container configuration rather than
vendoring the tool into the project.

Ninja is intentionally not required. The documented interface is the
generator-neutral CMake workflow:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

### 6.2 Vendored GoogleTest

GoogleTest is a source dependency rather than a tool supplied by the base
container. The repository will vendor the upstream GoogleTest source under:

```text
third_party/googletest/
```

The initial migration will use GoogleTest release **v1.17.0**, copied directly
into the repository. The exact upstream license and attribution files must be
preserved.

The upstream tree may contain GoogleMock. GoogleMock will not be configured,
linked, or used by this project during the initial migration. Keeping the
upstream GoogleMock source temporarily is acceptable and can be optimized in
a later dependency-maintenance phase.

The root CMake file will add the vendored GoogleTest source locally rather than
fetching it during configuration. A clean configure/build must not require
network access.

## 7. Standard Problem Contract

The standard problem contract is:

```text
problems/<problem_name>/
|-- CMakeLists.txt
|-- test_runner.cpp
|-- one or more implementation .c files
|-- public and private headers as needed
`-- optional test support files
```

For a standard problem:

- `test_runner.cpp` is the only test runner compiled by the CMake template;
- every local `.c` file is treated as an implementation source;
- no local `.c` file may define another `main()`;
- no local `.c` file may be an alternate build variant;
- the problem target is named `prob_<problem_name>`;
- the problem include directory is available;
- the shared include directory is available; and
- GoogleTest/CTest registration is added automatically.

Existing names are preserved. For example:

```text
problems/memcpy/
|-- CMakeLists.txt
|-- test_runner.cpp
|-- my_memcpy.c
|-- my_memcpy.h
`-- ...
```

The standard template compiles `my_memcpy.c` without requiring it to be
renamed.

## 8. Standard and Special CMake Templates

### 8.1 Standard template

The standard template will live at:

```text
cmake/templates/standard-problem.cmake
```

Each ordinary problem's `CMakeLists.txt` will contain only:

```cmake
include("${PROJECT_SOURCE_DIR}/cmake/templates/standard-problem.cmake")
```

The included template will:

1. derive the problem name from `CMAKE_CURRENT_SOURCE_DIR`;
2. create target `prob_<problem_name>`;
3. collect all local `*.c` files;
4. add `test_runner.cpp`;
5. add problem and shared include directories;
6. link GoogleTest;
7. register tests through `gtest_discover_tests`; and
8. apply the `problem` CTest label.

Because the file is included from a problem directory, the template must use
`CMAKE_CURRENT_SOURCE_DIR` to identify the problem folder. It must not use
`CMAKE_CURRENT_LIST_DIR`, which points to `cmake/templates/`.

The source glob should use `CONFIGURE_DEPENDS` so CMake can notice changes to
the list of local C files:

```cmake
file(GLOB PROBLEM_C_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)
```

### 8.2 Special template

The special template will live at:

```text
cmake/templates/special-problem.cmake
```

It will demonstrate explicit source lists and clearly commented customization
sections. It is intended for problems that need:

- selected rather than all local C files;
- additional shared source files;
- additional libraries;
- special compile definitions;
- platform-specific setup; or
- a non-standard test arrangement.

The templates are scaffolding aids, not live shared configuration. Existing
problem manifests remain independent after creation, so future template edits
do not silently alter existing problems.

## 9. Root CMake Problem Registration

Automatic validation of problem contents is deferred. The initial root CMake
will assume that every direct directory under `problems/` is intentionally
created as a valid problem directory.

The root CMake will automatically add problem subdirectories rather than
requiring a central list of individual problem names. Conceptually:

```cmake
file(GLOB PROBLEM_DIRECTORIES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/problems/*"
)

foreach(PROBLEM_DIRECTORY IN LISTS PROBLEM_DIRECTORIES)
    if(IS_DIRECTORY "${PROBLEM_DIRECTORY}")
        add_subdirectory("${PROBLEM_DIRECTORY}")
    endif()
endforeach()
```

The initial implementation will not inspect whether a problem directory
contains the expected files. If a user creates an incomplete problem
directory, CMake is allowed to report the resulting normal configuration or
build error.

## 10. C and C++ Boundary

Candidate implementations remain C and must continue to compile as C.

Test runners become C++ because GoogleTest is a C++ library. When a C header is
included by a C++ test runner, use an `extern "C"` block:

```cpp
extern "C" {
#include "problem_header.h"
}
```

The migration must preserve the original test intent and coverage. It must
not weaken assertions merely to make a converted test compile.

## 11. Expected Test Results

The repository has two separate test expectations.

### 11.1 Framework behavior that must pass

The migration must verify that:

- the container provides the required tools;
- CMake configures;
- vendored GoogleTest builds;
- a GoogleTest runner links;
- CTest discovers tests;
- problem targets compile;
- problem tests execute and report results; and
- the complete migrated workflow can be configured from a clean build
  directory.

### 11.2 Exercise behavior that may fail

Tests for unfinished problem implementations are expected to fail until a user
implements the exercise. This is normal and is not a framework failure.

The migration must not modify implementations just to make the migration
tests pass.

## 12. Checkpoint-Based Migration Plan

Each step below is intentionally small. Review the changes and create a
checkpoint commit before moving to the next step. Commit messages are
suggestions; commits should include the repository's required co-author
trailer when created.

### Step 0: Record the baseline

**Objective**

Record the current workflow and distinguish framework behavior from expected
exercise failures.

**Actions**

- Confirm the current repository status.
- Confirm the existing problem directories and source files.
- Confirm the current Dev Container image.
- Confirm which current tests are expected to fail because implementations are
  incomplete.
- Remove the stale `problems/self_check_2` VS Code registration if it is still
  present.

**Verification**

- Existing files remain unchanged except for the stale registration cleanup.
- The repository status is understood before the migration begins.

**Checkpoint**

Suggested commit:

```text
chore: clean stale test registration before cmake migration
```

### Step 1: Verify the container toolchain

**Objective**

Confirm that the existing container provides the required tools.

**Actions**

- Rebuild or reopen the Dev Container if necessary.
- Run the version checks for GCC, Make, CMake, and CTest.
- Do not add a repository copy of these tools.
- Update the container configuration only if a required tool is actually
  missing.

**Verification**

```bash
gcc --version
make --version
cmake --version
ctest --version
```

**Checkpoint**

Suggested commit:

```text
chore: verify cmake build toolchain in dev container
```

### Step 2: Vendor GoogleTest

**Objective**

Add a local, offline-capable GoogleTest dependency.

**Actions**

- Add the selected GoogleTest v1.17.0 source under
  `third_party/googletest/`.
- Preserve upstream license and attribution files.
- Do not configure or use GoogleMock.
- Do not change problem tests yet.

**Verification**

- The expected GoogleTest source files are present.
- No network is needed merely to access the dependency after checkout.

**Checkpoint**

Suggested commit:

```text
build: vendor googletest for offline test builds
```

### Step 3: Add the root CMake smoke target

**Objective**

Prove that CMake can build and run GoogleTest before migrating any problem.

**Actions**

- Add the root `CMakeLists.txt`.
- Set the required C language standard for candidate code.
- Add the vendored GoogleTest subdirectory.
- Enable CTest.
- Add a small framework smoke test target.
- Register the smoke test with CTest.

**Verification**

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The smoke test must pass. No problem test needs to pass at this stage.

**Checkpoint**

Suggested commit:

```text
build: add cmake ctest and googletest smoke target
```

### Step 4: Add the standard CMake template

**Objective**

Create the default per-problem build contract.

**Actions**

- Add `cmake/templates/standard-problem.cmake`.
- Detect the problem directory from `CMAKE_CURRENT_SOURCE_DIR`.
- Derive target `prob_<folder_name>`.
- Collect all local `*.c` files.
- Compile only `test_runner.cpp` as the test runner.
- Add include paths, GoogleTest linking, CTest discovery, and the
  `problem` label.
- Add comments explaining each important section.

**Verification**

- The template can be included from a problem directory.
- The target name is derived without editing the problem name in the
  template.

**Checkpoint**

Suggested commit:

```text
build: add standard problem cmake template
```

### Step 5: Add the special CMake template

**Objective**

Provide a clear starting point for problems that do not fit the standard
source layout.

**Actions**

- Add `cmake/templates/special-problem.cmake`.
- Use explicit source lists.
- Comment the sections users are expected to customize.
- Document when a special manifest is appropriate.

**Verification**

- The template is readable and can be adapted without relying on hidden root
  behavior.

**Checkpoint**

Suggested commit:

```text
build: add special problem cmake template
```

### Step 6: Add automatic root problem-directory registration

**Objective**

Have the root CMake add every direct problem subdirectory without maintaining
an individual central registry.

**Actions**

- Add `CONFIGURE_DEPENDS` directory globbing for `problems/*`.
- Add each discovered directory with `add_subdirectory`.
- Do not add problem-content validation yet.
- Keep the assumption that problem authors follow the documented contract.

**Verification**

```bash
cmake -S . -B build
```

The current problem directories may still fail configuration until their
individual manifests are migrated. That transitional failure is expected.

**Checkpoint**

Suggested commit:

```text
build: register problem directories from root cmake
```

### Step 7: Migrate `self_check`

**Objective**

Validate the complete per-problem migration procedure with the smallest
existing test group.

**Actions**

1. Add the problem's `CMakeLists.txt` using the standard template.
2. Copy `test_runner.c` to `test_runner.cpp`.
3. Convert the copied runner to GoogleTest syntax.
4. Rename the original runner to `test_runner.c.old`.
5. Configure and build the new target.
6. Verify CTest discovery and execution.
7. Confirm the test passes.
8. Remove `test_runner.c.old` after successful verification.
9. Remove the old problem Makefile.

**Verification**

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build -R prob_self_check --output-on-failure
```

**Checkpoint**

Suggested commit:

```text
refactor: migrate self check to cmake and googletest
```

### Step 8: Migrate `swap_2_nums`

**Objective**

Validate migration of a normal C implementation and a simple test suite.

**Actions**

- Follow the exact runner-copy and `.old` rename procedure.
- Keep `swap_2_nums.c` and `swap_2_nums.h` unchanged except where required
  for build compatibility.
- Add the standard local manifest.
- Convert all tests while preserving their assertions.
- Verify the target and CTest registration.
- Remove the old runner and Makefile only after verification.

**Verification**

```bash
cmake --build build
ctest --test-dir build -R prob_swap_2_nums --output-on-failure
```

The test may fail because `swap_2_nums.c` is intentionally empty.

**Checkpoint**

Suggested commit:

```text
refactor: migrate swap problem to cmake and googletest
```

### Step 9: Migrate `check_power_of_2`

**Objective**

Validate a C API whose unfinished implementation currently aborts.

**Actions**

- Follow the runner-copy and `.old` rename procedure.
- Convert the tests to GoogleTest.
- Preserve the expected abort/failure behavior of the unfinished
  implementation.
- Remove legacy files only after the new target executes correctly.

**Verification**

```bash
cmake --build build
ctest --test-dir build -R prob_check_power_of_2 --output-on-failure
```

Compilation and test execution are the migration requirements; passing the
exercise assertions is not required.

**Checkpoint**

Suggested commit:

```text
refactor: migrate power of two problem to cmake and googletest
```

### Step 10: Migrate `endianess_detection`

**Objective**

Validate a problem that explicitly uses a shared implementation detail.

**Actions**

- Follow the runner-copy and `.old` rename procedure.
- Convert the test runner.
- Preserve the existing public spelling and API names.
- Use the special template or an explicit source addition as needed for the
  shared source and test linkage.
- Verify the unfinished implementation's failure is reported normally.
- Remove old files after verification.

**Verification**

```bash
cmake --build build
ctest --test-dir build -R prob_endianess_detection --output-on-failure
```

**Checkpoint**

Suggested commit:

```text
refactor: migrate endianess problem to cmake and googletest
```

### Step 11: Migrate `bit_manip`

**Objective**

Validate migration of tests currently split between a runner and a test
header.

**Actions**

- Copy `test_runner.c` to `test_runner.cpp`.
- Move or convert the test definitions from `test_bit_manip.h` into an
  appropriate C++ test source arrangement.
- Rename the original runner to `test_runner.c.old`.
- Ensure the old Acutest header is not included by the new runner.
- Preserve all eight test intentions.
- Remove old files only after the new target is verified.

**Verification**

```bash
cmake --build build
ctest --test-dir build -R prob_bit_manip --output-on-failure
```

The unfinished implementation may cause the exercise tests to fail.

**Checkpoint**

Suggested commit:

```text
refactor: migrate bit manipulation problem to cmake and googletest
```

### Step 12: Migrate `memcpy`

**Objective**

Validate a problem with an implementation-specific filename and shared test
utility source.

**Actions**

- Preserve `my_memcpy.c` and `my_memcpy.h`.
- Copy the test runner to `test_runner.cpp`.
- Rename the original runner to `test_runner.c.old`.
- Convert memory comparisons and allocation tests to GoogleTest.
- Explicitly include `shared/utils.c` through the special manifest or another
  documented special configuration.
- Add any required standard headers exposed by the C++ conversion.
- Remove old files after verification.

**Verification**

```bash
cmake --build build
ctest --test-dir build -R prob_memcpy --output-on-failure
```

The unfinished implementation may cause the exercise tests to fail.

**Checkpoint**

Suggested commit:

```text
refactor: migrate memcpy problem to cmake and googletest
```

### Step 13: Migrate `memmove`

**Objective**

Validate migration of overlapping-memory tests and shared test utilities.

**Actions**

- Preserve `my_memmove.c` and `my_memmove.h`.
- Copy the runner to `test_runner.cpp`.
- Rename the original runner to `test_runner.c.old`.
- Convert overlap, boundary, identical-pointer, and string tests.
- Explicitly include `shared/utils.c`.
- Confirm the C++ conversion does not change pointer or allocation behavior.
- Remove old files after verification.

**Verification**

```bash
cmake --build build
ctest --test-dir build -R prob_memmove --output-on-failure
```

The unfinished implementation may cause the exercise tests to fail.

**Checkpoint**

Suggested commit:

```text
refactor: migrate memmove problem to cmake and googletest
```

### Step 14: Verify the complete new workflow

**Objective**

Confirm that all problems are represented by the new framework.

**Actions**

- Remove any remaining `.old` runner files.
- Configure from a clean build directory.
- Build all targets.
- List all registered tests.
- Run the framework smoke test.
- Run all problem tests and record expected exercise failures separately.
- Confirm every problem target has the expected `prob_<folder_name>` naming.

**Verification**

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build -N
ctest --test-dir build -L framework --output-on-failure
ctest --test-dir build -L problem --output-on-failure
```

The framework label must pass. Problem failures caused by unfinished
implementations are expected.

**Checkpoint**

Suggested commit:

```text
test: verify complete cmake and ctest workflow
```

### Step 15: Clean remaining legacy workflow files

**Objective**

Remove legacy infrastructure only after all problems use the new workflow.

**Actions**

- Confirm no migrated target depends on Acutest.
- Confirm no migrated target depends on `shared/common.mk`.
- Remove `shared/acutest.h`.
- Remove `shared/common.mk`.
- Remove obsolete Acutest extension modifications and notes.
- Remove old Acutest-specific VS Code settings and tasks.
- Remove any remaining problem Makefiles and `.old` runner files.
- Update only directly affected documentation so it does not instruct users
  to use deleted files.

**Verification**

- Search the repository for remaining Acutest, old Makefile, and old runner
  references.
- Configure and build from a clean `build/` directory.
- Run the framework smoke test.
- Confirm no new workflow target references deleted files.

**Checkpoint**

Suggested commit:

```text
refactor: remove legacy make and acutest workflow
```

## 13. Final Acceptance Criteria

The migration is complete when:

- a clean Dev Container provides GCC, Make, CMake, and CTest;
- GoogleTest is available offline from the repository;
- CMake configures without downloading dependencies;
- the framework smoke test passes;
- all current problem directories are registered automatically;
- each standard problem target is named `prob_<folder_name>`;
- standard problems compile `test_runner.cpp` and all local `.c` files;
- special problems can explicitly control their source lists;
- candidate implementation files remain C;
- test runners use GoogleTest C++;
- CTest discovers individual tests;
- the framework does not require unfinished exercises to pass;
- old files are removed only after their replacements are verified; and
- the documented new workflow can be used from a clean build directory.

## 14. Future Features, Ordered by Importance

The following features are intentionally deferred. They should be revisited in
the order below so the framework remains useful before additional complexity
is introduced.

### Priority 1: Important framework reliability

1. Add validation for malformed problem directories.
2. Add a scaffolding command that creates a complete problem directory from
   the standard template.
3. Make the scaffolding command update or coordinate root registration if
   manual registration still exists.
4. Add framework tests for the scaffolding command and template output.
5. Add clear problem status metadata so expected unfinished failures are
   distinguishable from infrastructure regressions.
6. Add a single documented command for configuring, building, and listing
   tests.
7. Improve dependency tracking and warning flags for C and C++ targets.

### Priority 2: Important development experience

1. Integrate CTest with a supported VS Code test interface.
2. Add automatic test refresh after a successful build.
3. Improve error messages for missing `CMakeLists.txt`,
   `test_runner.cpp`, or implementation sources.
4. Add optional AddressSanitizer and UndefinedBehaviorSanitizer support.
5. Add CI that validates framework functionality without requiring exercise
   implementations to pass.
6. Add reproducible dependency update instructions for vendored GoogleTest.

### Priority 3: Future exercise support

1. Define a supported `shared/support/` layout for reusable utilities.
2. Define a separate, explicit policy for optional reference implementations.
3. Add future exercises such as ring buffers, producer/consumer systems, and
   static memory pools.
4. Add special templates for exercises needing multiple libraries,
   platform-specific sources, or generated data.
5. Consider GoogleMock only if future exercises genuinely require mocking.

### Priority 4: Cross-platform and maintenance improvements

1. Validate the workflow on Linux, macOS, Windows/WSL, and Codespaces.
2. Pin or document Dev Container image versions and update policy.
3. Add line-ending policy and cross-platform shell guidance.
4. Improve full project documentation after the new workflow stabilizes.
5. Consider a compatibility strategy for the public spelling of
   `endianess_detection`.
6. Optimize the vendored GoogleTest tree if repository size becomes a
   practical concern.

## 15. Risks and Rollback Guidance

### Risk: test conversion changes behavior

Compare converted test names, assertions, inputs, and expected results with
the original Acutest tests. Do not repair exercise implementations as part of
test conversion.

### Risk: automatic C source collection includes an unintended file

Rename the old runner to `test_runner.c.old` before configuring the new target.
Use the special template for any problem that cannot obey the standard
directory contract.

### Risk: mixed build systems become confusing

During migration, clearly identify which problems have moved and which still
use legacy files. Keep shared legacy infrastructure until no remaining
problem depends on it.

### Risk: vendored dependency is incomplete

Build the smoke target from a clean directory before migrating any problem.
Preserve the complete upstream licensing and source files needed by GoogleTest.

### Rollback

Each migration step is independently reviewable and checkpointed. If a step
fails, stop at that checkpoint, inspect the target-specific changes, and
restore the previous checkpoint without reverting unrelated user work.
