#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <utility>

using namespace std;
unordered_map<string, int> flows;
unordered_map<string, vector<string>> graph;
unordered_map<string, unordered_map<string, int>> paths;
unordered_map<string, bool> isOpen;
vector<string> valves;
int positiveFlowValvesCnt = 0;

void floyd() {
    for (int i = 0; i < valves.size(); ++i) {
        for (int j = 0; j < valves.size(); ++j) {
            int path = i == j ? 0 : 100000;
            paths[valves[i]][valves[j]] = path;
            paths[valves[j]][valves[i]] = path;
        }
    }

    for (const auto &v: graph) {
        for (const auto &u: v.second) {
            paths[v.first][u] = 1;
        }
    }

    for (int i = 0; i < valves.size(); ++i) {
        for (int j = 0; j < valves.size(); ++j) {
            for (int k = 0; k < valves.size(); ++k) {
                paths[valves[j]][valves[k]] = min(paths[valves[j]][valves[k]],
                                                  paths[valves[j]][valves[i]] + paths[valves[i]][valves[k]]);
            }
        }
    }
}

uint64_t traverse(const string &valve, int time) {
    uint64_t ans = 0;
    for (const auto &to: paths[valve]) {
        if (flows[to.first] > 0 && !isOpen[to.first] && time >= to.second + 1) {
            isOpen[to.first] = true;
            ans = max(ans, flows[to.first] * (time - to.second - 1) + traverse(to.first, time - to.second - 1));
            isOpen[to.first] = false;
        }
    }
    return ans;
}

//struct Task {
//    int timeLeft;
//    ::uint64_t curScore;
//    string valve;
//
//    Task(int timeLeft, uint64_t curScore, string valve) : timeLeft(timeLeft), curScore(curScore), valve(std::move(valve)) {}
//};
//
//uint64_t traverseWithHelp(const string& valve1, const string& valve2, int time) {
//}

int main() {
    ifstream infile("day16/day16.in");
    string line;
    while (getline(infile, line)) {
        string valveName = line.substr(6, 2);
        size_t offset = line.find('=') + 1;
        int flowRate = stoi(line.substr(offset, line.find(';') - offset));
        flows[valveName] = flowRate;
        offset = line.find("valve") + 6;
        vector<string> tunnels;
        if (line[offset] == ' ') offset++;
        while (offset < line.length()) {
            tunnels.push_back(line.substr(offset, 2));
            offset += 4;
        }
        graph[valveName] = tunnels;
        isOpen[valveName] = false;
        if (flowRate > 0) positiveFlowValvesCnt++;
        valves.push_back(valveName);
    }

    floyd();
    cout << "Answer1: " << traverse("AA", 30) << endl;
//    cout << "Answer2: " << traverseWithHelp("AA", "AA", 26) << endl;
}
