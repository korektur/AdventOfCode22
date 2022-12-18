#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

struct Cube {
    int x, y, z;

    Cube(int x, int y, int z) : x(x), y(y), z(z) {}
};

unordered_map<int, unordered_map<int, unordered_set<int>>> cache;
unordered_map<int, unordered_map<int, unordered_set<int>>> visited;

bool is_present(const int &x, const int &y, const int &z) {
    return cache[x][y].contains(z);
}

int main() {
    ifstream infile("day18/day18.in");
    string line;
    vector<Cube> cubes;
    int cmin = INT_MAX, cmax = INT_MIN;
    while (getline(infile, line)) {
        int x, y, z;
        sscanf_s(line.c_str(), "%d,%d,%d", &x, &y, &z);
        cubes.emplace_back(x, y, z);
        cache[x][y].insert(z);
    }

    uint16_t ans1 = 0, ans2 = 0;
    for (const auto &cube: cubes) {
        int c[] = {cube.x, cube.y, cube.z};
        for (int &i: c) {
            cmin = min(cmin, i);
            cmax = max(cmax, i);
            i -= 1;
            ans1 += is_present(c[0], c[1], c[2]) ? 0 : 1;
            i += 2;
            ans1 += is_present(c[0], c[1], c[2]) ? 0 : 1;
            i--;
        }
    }

    cout << "Answer1: " << ans1 << endl;

    queue<Cube> q;
    q.emplace(0, 0, 0);
    visited[0][0].insert(0);
    while (!q.empty()) {
        auto &cube = q.front();
        int c[] = {cube.x, cube.y, cube.z};
        q.pop();
        for (int &i: c) {
            i -= 1;
            if (is_present(c[0], c[1], c[2])) ans2++;
            else if (i >= cmin - 1 && !visited[c[0]][c[1]].contains(c[2])) {
                visited[c[0]][c[1]].insert(c[2]);
                q.emplace(c[0], c[1], c[2]);
            }
            i += 2;
            if (is_present(c[0], c[1], c[2])) ans2++;
            else if (i <= cmax + 1 && !visited[c[0]][c[1]].contains(c[2])) {
                visited[c[0]][c[1]].insert(c[2]);
                q.emplace(c[0], c[1], c[2]);
            }
            i--;
        }
    }
    cout << "Answer2: " << ans2 << endl;
}
