
#include <iostream>
#include <cassert>
#include <vector>
#include "../src/analysis.cpp"

using namespace std;

int main() {
    NetworkNode node1{1, 100, 50};
    NetworkNode node2{2, 200, 140};
    NetworkNode node3{3, 100, 90};

    // Test 1
    assert(calculateLoadPercent(node1) == 50.0);

    // Test 2
    assert(classifyLoad(node1) == "NORMAL");

    // Test 3
    assert(classifyLoad(node2) == "HIGH");

    // Test 4
    assert(classifyLoad(node3) == "OVERLOADED");

    // Test 5
    vector<NetworkNode> nodes = {node1, node2, node3};
    assert(findMinLoad(nodes) == 1);

    // Test 6
    assert(findMaxLoad(nodes) == 2);

    // Test 7
    assert(calculateFreeCapacity(node1) == 50.0);

    cout << "All analysis tests passed!" << endl;

    return 0;
}
