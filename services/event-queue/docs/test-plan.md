# Event Queue — Test Plan

Team 4 · Related Issue: #9 · Requirements: `docs/requirements.md`

## 1. Approach

- Tests are executed manually through the console menu.
- Build command: `g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app`
- Test T-10 (full queue) uses input redirection from a prepared text file: `./app < tests/input-full-queue.txt`
- Actual results and status (PASS / FAIL) are recorded in `tests/test-cases.md` after implementation. A failed test is linked to a bug Issue.

## 2. Test Scenarios

| ID | Purpose | Input (menu commands and values) | Expected result |
|---|---|---|---|
| T-01 | Typical scenario: priority order (CE-1) | Add (1, p2, src, msg1), (2, p3, src, msg2), (3, p3, src, msg3); option 3 three times | Processed in order 2, 3, 1; then `Queue is empty.` on option 2 |
| T-02 | Minimum and boundary values of priority | Add (1, p1); add (2, p3); add (3, p0); add (4, p4) | Events 1 and 2 added; p0 and p4 rejected with `Invalid priority: enter 1, 2 or 3.`; `eventCount` = 2 |
| T-03 | Invalid type (non-numeric input) | Menu `abc`; option 1 with id `abc`; option 1 with priority `abc` | Each input rejected with the matching error message; program returns to menu; queue unchanged |
| T-04 | Invalid id range | Add event with id `0`; add event with id `-5` | Both rejected: `Invalid id: enter a positive integer.`; `eventCount` = 0 |
| T-05 | Empty text fields | Add event with empty `source`; add event with `message` of only spaces | Rejected: `Source must not be empty.` / `Message must not be empty.`; queue unchanged |
| T-06 | Special risk: duplicate id | Add (1, p2); add (1, p3) | Second event rejected: `Event with id 1 already exists.`; first event unchanged; `eventCount` = 1 |
| T-07 | Special risk: empty collection | On an empty queue: option 3, option 4 with id 1, option 5 | `Queue is empty. Nothing to process.`; `Queue is empty.`; statistics all 0, free places 100; no crash, no division |
| T-08 | Equal priority uses arrival order (CE-2) | Add (5, p1), then (2, p1); option 3 | Event 5 is processed first |
| T-09 | Remove by id | Add (1, p1), (2, p1), (3, p1); remove id 2; option 2; remove id 9 | After removal queue shows 1, 3 in this order; `Event with id 9 not found.`; queue unchanged |
| T-10 | Maximum number of records | Add 100 valid events (ids 1–100); try to add event 101; option 5 | 101st event rejected: `Queue is full (100 events). Event not added.`; statistics: total 100, free places 0 |

## 3. Coverage of Mandatory Categories

| Category (specification, section 13) | Test |
|---|---|
| Typical correct scenario | T-01 |
| Minimum allowed value | T-02 |
| Exact boundary value | T-02, T-10 |
| Invalid type or range | T-03, T-04 |
| Special risk of the service (duplicate id, missing element, empty collection) | T-06, T-07, T-09 |

At least two of the selected tests are negative or boundary (T-02, T-04, T-06, T-07).
