# Network Operations Simulator

An educational C++17 monorepository for eight student teams. Each team develops one independent console application that models a small network-operations task.

This is **not** a production telecom platform and not a microservice implementation. During the first two-week assignment, services do not communicate over a network and are not integrated. The assessed work is requirements analysis, algorithm design, C++ programming, testing, documentation, and disciplined collaboration through GitHub.

## Team ownership

| Team | Service directory | Educational responsibility |
|---|---|---|
| 1 | `services/device-registry` | Device records and validation |
| 2 | `services/signal-analyzer` | Signal measurements and classification |
| 3 | `services/traffic-monitor` | Traffic observations and threshold checks |
| 4 | `services/event-queue` | Event storage and queue processing |
| 5 | `services/incident-manager` | Incident records and prioritisation |
| 6 | `services/availability-monitor` | Availability measurements and summaries |
| 7 | `services/recovery-simulator` | Recovery actions and outcome simulation |
| 8 | `services/operations-report` | Aggregated operational reporting |

The exact requirements, data model, rules, thresholds, and test cases are intentionally left for the students to define with the instructor. A team may change only its own service directory unless the instructor approves a shared change.

## Required result for each team

Each service must contain:

- one complete C++17 console application using the standard library;
- a clear data model that matches the code;
- input validation and explicit handling of invalid data;
- four diagrams: context, component, main algorithm flowchart, and data model;
- at least ten documented test scenarios with actual results;
- a service README with build, run, usage, and design instructions;
- evidence of individual contribution through Issues, branches, commits, pull requests, and reviews.

The minimum local build command remains mandatory:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app
./app
```

Docker is optional and may earn up to two bonus points. It does not replace local compilation or correct algorithms.

## Workflow

1. Select an assigned Issue from the project board.
2. Create a short-lived branch, for example `team-3/issue-17-input-validation`.
3. Commit a focused change with a meaningful message.
4. Open a pull request and link the Issue with `Closes #...`.
5. Obtain at least one review and address the feedback.
6. Merge using the repository's selected merge method.

See [CONTRIBUTING.md](CONTRIBUTING.md) for the complete rules and [docs/instructor-guide.md](docs/instructor-guide.md) for the teaching plan, checkpoints, assessment rubric, and GitHub setup checklist.

## Repository layout

```text
network-operations-simulator/
├── .github/
│   ├── ISSUE_TEMPLATE/
│   └── pull_request_template.md
├── docs/
├── integration/
├── shared/sample-data/
└── services/
    ├── device-registry/
    ├── signal-analyzer/
    ├── traffic-monitor/
    ├── event-queue/
    ├── incident-manager/
    ├── availability-monitor/
    ├── recovery-simulator/
    └── operations-report/
```

