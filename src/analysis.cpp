
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

struct NetworkNode {
    int id;
    double capacity;
    double currentLoad;
};

double calculateLoadPercent(const NetworkNode& node) {
    if (node.capacity <= 0 || node.currentLoad < 0 ||
        node.currentLoad > node.capacity) {
        throw invalid_argument("Invalid network node data");
    }

    return (node.currentLoad / node.capacity) * 100.0;
}

string classifyLoad(const NetworkNode& node) {
    double percent = calculateLoadPercent(node);

    if (percent < 70.0) {
        return "NORMAL";
    } else if (percent < 90.0) {
        return "HIGH";
    } else {
        return "OVERLOADED";
    }
}

int findMinLoad(const vector<NetworkNode>& nodes) {
    if (nodes.empty()) {
        throw invalid_argument("Node list is empty");
    }

    return min_element(nodes.begin(), nodes.end(),
        [](const NetworkNode& a, const NetworkNode& b) {
            return calculateLoadPercent(a) <
                   calculateLoadPercent(b);
        })->id;
}

int findMaxLoad(const vector<NetworkNode>& nodes) {
    if (nodes.empty()) {
        throw invalid_argument("Node list is empty");
    }

    return max_element(nodes.begin(), nodes.end(),
        [](const NetworkNode& a, const NetworkNode& b) {
            return calculateLoadPercent(a) <
                   calculateLoadPercent(b);
        })->id;
}

double calculateFreeCapacity(const NetworkNode& node) {
    if (node.capacity <= 0 || node.currentLoad < 0 ||
        node.currentLoad > node.capacity) {
        throw invalid_argument("Invalid network node data");
    }

    return node.capacity - node.currentLoad;
}
