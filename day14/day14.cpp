#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;
#define STONE '#'
#define WATER 'o'
#define EMPTY '.'

vector<vector<char>> graph(600, vector<char>(1500, EMPTY));
pair<int, int> cursor(0, 0);

void moveCursor(const pair<int, int> &dest) {
    int d = cursor.first < dest.first ? 1 : -1;
    while (cursor.first != dest.first) {
        graph[cursor.first][cursor.second] = STONE;
        cursor.first += d;
    }

    d = cursor.second < dest.second ? 1 : -1;
    while (cursor.second != dest.second) {
        graph[cursor.first][cursor.second] = STONE;
        cursor.second += d;
    }
    graph[cursor.first][cursor.second] = STONE;
}

int dfs(int y, int x) {
    if (y == graph.size() - 1) return 0;
    int ans = 0;
    if (graph[y + 1][x] == EMPTY) {
        ans += dfs(y + 1, x);
    }

    if (graph[y + 1][x] != EMPTY && graph[y + 1][x - 1] == EMPTY) {
        ans += dfs(y + 1, x - 1);
    }

    if (graph[y + 1][x] != EMPTY && graph[y + 1][x - 1] != EMPTY && graph[y + 1][x + 1] == EMPTY) {
        ans += dfs(y + 1, x + 1);
    }

    if (graph[y + 1][x] != EMPTY && graph[y + 1][x - 1] != EMPTY && graph[y + 1][x + 1] != EMPTY) {
        graph[y][x] = WATER;
        ans++;
    }
    return ans;
}

int main() {
    ifstream infile("day14/day14.in");
    string line;

    string delim = " -> ";
    int maxY = 0;
    while (getline(infile, line)) {
        auto start = 0U;
        auto end = line.find(delim);
        int i = 0;
        while (true) {
            pair<int, int> p;
            string token = line.substr(start, end - start);
            sscanf_s(line.c_str() + start, "%d,%d", &p.second, &p.first);
            p.second += 500;
            if (i++ == 0) cursor.swap(p); else moveCursor(p);
            maxY = max(maxY, p.first);
            start = end + delim.length();
            if (end == std::string::npos) break;
            end = line.find(delim, start);
        }
    }
    for (char & i : graph[maxY + 2]) i = STONE;
    cout << "Answer: " << dfs(0, 1000) << endl;
}
