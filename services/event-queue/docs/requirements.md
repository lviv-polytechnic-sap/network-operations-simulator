# Event Queue — Requirements

Team 4 · Service directory: `services/event-queue/` · Related Issue: #1

## 1. Purpose

The Event Queue service is a console application written in C++17. Its purpose is to simulate the storage and processing of events according to their priority and arrival order.

The service is an educational simulation. It does not use real telecommunications protocols, external networks, databases, files, or communication between services.

## 2. Actors and External Data (context diagram)

| Direction | Data |
|---|---|
| User → Event Queue | event data (id, priority, source, message), menu command, event id for removal |
| Event Queue → User | queue state, processed event, statistics, error message |
| Services 1, 2, 3, 6 → Event Queue | event data — **future integration, not in v1.0.0** |

## 3. Data Model (data model diagram)

### 3.1 Structure `Event`

| Field | Type | Allowed values |
|---|---|---|
| `id` | `int` | greater than 0, unique within the queue |
| `priority` | `int` | 1, 2 or 3 (3 is the highest) |
| `source` | `std::string` | not empty |
| `message` | `std::string` | not empty |

### 3.2 Constants and state

| Name | Type | Meaning |
|---|---|---|
| `MAX_EVENTS` | `const int` = 100 | maximum number of events |
| `MIN_PRIORITY` | `const int` = 1 | lowest allowed priority |
| `MAX_PRIORITY` | `const int` = 3 | highest allowed priority |
| `queue[MAX_EVENTS]` | array of `Event` | events stored in arrival order |
| `eventCount` | `int`, 0 to `MAX_EVENTS` | number of events currently in the queue |

`queue` and `eventCount` are declared in `main` and passed to functions as parameters. No global variables are used.

### 3.3 Invariant

After every operation:

- `0 <= eventCount <= MAX_EVENTS`;
- elements `queue[0] … queue[eventCount - 1]` are valid events stored in arrival order;
- all ids in `queue[0] … queue[eventCount - 1]` are unique.

## 4. Functional Requirements

### FR-4.1: Add an event (menu option 1)

- If `eventCount == MAX_EVENTS`, the program prints `Queue is full (100 events). Event not added.` and does not read event data.
- Otherwise the program reads `id`, `priority`, `source`, `message`.
- The event is validated (section 6). A valid event is written to `queue[eventCount]` and `eventCount` is increased by 1. The program prints `Event <id> added.`
- An invalid event is rejected with an error message; the queue does not change.

### FR-4.2: Display the queue (menu option 2)

- The program prints all events in arrival order: position, id, priority, source, message, and the total number of events.
- If the queue is empty, the program prints `Queue is empty.`

### FR-4.3: Process the next event (menu option 3)

- `findNextEvent` performs a linear scan of `queue[0] … queue[eventCount - 1]` and returns the index of the event with the highest priority. If several events have the same priority, the event with the smallest index (the earliest arrival) is selected.
- `processNext` prints the selected event as the processed event, removes it from the array (section 5, rule 4) and decreases `eventCount` by 1.
- If the queue is empty, the program prints `Queue is empty. Nothing to process.` and the state does not change.

### FR-4.4: Remove an event by ID (menu option 4)

- The program reads an id and searches for it with a linear scan.
- If the event is found, it is removed (section 5, rule 4) and the program prints `Event <id> removed.`
- If it is not found, the program prints `Event with id <id> not found.` and the queue does not change.
- If the queue is empty, the program prints `Queue is empty.`

### FR-4.5: Display statistics (menu option 5)

The program prints:

- the total number of events in the queue (`eventCount`);
- the number of events with priority 3, 2 and 1;
- the number of free places (`MAX_EVENTS - eventCount`).

For an empty queue all counters are 0 and free places are 100. Statistics use only counting and subtraction, so division by zero is impossible.

### Menu option 0: Exit

The program ends.

## 5. Processing Rules

1. Priority 3 is processed first, then priority 2, then priority 1.
2. Events with the same priority are processed in arrival order (FIFO). The event `id` is **not** used to break ties.
3. Arrival order equals the array position: new events are appended at index `eventCount`, and removal shifts later elements left, so the relative order is preserved.
4. Removal of the element at index `k`: elements `queue[k + 1] … queue[eventCount - 1]` are shifted one position to the left, then `eventCount` is decreased by 1.
5. The next event is found by a linear scan. Linked lists, `priority_queue` and sorting are not used.

## 6. Validation Rules

Validation is performed before any change of the application state. Invalid input never changes `queue` or `eventCount`.

