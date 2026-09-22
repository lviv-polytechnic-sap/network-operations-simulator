# Instructor Guide

## 1. Teaching objective and constraints

The Network Operations Simulator is a two-week introductory team project for first-year distance-learning students. It consists of eight independent educational services in one monorepository. Require small, finished C++17 programs—not an imitation of an industrial microservice platform.

Explain at the start that:

- the services are simulations, not real telecommunications systems;
- algorithms, C++, design reasoning, testing, and team discipline are assessed;
- service-to-service networking is not required;
- diagrams and test examples come before implementation;
- “diagram as fiction” is unacceptable: final diagrams must match the program;
- one strong student cannot submit the work of four people;
- Docker is optional and cannot compensate for an incorrect algorithm.

## 2. Preparation before assigning the project

Prepare the repository and teaching environment:

- create one public repository named `network-operations-simulator`, if institutional rules permit public visibility;
- add all 32 students as collaborators or members of a teaching organization;
- assign four students to each service and record the assignment;
- replace all placeholder usernames in `CODEOWNERS`;
- configure branch protection and repository settings described below;
- create the labels and project board described below;
- announce checkpoint dates and the assessment rubric before work begins;
- prepare three to five private control inputs for each service;
- give a 20-minute demonstration of Issue → branch → commit → pull request → review → merge.

Do not provide implementations, complete algorithms, final data models, or finished test suites. Those are student deliverables.

## 3. GitHub configuration

Recommended repository settings:

| Setting | Recommendation |
|---|---|
| Visibility | Public, when institutional policy permits |
| Default branch | `main` |
| Pull requests | Required before merge |
| Approvals | At least one |
| Direct pushes to `main` | Prohibited for students |
| Force pushes | Prohibited |
| Protected-branch deletion | Prohibited |
| Merge method | Select either squash merge or merge commit and use it consistently |
| Issues | Enabled |
| Projects | One board, filterable by `team-N` |

Create labels `team-1` through `team-8`, plus `docs`, `code`, `test`, and `bug`.

Create one GitHub Project with these fields/statuses:

`Backlog` → `Ready` → `In progress` → `Review` → `Done`

## 4. Roles and rotation

| Student | Sprint 1 | Sprint 2 |
|---|---|---|
| A | Scrum Master | Quality Owner |
| B | Architecture Owner | Implementation Owner |
| C | Implementation Owner | Documentation Owner |
| D | Quality Owner | Scrum Master |

The Architecture Owner coordinates consistency but does not draw every diagram. The Implementation Owner integrates code but does not write every function. Every student must have an individual Issue and pull request.

## 5. Two-week checkpoints

| Day | Evidence shown by the team | Instructor decision |
|---|---|---|
| 2 | Backlog, roles, clarified requirements | Approve or return for clarification |
| 4 | Four draw.io diagrams and pseudocode | Architecture gate |
| 7 | Build, input, validation, basic scenario | MVP gate |
| 10 | Complete main algorithm and five tests | Quality gate |
| 13 | Ten tests, reviews, README, final diagrams | Release gate |
| 14 | Release tag and demonstration | Defence |

If a gate fails, record the missing acceptance criteria in the relevant Issues and keep those items out of `Done`.

## 6. Minimum backlog for every team

Create or ask each team to create these twelve Issues. Each Issue must have an owner, acceptance criteria, a `team-N` label, and a project status.

| # | Task |
|---|---|
| 1 | Clarify requirements and acceptance criteria |
| 2 | Describe the data model |
| 3 | Create the context diagram |
| 4 | Create the component diagram |
| 5 | Create the main algorithm flowchart |
| 6 | Implement input and validation |
| 7 | Implement the main algorithm |
| 8 | Implement reporting and the menu |
| 9 | Prepare ten test scenarios |
| 10 | Refactor the implementation |
| 11 | Complete the service README |
| 12 | Prepare the release and demonstration |

## 7. Diagram review checklist

