# low-level-interview-prep

## Purpose
This repo is used to practice interview problems relating to Low-Level / Embedded or Firmware Engineer positions. Focus on problems that are not easily found and not test-ready on popular coding platforms.

## Workflow

### Practice
- Implement.
- Build: `Ctrl + Shift + B` or [VS Code] Terminal > Run Task... > Build all problems.
- Testing (left side panel): click Refresh Tests > click on the tests for each problem to run.

### Add New Test Group
- Create new test group folder, says `X`, under `problems` directory. Provide as many source files as needed in here.
- Must have a C file to to include `TEST_LIST` macro, which include all unit tests to run. The name of this file, says `X1`, should also be the same as in other test groups (default is `test_runner.c`).
- Decide on the name of the final executable file. The name of this file, says `X2`, should also be the same as in other test groups (default is `test.out`).
- Create a `Makefile` and set up building accordingly. Make sure the final output executable file is `X2`.
- Open `.vscode/settings.json` to include the new test group `X`, as well as setting the names of the test source `X1` and executable `X2` files accordingly.

Hint: refer the `self_check` folder for a sample test group.