# Event Queue — Requirements

## 1. Purpose

The Event Queue service is a console application written in C++17. Its purpose is to simulate the storage and processing of events according to their priority and arrival order.

The service is an educational simulation. It does not use real telecommunications protocols, external networks, databases, or communication between services.

## 2. Data Model

Each event is represented by an `Event` structure with the following fields:

- `id` — unique event identifier.
- `priority` — event priority, from 1 to 3.
- `source` — source of the event.
- `message` — description of the event.

Events must be stored in a fixed-size array with a maximum capacity of 100 events.

## 3. Functional Requirements

### FR-1: Add an event

The user can add a new event to the queue.

The application must validate the event before adding it. An event must not be added if its identifier is already in use or its data is invalid.

The application must not exceed the maximum capacity of 100 events.

### FR-2: Display the queue

The user can display the current queue of events.

Events must be displayed in the order determined by the agreed queue-processing rules.

### FR-3: Process the next event

The user can process the next event in the queue.

The event with the highest priority must be processed first. Priority 3 is the highest, followed by priority 2 and priority 1.

If multiple events have the same priority, the event that arrived first must be processed first.

### FR-4: Remove an event by ID

The user can remove an event by specifying its identifier.

The application must handle attempts to remove an event that does not exist.

### FR-5: Display statistics

The user can request statistics about the events in the queue.

The exact statistics to be displayed must be agreed upon by the team before implementation.

## 4. Validation Rules

- Event identifiers must be unique.
- Event priority must be an integer from 1 to 3.
- The queue must not contain more than 100 events.
- Invalid input must be detected before it changes the application state.
- The program must not access elements outside the array boundaries.
- Empty-queue situations must be handled safely.
- The application must provide understandable messages for invalid input and unsuccessful operations.

Additional validation rules for event identifiers, sources, and messages must be clarified by the team.

## 5. Technical Constraints

- The application must use C++17.
- Only the standard C++ library may be used for the main algorithm.
- Events must be stored in a fixed-size array.
- The next event must be selected by a linear scan of the array.
- Linked lists and `priority_queue` must not be used.
- The program must compile locally with the following command:

`g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o app`

## 6. Expected Output

The application must provide:

- confirmation or an error message after an operation;
- a readable representation of the current queue;
- information about the event processed or removed;
- the requested statistics.

The exact format of the output will be defined during implementation.

## 7. Unclear Points to Clarify

The following details must be agreed upon by the team before implementation:

1. What exact statistics should the application display?
2. What formats are allowed for event identifiers?
3. Can `source` or `message` be empty, and should they have length limits?
4. What should happen when the user attempts to process an empty queue?
5. What should happen when the user attempts to remove an event that does not exist?
6. Should the queue display events in processing order or in their original arrival order?

These decisions must be reflected in the final requirements, data model, algorithm flowchart, implementation, and tests.

## 8. Acceptance Criteria

- All functional requirements are documented.
- The data model and validation rules are defined.
- Technical constraints are documented.
- Unclear requirements are recorded for team clarification.
- The requirements are reviewed by the team before implementation.
