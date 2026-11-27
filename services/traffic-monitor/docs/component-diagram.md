
# Component Diagram — Traffic Monitor

## 1. System Overview

Traffic Monitor is a system component responsible for monitoring network node loads and analyzing available capacity.

## 2. Main Components

### Input Component
Receives network node data from the user.

### Validation Component
Checks whether capacity and current load values are valid.

### Processing Component
Calculates load percentage, classifies network load, finds minimum and maximum load, and calculates free capacity.

### State Component
Stores information about network nodes and their current load.

### Report Component
Displays load percentages, node statuses, minimum and maximum load, and free capacity.

### Main Component
Controls the program flow and connects all components.

## 3. Component Interactions

1. Main sends a request to Input to receive node data.
2. Input sends node data to Validation.
3. Validation checks the data and returns valid nodes.
4. State stores the validated network node data.
5. Processing receives node data from State.
6. Processing calculates load percentage, status, minimum load, maximum load, and free capacity.
7. Processing sends analysis results to Report.
8. Report displays the results to the user.

## 4. Analysis Functions

- calculateLoadPercent() — calculates load percentage.
- classifyLoad() — determines the load status.
- findMinLoad() — finds the node with the lowest load percentage.
- findMaxLoad() — finds the node with the highest load percentage.
- calculateFreeCapacity() — calculates available capacity.

## 5. Design Assumptions

Load classification thresholds:
- NORMAL: below 70%.
- HIGH: from 70% to below 90%.
- OVERLOADED: from 90% to 100%.

Invalid data:
- Capacity must be greater than zero.
- Current load cannot be negative.
- Current load cannot exceed capacity.
