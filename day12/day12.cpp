#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

constexpr inline bool canGo(const vector<string> &graph,
                            const vector<vector<size_t>> &dist,
                            size_t x, size_t y, size_t xTo, size_t yTo) {
    return (xTo >= 0 && xTo < graph.size() && yTo >= 0 && yTo < graph[x].size() && graph[x][y] + 1 >= graph[xTo][yTo]
            && dist[xTo][yTo] > dist[x][y] + 1);
}

void traverse(const vector<string> &graph,
              vector<vector<size_t>> &dist,
              size_t steps, size_t x, size_t y) {
    dist[x][y] = steps;
    if (canGo(graph, dist, x, y, x - 1, y)) {
        traverse(graph, dist, steps + 1, x - 1, y);
    }

    if (canGo(graph, dist, x, y, x, y - 1)) {
        traverse(graph, dist, steps + 1, x, y - 1);
    }

    if (canGo(graph, dist, x, y, x + 1, y)) {
        traverse(graph, dist, steps + 1, x + 1, y);
    }

    if (canGo(graph, dist, x, y, x, y + 1)) {
        traverse(graph, dist, steps + 1, x, y + 1);
    }
}

int main() {
    ifstream infile("day12/day12.in");
    string line;
    vector<string> map;
    vector<vector<size_t>> dist;
    pair<size_t, size_t> start(-1, -1);
    pair<size_t, size_t> end(-1, -1);
    int i = 0;
    while (getline(infile, line)) {
        if (start.first == -1 && line.contains('S')) {
            start.first = i;
            start.second = line.find('S');
            line[start.second] = 'a';
        }
        if (end.first == -1 && line.contains('E')) {
            end.first = i;
            end.second = line.find('E');
            line[end.second] = 'z';
        }
        map.push_back(line);
        dist.emplace_back(line.size(), INT_MAX);
        ++i;
    }

    dist[start.first][start.second] = 0;
    traverse(map, dist, 0, start.first, start.second);
    cout << dist[end.first][end.second] << endl;

    for(i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == 'a') {
                traverse(map, dist, 0, i, j);
            }
        }
    }
    cout << dist[end.first][end.second] << endl;
}