| Input | Rule | Error message |
|---|---|---|
| menu option | integer from 0 to 5 | `Invalid menu option. Enter a number from 0 to 5.` |
| `id` | integer greater than 0 | `Invalid id: enter a positive integer.` |
| `id` (add) | not already in the queue | `Event with id <id> already exists.` |
| `priority` | integer from 1 to 3 | `Invalid priority: enter 1, 2 or 3.` |
| `source` | not empty and not only spaces | `Source must not be empty.` |
| `message` | not empty and not only spaces | `Message must not be empty.` |
| queue size (add) | `eventCount < MAX_EVENTS` | `Queue is full (100 events). Event not added.` |

If a number is expected and the user enters text, the input stream is cleared, the rest of the line is discarded, the error message is printed, and the program returns to the menu. `source` and `message` are read as whole lines and may contain spaces.

## 7. Technical Constraints

- The application must use C++17 and only the standard C++ library.
- Events must be stored in a fixed-size array of `Event`; the array size is `MAX_EVENTS`.
- The program must not access elements outside the array boundaries.
- All limits (`MAX_EVENTS`, `MIN_PRIORITY`, `MAX_PRIORITY`) are stored as `const`.
- No global variables.
- The program is divided into functions matching the component diagram:

| Component | Functions |
|---|---|
| Input | `readEvent` |
| Validation | `validateEvent` |
| Processing | `enqueue`, `findNextEvent`, `processNext`, `removeById` |
| State | `queue`, `eventCount` |
| Report | `printQueue`, `printStatistics` |
| Main | menu loop and calls of the functions above |

- The program must compile without errors and warnings with:

`g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app`

## 8. Clarified Decisions

The open questions from the first version were agreed as follows:

| # | Question | Decision |
|---|---|---|
| 1 | What statistics are displayed? | Total events, count per priority 3/2/1, free places (FR-4.5) |
| 2 | What format is allowed for ids? | Positive integer (`int > 0`), unique within the queue |
| 3 | Can `source` or `message` be empty? | No. Empty or space-only values are rejected; no length limit (`std::string`) |
| 4 | Processing an empty queue? | Message `Queue is empty. Nothing to process.`; state unchanged |
| 5 | Removing an id that does not exist? | Message `Event with id <id> not found.`; state unchanged |
| 6 | Display in processing or arrival order? | Arrival order (array order); processing order applies only to FR-4.3 |
| 7 | Tie-break for equal priority? | Arrival order (smallest array index), not the smaller id |
| 8 | Adding to a full queue? | Rejected with a message; state unchanged |

## 9. Future Integration (not implemented in v1.0.0)

Services 1, 2, 3 and 6 may later send events to the Event Queue as text records in the shared format `EVENT;id;type;severity;source`. The `severity` value would be mapped to `priority`. This contract is shown with dashed lines on the context diagram and is **not** implemented in the first assignment.

## 10. Acceptance Criteria

- All functional requirements FR-4.1 – FR-4.5 are documented with expected behaviour.
- The data model matches the `Event` structure and state in the data model diagram.
- Inputs, outputs and validation rules, including error behaviour, are described.
- All previously unclear points are resolved (section 8).
- The requirements are consistent with the context and component diagrams.
- The requirements are reviewed by another team member before implementation.


## 11. Control Examples

| ID | Input sequence | Expected result |
|---|---|---|
| CE-1 | Add (id=1, p=2), (id=2, p=3), (id=3, p=3); process next 3 times | Processing order: 2, 3, 1; queue is empty |
| CE-2 | Add (id=5, p=1), then (id=2, p=1); process next | Event 5 is processed first (arrival order, not smaller id) |
| CE-3 | Add (id=1, p=2); add (id=1, p=3) | Second event rejected: `Event with id 1 already exists.`; `eventCount` = 1 |
| CE-4 | Add event with priority 4 | Rejected: `Invalid priority: enter 1, 2 or 3.`; `eventCount` unchanged |
| CE-5 | Process next on an empty queue | `Queue is empty. Nothing to process.`; `eventCount` = 0 |
| CE-6 | Queue contains ids 1, 2; remove id 9 | `Event with id 9 not found.`; queue unchanged |
| CE-7 | After adding the three events of CE-1, show statistics | Total 3; priority 3: 2; priority 2: 1; priority 1: 0; free places: 97 |
| CE-8 | Enter `abc` as menu option | `Invalid menu option. Enter a number from 0 to 5.`; program returns to menu |

CE-1 is the control example from the project specification. CE-2 confirms the tie-break rule from section 5.
