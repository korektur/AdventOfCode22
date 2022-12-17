#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <deque>
#include <chrono>

using namespace std;
using namespace std::chrono;

deque<vector<int>> tetris;

void createMinus() {
    tetris.push_back({0, 0, 1, 1, 1, 1, 0});
}

void createPlus() {
    tetris.push_back({0, 0, 0, 1, 0, 0, 0});
    tetris.push_back({0, 0, 1, 1, 1, 0, 0});
    tetris.push_back({0, 0, 0, 1, 0, 0, 0});
}

void createLShape() {
    tetris.push_back({0, 0, 1, 1, 1, 0, 0});
    tetris.push_back({0, 0, 0, 0, 1, 0, 0});
    tetris.push_back({0, 0, 0, 0, 1, 0, 0});
}

void createPlank() {
    tetris.push_back({0, 0, 1, 0, 0, 0, 0});
    tetris.push_back({0, 0, 1, 0, 0, 0, 0});
    tetris.push_back({0, 0, 1, 0, 0, 0, 0});
    tetris.push_back({0, 0, 1, 0, 0, 0, 0});
}

void createCube() {
    tetris.push_back({0, 0, 1, 1, 0, 0, 0});
    tetris.push_back({0, 0, 1, 1, 0, 0, 0});
}

void initialize() {
    tetris.push_back({0, 0, 0, 0, 0, 0, 0});
    tetris.push_back({0, 0, 0, 0, 0, 0, 0});
    tetris.push_back({0, 0, 0, 0, 0, 0, 0});
}

bool moveLeft(int i) {
    for (int j = i; j < tetris.size(); ++j) {
        if (tetris[j][0] == 1) return false;
        for (int k = 1; k < tetris[i].size(); ++k) {
            if (tetris[j][k] == 1 && tetris[j][k - 1] == 2) return false;
        }
    }
    for (; i < tetris.size(); ++i) {
        for (int j = 1; j < tetris[i].size(); ++j) {
            if (tetris[i][j] == 1) {
                tetris[i][j - 1] = 1;
                tetris[i][j] = 0;
            }
        }
    }
    return true;
}

bool moveRight(int i) {
    for (int j = i; j < tetris.size(); ++j) {
        if (tetris[j].back() == 1) return false;
        for (int k = 5; k >= 0; --k) {
            if (tetris[j][k] == 1 && tetris[j][k + 1] == 2) return false;
        }
    }
    for (; i < tetris.size(); ++i) {
        for (int j = 5; j >= 0; --j) {
            if (tetris[i][j] == 1) {
                tetris[i][j + 1] = 1;
                tetris[i][j] = 0;
            }
        }
    }
    return true;
}

bool moveDown(int i) {
    if (i == 0) return false;
    for (int k = i; k < tetris.size(); ++k) {
        for (int j = 0; j < tetris[i].size(); ++j) {
            if (tetris[k][j] == 1 && tetris[k - 1][j] == 2) return false;
        }
    }
    for (; i < tetris.size(); ++i) {
        for (int j = 0; j < tetris[i].size(); ++j) {
            if (tetris[i][j] == 1) {
                tetris[i - 1][j] = tetris[i][j];
                tetris[i][j] = 0;
            }
        }
    }
    for (const auto &item: tetris.back()) {
        if (item != 0) return true;
    }
    tetris.pop_back();
    return true;
}

void mark_done(int i) {
    for (; i < tetris.size(); ++i) std::replace(tetris[i].begin(), tetris[i].end(), 1, 2);
}

void toInt(const vector<int> &v, uint64_t & hash, int base) {
    for (int i = 0; i < v.size(); ++i) if (v[i] == 2) hash |= 1ULL << (i + base);
}

unordered_map<size_t, unordered_map<::uint64_t , pair<uint64_t, uint64_t>>> statesHash;

int main() {
    auto start1 = high_resolution_clock::now();
    ifstream infile("day17/day17.in");
    string pattern;
    getline(infile, pattern);
    auto figures = vector({createMinus, createPlus, createLShape, createPlank, createCube});
    size_t curAction = 0, curFigure = 0;
    uint64_t ans = 60;
    for (uint64_t i = 0; i < 1000000000000; ++i) {
        if (i == 2023) cout << "Answer1: " << ans << endl;
        initialize();
        int ptr = (int) tetris.size();
        if (curFigure == 1 && tetris.size() == 63) {
            ::uint64_t hash = 0;
            toInt(tetris[ptr - 4], hash, 0);
            toInt(tetris[ptr - 5], hash, 8);
            toInt(tetris[ptr - 6], hash, 16);
            if (statesHash[curAction].contains(hash)) {
                auto &res = statesHash[curAction][hash];
                ::uint64_t step = i - res.second;
                ::uint64_t ansStep = ans - res.first;
                auto skipping = (1000000000000 - i) / step;
                ans += ansStep * skipping;
                i += skipping * step;
            } else {
                statesHash[curAction][hash] = make_pair(ans, i);
            }
        }
        figures[curFigure]();
        curFigure = (curFigure + 1) % figures.size();
        while (true) {
            char action = pattern[curAction];
            curAction = (curAction + 1) % pattern.size();
            if (action == '<') moveLeft(ptr); else moveRight(ptr);
            if (!moveDown(ptr)) {
                mark_done(ptr);
                while (tetris.size() > 60) {
                    tetris.pop_front();
                    ans++;
                }
                break;
            }
            --ptr;
        }
    }

    cout << "Answer2: " << ans << endl;
    auto stop1 = high_resolution_clock::now();
    cout << "answer took " << duration_cast<milliseconds>(stop1 - start1).count() << "ms" << endl;
}
