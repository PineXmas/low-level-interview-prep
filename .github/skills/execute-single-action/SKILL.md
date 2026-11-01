---
name: execute-single-action
description: "Execute exactly one requested action with strict scope, inspection, verification, and reporting"
disable-model-invocation: true
---
Execute exactly one specific action from the user's request.

- Inspect the relevant files, context, and existing behavior before editing.
- Make only the changes necessary to complete the requested action.
- Preserve existing behavior unless the action explicitly requires changing it.
- Do not expand the scope or perform additional actions.
- Do not solve unrelated unfinished exercises or issues.
- Follow existing project patterns, conventions, and documentation.
- Run the smallest verification commands that prove the action's acceptance criteria.
- Distinguish framework, infrastructure, or tooling failures from expected task or test failures.
- Do not make unrelated documentation changes.
- Do not commit changes.
- Stop and report if the request is ambiguous.
- Stop and report if verification fails.
- Summarize the action taken, files changed, and verification results.
