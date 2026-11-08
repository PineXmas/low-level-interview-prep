# low-level-interview-prep

## Purpose
This repo is used to practice interview problems relating to Low-Level / Embedded or Firmware Engineer positions. Focus on problems that are not easily found and not test-ready on popular coding platforms.

## Workflow

### Practice
- Implement.
- Configure and build: `cmake -S . -B build && cmake --build build`.
- Run tests with `ctest --test-dir build --output-on-failure`.

### Add New Test Group
- Create a problem directory under `problems/` using the `NNN_problem_name`
  convention.
- Add the public C header, candidate-owned C implementation, C++ test runner,
  and a `CMakeLists.txt` using the standard or special CMake template.
- Reconfigure and build from the repository root. The root CMake file discovers
  the new problem automatically.

The [project guide](DOC.md) contains the complete build and test workflow.