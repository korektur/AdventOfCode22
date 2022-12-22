#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<string> map;

constexpr pair<int, int> rotate(char changeDirection, const pair<int, int> &direction) {
    if (changeDirection == 'L') {
        if (direction.first != 0) {
            return make_pair(0, -direction.first);
        }
        return make_pair(direction.second, 0);
    } else {
        if (direction.first != 0) {
            return make_pair(0, direction.first);
        }
        return make_pair(-direction.second, 0);
    }
}

int score(int x, int y, pair<int, int> direction) {
    int score = 1000 * (y + 1) + 4 * (x + 1);
    if (direction.first == 1) {
        score += 0;
    } else if (direction.first == -1) {
        score += 2;
    } else if (direction.second == 1) {
        score += 1;
    } else {
        score += 3;
    }
    return score;
}

int main() {
    ifstream infile("day22/day22.in");
    string line;
    while (getline(infile, line) && !line.empty()) {
        map.push_back(line);
    }

    int x = (int) map[0].find_first_of('.');
    int y = 0;
    pair<int, int> direction(1, 0);
    getline(infile, line);
    int i = 0;
    while(i < line.length()) {
        int k = 0;
        while (i + k < line.length() && line[i + k] >= '0' && line[i + k] <= '9') k++;
        if (k > 0) {
            int steps = stoi(line.substr(i, k));
            for (int step = 0; step < steps; ++step) {
                int newY = y + direction.second;
                int newX = x + direction.first;
                char to = ' ';
                if (newY >= 0 && newX >= 0 && newY < map.size() && newX < map[newY].size()) {
                    to = map[newY][newX];
                }
                if (to == ' ') {
                    if (direction.first == 1) {
                        newX = 0;
                        while(map[y][newX] == ' ') ++newX;
                    } else if (direction.first == -1) {
                        newX = map[y].find_last_not_of(' ');
                    } else if (direction.second == 1) {
                        newY = 0;
                        while (x >= map[newY].size() || map[newY][x] == ' ') newY++;
                    } else {
                        newY = map.size() - 1;
                        while (newY >= map.size() || map[newY].size() <= x || map[newY][x] == ' ') newY--;
                    }
                }
                to = map[newY][newX];
                if (to == '#') {
                    break;
                } else {
                    x = newX;
                    y = newY;
                }
            }
        }

        i += k;
        if (i < line.length()) {
            direction = rotate(line[i++], direction);
        }
    }
    cout << score(x, y, direction) << endl;
}

#pragma clang diagnostic pop