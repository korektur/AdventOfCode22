#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <queue>

#define ll unsigned long long
using namespace std;

inline bool shouldMove(const pair<int, int> &fst, const pair<int, int> &snd) {
    return abs(fst.first - snd.first) > 1 || abs(fst.second - snd.second) > 1;
}

size_t simulate(size_t rope_size) {
    ifstream infile("day9/day9.in");
    string line;
    vector<pair<int, int>> rope;
    for (int i = 0; i < rope_size; ++i) {
        rope.emplace_back(0, 0);
    }
    char direction;
    int steps;
    unordered_set<ll> visited;
    while (getline(infile, line)) {
        direction = line[0];
        steps = stoi(line.substr(2));

        for (int j = 0; j < steps; ++j) {
            if (direction == 'U') {
                rope[0].second += 1;
                for (int i = 1; i < rope_size && shouldMove(rope[i - 1], rope[i]); ++i) {
                    rope[i].first = rope[i - 1].first;
                    rope[i].second += 1;
                }
            } else if (direction == 'D') {
                rope[0].second -= 1;
                for (int i = 1; i < rope_size && shouldMove(rope[i - 1], rope[i]); ++i) {
                    rope[i].first = rope[i - 1].first;
                    rope[i].second -= 1;
                }
            } else if (direction == 'L') {
                rope[0].first -= 1;
                for (int i = 1; i < rope_size && shouldMove(rope[i - 1], rope[i]); ++i) {
                    rope[i].second = rope[i - 1].second;
                    rope[i].first -= 1;
                }
            } else {
                rope[0].first += 1;
                for (size_t i = 1; i < rope_size && shouldMove(rope[i - 1], rope[i]); ++i) {
                    rope[i].second = rope[i - 1].second;
                    rope[i].first += 1;
                }
            }

            ll val = ((ll) rope[rope_size - 1].first) * 100000 + rope[rope_size - 1].second;
            if (!visited.contains(val)) visited.insert(val);
        }
    }

    cout << visited.size() << endl;
    cout << endl;
    infile.close();
    return visited.size();
}

int main() {
//    cout << simulate(2) << endl;
    cout << simulate(10) << endl;
}