- The context diagram contains only the system, actors, and external flows; do not mix internal functions into it.
- The component diagram communicates responsibilities rather than listing meaningless filenames.
- Every arrow names the data that moves along it.
- Every decision diamond in the main flowchart is phrased as a question.
- Yes and No lead to distinct paths.
- Every loop has an entry or continuation condition and an exit.
- Invalid input follows an explicit path.
- The data model matches the `struct` definitions in code.
- Both the final PDF and editable `.drawio` source are committed.

## 8. C++ review procedure

Run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app
./app
```

Then:

- run minimum, typical, and invalid scenarios;
- test the maximum number of records;
- test a duplicate identifier;
- test every classification boundary;
- test an empty set and every possible division by zero;
- compare the main algorithm with the flowchart;
- ask a randomly selected participant to explain one function;
- ask the team to show a pull request in which reviewer feedback was addressed.

## 9. Docker policy

Docker is an alternative way to run the application, not a required architecture. Local compilation remains mandatory. Do not require Docker Compose, networking, volumes, a CI registry, or a multi-stage build.

Accept a short Dockerfile when it:

- uses the official `gcc:14` image or a compatible official GCC image;
- uses only the service directory as its build context;
- compiles with the required flags;
- starts the console program with `docker run -it`;
- documents build and run commands in the service README.

Award no more than two bonus points, and never raise the total above 100.

## 10. Risks and instructor responses

| Risk | Response |
|---|---|
| One student does everything | Inspect Issues and pull requests; question every student individually |
| Copied finished code | Ask the student to change a rule or threshold live and explain the result |
| Over-engineered architecture | Return the team to one executable and the standard library |
| Diagram differs from code | Do not pass the release gate until both agree |
| Docker does not run | Assess the local project; Docker remains optional bonus work |
| Monorepository conflicts | Enforce changes within each team's directory |
| Team is late | Freeze optional features and finish the MVP |
| AI use without understanding | Defend a function, a test, and a specific commit orally |

## 11. Defence questions

1. Which invariant does the main loop maintain?
2. Where is the array or collection limit guaranteed?
3. Why does this function have these parameters?
4. What happens exactly at the threshold value?
5. Which test broke the first version?
6. Show a discrepancy found by a reviewer.
7. How did the diagram change after implementation?
8. What is stored in the `struct`, and why?
9. How are duplicate identifiers prevented?
10. Which data format could this service send to a neighbouring service?

## 12. Assessment rubric

| Criterion | Points | Evidence |
|---|---:|---|
| Requirements | 10 | Clarity, boundaries, acceptance criteria |
| Architecture | 20 | Four diagrams and correspondence with program structure |
| Implementation | 25 | Correct C++ and appropriate decomposition |
| Reliability | 10 | Validation and boundary cases |
| Tests | 15 | Ten scenarios with actual results |
| GitHub workflow | 10 | Issues, pull requests, reviews, individual contribution |
| Defence | 10 | Demonstration and answers |
| **Total** | **100** | Docker may recover up to two points but cannot exceed 100 |

## 13. Future integration sequence

Do not integrate services during the first two-week assignment. After the defence:

1. Agree on one text record format: `EVENT;id;type;severity;source`.
2. Teach services 2, 3, and 6 to export events to a file.
3. Teach service 4 to read the shared file.
4. Pass events to service 5.
5. Aggregate numeric results in service 8.
6. Introduce processes, sockets, HTTP, or container networking only in a later course.

## 14. Final instructor checklist

- [ ] Repository and eight service directories exist.
- [ ] All 32 students are added and divided into eight teams.
- [ ] `main` is protected.
- [ ] Issue and pull request templates work.
- [ ] Team labels exist and `CODEOWNERS` contains real usernames.
- [ ] The GitHub Flow demonstration has been delivered.
- [ ] Checkpoint dates have been announced.
- [ ] Docker rules have been explained.
- [ ] The assessment rubric was published before work began.
- [ ] Three to five private control inputs exist for each service.

