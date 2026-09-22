# Contributing Guide

## Scope

This repository is used for assessed student work. Do not add networking, databases, third-party frameworks, distributed infrastructure, or unnecessary abstractions to the first assignment. Prefer one understandable executable and the C++ standard library.

## Before coding

1. Confirm that the task has an Issue, acceptance criteria, an assignee, and a `team-N` label.
2. Prepare or update the relevant diagram and pseudocode.
3. Agree on representative, boundary, and invalid examples.
4. Create a branch from the current `main` branch.

## Branches and commits

- Use a branch such as `team-2/issue-15-classification`.
- Keep each branch focused on one Issue.
- Use descriptive commits such as `Validate duplicate device identifiers`.
- Do not commit compiled binaries, editor state, secrets, or generated temporary files.
- Do not push directly to `main`.

## Pull requests

- Complete every section of the pull request template.
- Link the Issue with `Closes #<number>`.
- Include exact build and test commands.
- Request a review from another team member.
- The author must respond to review feedback before merging.
- Keep changes inside the assigned service directory. Changes to shared or root files require instructor approval.
- Use the single merge method selected by the instructor for the whole repository.

Every student must own at least one Issue and open at least one pull request. Roles coordinate work; they do not transfer all implementation or documentation to one person.

## C++ quality baseline

All submitted code must compile as C++17 with warnings enabled:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app
```

Before requesting review, test at least:

- minimum and maximum supported input;
- a typical scenario;
- invalid input;
- duplicate identifiers;
- every classification boundary;
- an empty data set;
- any calculation that could divide by zero;
- the maximum supported number of records.

The final flowchart, data model, and README must describe the code that actually exists.

## Diagrams

Commit both the editable `.drawio` source and an exported PDF. Data-flow arrows must be labelled. Decision diamonds must be questions with distinct Yes and No paths. Loops and invalid-input paths must be visible. The data model must match the C++ `struct` definitions.

## Optional Docker support

A team may add one small Dockerfile based on an official compatible GCC image. The build context must be the service directory. The image must compile with the required warning flags, and `docker run -it` must start the console program. Document exactly two commands—build and run—in the service README. Docker Compose, container networking, volumes, registries, and multi-stage builds are outside the assignment.

