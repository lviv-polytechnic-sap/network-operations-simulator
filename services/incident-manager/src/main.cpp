#include <iostream>
#include <limits>
#include <string>

const int MAX_INCIDENTS = 100;

struct Incident {
    int id;
    std::string source;
    std::string category;
    std::string severity;
    bool resolved;
};

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isDuplicateId(const Incident incidents[], int count, int id) {
    for (int i = 0; i < count; ++i) {
        if (incidents[i].id == id) {
            return true;
        }
    }
    return false;
}

bool isValidCategory(const std::string& category) {
    return category == "SIGNAL" ||
           category == "OVERLOAD" ||
           category == "DEVICE_OFFLINE" ||
           category == "DATA_ERROR";
}

bool isValidSeverity(const std::string& severity) {
    return severity == "INFO" ||
           severity == "WARNING" ||
           severity == "CRITICAL";
}

bool validateIncident(const Incident& inc, const Incident incidents[], int count) {
    if (inc.id <= 0) {
        std::cout << "Validation error: ID must be a positive integer.\n";
        return false;
    }
    if (isDuplicateId(incidents, count, inc.id)) {
        std::cout << "Validation error: Incident with this ID already exists.\n";
        return false;
    }
    if (inc.source.empty()) {
        std::cout << "Validation error: Source cannot be empty.\n";
        return false;
    }
    if (!isValidCategory(inc.category)) {
        std::cout << "Validation error: Invalid category. Allowed: SIGNAL, OVERLOAD, DEVICE_OFFLINE, DATA_ERROR.\n";
        return false;
    }
    if (!isValidSeverity(inc.severity)) {
        std::cout << "Validation error: Invalid severity. Allowed: INFO, WARNING, CRITICAL.\n";
        return false;
    }
    return true;
}

bool readIncident(Incident& inc) {
    std::cout << "Enter incident ID (integer > 0): ";
    if (!(std::cin >> inc.id)) {
        clearInputBuffer();
        std::cout << "Input error: Expected an integer for ID.\n";
        return false;
    }
    clearInputBuffer();

    std::cout << "Enter source (e.g., router-1): ";
    std::getline(std::cin, inc.source);

    std::cout << "Enter category (SIGNAL / OVERLOAD / DEVICE_OFFLINE / DATA_ERROR): ";
    std::cin >> inc.category;
    clearInputBuffer();

    std::cout << "Enter severity (INFO / WARNING / CRITICAL): ";
    std::cin >> inc.severity;
    clearInputBuffer();

    inc.resolved = false;
    return true;
}

int countOpenWarningsBySource(const Incident incidents[], int count, const std::string& source) {
    int openWarnings = 0;
    for (int i = 0; i < count; ++i) {
        if (!incidents[i].resolved &&
            incidents[i].severity == "WARNING" &&
            incidents[i].source == source) {
            ++openWarnings;
        }
    }
    return openWarnings;
}

void showOpenIncidents(const Incident incidents[], int count) {
    int openCount = 0;
    std::cout << "\n--- Open Incidents ---\n";
    for (int i = 0; i < count; ++i) {
        if (!incidents[i].resolved) {
            std::cout << "ID: " << incidents[i].id
                      << " | Source: " << incidents[i].source
                      << " | Category: " << incidents[i].category
                      << " | Severity: " << incidents[i].severity << "\n";
            ++openCount;
        }
    }

    if (openCount == 0) {
        std::cout << "No open incidents found.\n";
    }
    std::cout << "Open incidents: " << openCount << " | Total incidents: " << count << "\n";
}

void closeIncidentById(Incident incidents[], int count) {
    if (count == 0) {
        std::cout << "No incidents available to close.\n";
        return;
    }

    int targetId;
    std::cout << "Enter incident ID to close: ";
    if (!(std::cin >> targetId)) {
        clearInputBuffer();
        std::cout << "Input error: Expected an integer for ID.\n";
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < count; ++i) {
        if (incidents[i].id == targetId) {
            if (incidents[i].resolved) {
                std::cout << "Incident ID " << targetId << " is already closed.\n";
            } else {
                incidents[i].resolved = true;
                std::cout << "Incident ID " << targetId << " has been closed.\n";
            }
            return;
        }
    }

    std::cout << "Incident with ID " << targetId << " not found.\n";
}

int main() {
    Incident incidents[MAX_INCIDENTS];
    int incidentCount = 0;
    int choice = -1;

    while (true) {
        std::cout << "\n=== INCIDENT MANAGER ===\n\n";
        std::cout << "1. Add new incident\n";
        std::cout << "2. Show open incidents\n";
        std::cout << "3. Close incident by ID\n";
        std::cout << "0. Exit\n\n";
        std::cout << "Select an option: ";

        if (!(std::cin >> choice)) {
            clearInputBuffer();
            std::cout << "Input error: Please enter 1, 2, 3, or 0.\n";
            continue;
        }
        clearInputBuffer();

        if (choice == 0) {
            std::cout << "Exiting Incident Manager. Total incidents stored: "
                      << incidentCount << ".\n";
            break;
        } else if (choice == 1) {
            if (incidentCount >= MAX_INCIDENTS) {
                std::cout << "Storage error: Maximum limit of "
                          << MAX_INCIDENTS << " incidents reached.\n";
                continue;
            }

            Incident tempIncident;
            if (readIncident(tempIncident)) {
                if (validateIncident(tempIncident, incidents, incidentCount)) {
                    incidents[incidentCount] = tempIncident;
                    ++incidentCount;

                    if (incidents[incidentCount - 1].severity == "WARNING") {
                        int openWarnings = countOpenWarningsBySource(incidents, incidentCount, incidents[incidentCount - 1].source);
                        if (openWarnings == 3) {
                            incidents[incidentCount - 1].severity = "CRITICAL";
                            std::cout << "Escalation: 3rd open WARNING for source '"
                                      << incidents[incidentCount - 1].source
                                      << "'. Severity automatically upgraded to CRITICAL.\n";
                        }
                    }

                    std::cout << "Incident validated and added successfully (total: "
                              << incidentCount << ").\n";
                }
            }
        } else if (choice == 2) {
            showOpenIncidents(incidents, incidentCount);
        } else if (choice == 3) {
            closeIncidentById(incidents, incidentCount);
        } else {
            std::cout << "Invalid option. Please enter 1, 2, 3, or 0.\n";
        }
    }

    return 0;
}