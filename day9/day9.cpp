#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>

#define ll long long
using namespace std;

inline bool shouldMove(const pair<int, int> &fst, const pair<int, int> &snd) {
    return abs(fst.first - snd.first) > 1 || abs(fst.second - snd.second) > 1;
}

size_t simulate(size_t rope_size) {
    ifstream infile("day9/day9.in");
    string line;
    vector<pair<int, int>> rope(rope_size, make_pair(0, 0));
    char direction;
    int steps;
    unordered_set<ll> visited;
    while (getline(infile, line)) {
        direction = line[0];
        steps = stoi(line.substr(2));

        for (int j = 0; j < steps; ++j) {
            switch(direction) {
                case 'U': rope[0].second += 1; break;
                case 'D': rope[0].second -= 1; break;
                case 'L': rope[0].first -= 1; break;
                default: rope[0].first += 1; break;
            }

            for (int i = 1; i < rope_size && shouldMove(rope[i - 1], rope[i]); ++i) {
                if (rope[i].second != rope[i - 1].second) {
                    rope[i].second += (rope[i - 1].second > rope[i].second) ? 1 : -1;
                }
                if (rope[i].first != rope[i - 1].first) {
                    rope[i].first += (rope[i - 1].first > rope[i].first) ? 1 : -1;
                }
            }

            visited.insert(((ll) rope.back().first) * 10000 + rope.back().second);
        }
    }

    infile.close();
    return visited.size();
}

int main() {
    cout << simulate(2) << endl;
    cout << simulate(10) << endl;
}
