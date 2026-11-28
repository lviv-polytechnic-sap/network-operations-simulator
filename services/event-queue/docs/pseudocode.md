# Event Queue — Pseudocode

Team 4 · Related Issue: #5 · Flowchart: `docs/main-flowchart.pdf` · Requirements: `docs/requirements.md`

Names match the data model and component diagrams.

## Constants and state

```
CONST MAX_EVENTS   = 100
CONST MIN_PRIORITY = 1
CONST MAX_PRIORITY = 3

STRUCT Event
    id       : int      // > 0, unique
    priority : int      // MIN_PRIORITY .. MAX_PRIORITY
    source   : string   // not empty
    message  : string   // not empty
END STRUCT
```

## Main (menu loop)

```
FUNCTION main()
    queue[MAX_EVENTS] : Event
    eventCount ← 0
    REPEAT
        print menu: 1 Add, 2 Show queue, 3 Process next, 4 Remove by id, 5 Statistics, 0 Exit
        read option
        IF option is not an integer from 0 to 5 THEN
            print "Invalid menu option. Enter a number from 0 to 5."
            CONTINUE
        END IF
        SWITCH option
            CASE 1: addEvent(queue, eventCount)
            CASE 2: printQueue(queue, eventCount)
            CASE 3: processNext(queue, eventCount)
            CASE 4: removeById(queue, eventCount)
            CASE 5: printStatistics(queue, eventCount)
        END SWITCH
    UNTIL option = 0
END FUNCTION
```

## Add an event (FR-4.1)

```
FUNCTION addEvent(queue, eventCount)
    IF eventCount = MAX_EVENTS THEN
        print "Queue is full (100 events). Event not added."
        RETURN
    END IF
    event ← readEvent()
    IF validateEvent(event, queue, eventCount) THEN
        enqueue(queue, eventCount, event)
        print "Event <id> added."
    END IF                              // validateEvent prints the error message
END FUNCTION

FUNCTION validateEvent(event, queue, eventCount) → bool
    IF event.id ≤ 0 THEN print "Invalid id: enter a positive integer."; RETURN false
    IF event.priority < MIN_PRIORITY OR event.priority > MAX_PRIORITY THEN
        print "Invalid priority: enter 1, 2 or 3."; RETURN false
    IF event.source is empty or only spaces THEN print "Source must not be empty."; RETURN false
    IF event.message is empty or only spaces THEN print "Message must not be empty."; RETURN false
    FOR i ← 0 TO eventCount − 1
        IF queue[i].id = event.id THEN print "Event with id <id> already exists."; RETURN false
    END FOR
    RETURN true
END FUNCTION

FUNCTION enqueue(queue, eventCount, event)
    queue[eventCount] ← event           // safe: eventCount < MAX_EVENTS was checked
    eventCount ← eventCount + 1
END FUNCTION
```

## Process the next event (FR-4.3) — main algorithm

```
FUNCTION findNextEvent(queue, eventCount) → int
    IF eventCount = 0 THEN RETURN −1
    best ← 0
    FOR i ← 1 TO eventCount − 1
        IF queue[i].priority > queue[best].priority THEN   // strict ">" keeps the earliest event
            best ← i
        END IF
    END FOR
    RETURN best
END FUNCTION

FUNCTION processNext(queue, eventCount)
    best ← findNextEvent(queue, eventCount)
    IF best = −1 THEN
        print "Queue is empty. Nothing to process."
        RETURN
    END IF
    print queue[best] as processed event
    removeAt(queue, eventCount, best)
END FUNCTION

FUNCTION removeAt(queue, eventCount, k)
    FOR j ← k TO eventCount − 2
        queue[j] ← queue[j + 1]          // shift left, arrival order is preserved
    END FOR
    eventCount ← eventCount − 1
END FUNCTION
```

Loop invariant of `findNextEvent`: after each step, `queue[best]` has the highest priority among `queue[0 … i]`, and among equal priorities it has the smallest index (earliest arrival).

## Remove by id (FR-4.4)

```
FUNCTION removeById(queue, eventCount)
    IF eventCount = 0 THEN print "Queue is empty."; RETURN
    read id
    k ← −1
    FOR i ← 0 TO eventCount − 1
        IF queue[i].id = id THEN k ← i; BREAK
    END FOR
    IF k = −1 THEN
        print "Event with id <id> not found."
    ELSE
        removeAt(queue, eventCount, k)
        print "Event <id> removed."
    END IF
END FUNCTION
```

## Report (FR-4.2, FR-4.5)

```
FUNCTION printQueue(queue, eventCount)
    IF eventCount = 0 THEN print "Queue is empty."; RETURN
    FOR i ← 0 TO eventCount − 1
        print i + 1, queue[i].id, queue[i].priority, queue[i].source, queue[i].message
    END FOR
    print "Total: ", eventCount
END FUNCTION

FUNCTION printStatistics(queue, eventCount)
    count[1..3] ← 0
    FOR i ← 0 TO eventCount − 1
        count[queue[i].priority] ← count[queue[i].priority] + 1
    END FOR
    print "Total: ", eventCount
    print "Priority 3: ", count[3], "Priority 2: ", count[2], "Priority 1: ", count[1]
    print "Free places: ", MAX_EVENTS − eventCount      // no division, no division by zero
END FUNCTION
```
