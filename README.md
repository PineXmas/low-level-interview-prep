# low-level-interview-prep

Practice problems for low-level, embedded, and firmware engineering
interviews. Exercise implementations are intentionally incomplete.

## Practice a problem

From the repository root:

```bash
cmake -S . -B build
cmake --build build --target prob_<problem-folder>
ctest --test-dir build -R '^<problem-folder>/' --output-on-failure
```

For example:

```bash
cmake --build build --target prob_000_example
ctest --test-dir build -R '^000_example/' --output-on-failure
```

Use `ctest --test-dir build -L framework --output-on-failure` to run the
framework smoke test. Failures in unfinished exercises are expected practice
feedback; framework failures are not.

## Add a problem

1. Create `problems/NNN_problem_name/`.
2. Add a public C header, candidate-owned C implementation, and
   `test_runner.cpp`.
3. Add a `CMakeLists.txt` that includes the standard or special template from
   [`infra/cmake/templates/`](infra/cmake/templates/).
4. Configure and build from the repository root. Problem directories with a
   `CMakeLists.txt` are discovered automatically.

See [`DOC.md`](DOC.md) for the technical architecture, build conventions, and
planned framework work.
