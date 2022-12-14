#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <utility>
#include <chrono>

using namespace std;
using namespace std::chrono;
int steps = 26;
unordered_map<string, int> flows;
unordered_map<string, vector<string>> graph;
unordered_map<string, unordered_map<string, int>> paths;
vector<string> valves;
vector<string> valvesWithPositiveFlow;
unordered_map<string, unordered_map<int, unordered_map<size_t, int>>> visited;
unordered_map<string, int> ordering;

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

int traverse(const string &valve, int time, size_t state) {
    if (visited[valve][time][state] != 0) return visited[valve][time][state];
    int ans = 0;
    if (time <= 2) return 0;
    for (const auto &to: valvesWithPositiveFlow) {
        if (to == valve || ((state >> ordering[to]) & 1U) == 1 || flows[to] == 0) continue;
        int l = paths[valve][to];
        int timeLeft = time - l - 1;
        int pts = flows[to] * timeLeft;
        if (time <= l + 1) continue;
        state |= 1UL << ordering[to];
        ans = max(ans, pts + traverse(to, timeLeft, state));
        state &= ~(1UL << ordering[to]);
    }
    visited[valve][time][state] = ans;
    return ans;
}

int traverseWithHelp(const string &valve, int time, size_t state) {
    int ans = 0;
    if (time > 2) {
        for (const auto &to: valvesWithPositiveFlow) {
            if (to == valve || ((state >> ordering[to]) & 1U) == 1 || flows[to] == 0) continue;
            int l = paths[valve][to];
            int timeLeft = time - l - 1;
            if (timeLeft <= 0) continue;
            int pts = flows[to] * timeLeft;
            state |= 1UL << ordering[to];
            ans = max(ans, pts + traverseWithHelp(to, timeLeft, state));
            state &= ~(1UL << ordering[to]);
        }
    }
    if (time < 15) ans = max(ans, traverse("AA", steps, state));
    return ans;
}

int main() {
    ifstream infile("day16/day16.in");
    string line;
    int z = 0;
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
        if (flowRate > 0) {
            valvesWithPositiveFlow.push_back(valveName);
            ordering[valveName] = z++;
        }
        valves.push_back(valveName);
    }

    floyd();
    auto start1 = high_resolution_clock::now();
    cout << "Answer1: " << traverse("AA", 30, 0) << endl;
    auto stop1 = high_resolution_clock::now();
    cout << "answer1 took " << duration_cast<milliseconds>(stop1 - start1).count() << "ms" << endl;
    auto start2 = high_resolution_clock::now();

    cout << "Answer2: " << traverseWithHelp("AA", steps, 0) << endl;
    auto stop2 = high_resolution_clock::now();
    cout << "answer2 took " << duration_cast<milliseconds>(stop2 - start2).count() << "ms" << endl;
}
