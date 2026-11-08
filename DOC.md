# Low-Level Interview Prep: Project and Framework Guide

This repository is a practice framework for low-level, embedded, and firmware
interview problems. Implementations under `problems/` are intentionally
incomplete; framework checks and exercise assertions are separate concerns.

## Repository structure

```text
.
|-- .devcontainer/       # Dev Container configuration
|-- infra/                # CMake templates, support code, and GoogleTest
|-- problems/             # Independent exercise directories
|-- .vscode/settings.json # CMake/CTest editor settings
|-- CMakeLists.txt        # Root project and test discovery
`-- README.md
```

Each migrated problem has a `CMakeLists.txt`, a C++ `test_runner.cpp`, and
candidate-owned C implementations. Standard problems use
`infra/cmake/templates/standard-problem.cmake`, which automatically compiles
all local `.c` files. Special problems provide an explicit source list with
`infra/cmake/templates/special-problem.cmake`.

Problem targets are named `prob_<directory-name>`. CTest test names are
prefixed with the problem directory name, so individual tests can be selected
without running every exercise.

## Build and test

From the repository root, configure and build a clean tree:

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

List discovered tests:

```bash
ctest --test-dir build -N
```

Run the framework smoke test:

```bash
ctest --test-dir build -L framework --output-on-failure
```

Run the example integration test:

```bash
ctest --test-dir build -R '^000_example/' --output-on-failure
```

Exercise implementations are intentionally unfinished, so their tests may
fail. A build or framework smoke-test failure is a framework problem; an
assertion failure in an unfinished exercise is expected practice feedback.

## Adding a problem

1. Create a directory under `problems/` using the `NNN_problem_name`
   convention.
2. Add the public C header, candidate-owned C implementation, and
   `test_runner.cpp`.
3. Add a `CMakeLists.txt` that includes the standard or special template.
4. Configure and build from the repository root.

The root `infra/CMakeLists.txt` discovers problem directories automatically
when they contain a `CMakeLists.txt`; no central test-registration edit is
needed.

## Development environment

The supported environment is the Ubuntu 24.04 C/C++ Dev Container configured
in [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json). The
repository vendors GoogleTest under `infra/third_party/googletest`, so CMake
does not download dependencies during configuration.
