#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <functional>

using namespace std;

vector<vector<int>> tetris;

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
    for(int j = i; j < tetris.size(); ++j) {
        if (tetris[j][0] == 1) return false;
        for (int k = 1; k < tetris[i].size(); ++k) {
            if (tetris[j][k] == 1 && tetris[j][k - 1] == 2) return false;
        }
    }
    for(; i < tetris.size(); ++i) {
        for(int j = 1; j < tetris[i].size(); ++j) {
            if (tetris[i][j] == 1) {
                tetris[i][j - 1] = 1;
                tetris[i][j] = 0;
            }
        }
    }
    return true;
}

bool moveRight(int i) {
    for(int j = i; j < tetris.size(); ++j) {
        if (tetris[j].back() == 1) return false;
        for (int k = (int)tetris[i].size() - 2; k >= 0 ; --k) {
            if (tetris[j][k] == 1 && tetris[j][k + 1] == 2) return false;
        }
    }
    for(; i < tetris.size(); ++i) {
        for(int j = (int)tetris[i].size() - 2; j >= 0 ; --j) {
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
    for(; i < tetris.size(); ++i) {
        for(int j = 0; j < tetris[i].size(); ++j) {
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
    for(; i < tetris.size(); ++i) {
        for(int j = 0; j < tetris[i].size(); ++j) {
            if (tetris[i][j] == 1) tetris[i][j] = 2;
        }
    }
}

int main() {
    ifstream infile("day17/day17.in");
    string pattern;
    getline(infile, pattern);
    auto figures = vector({ createMinus, createPlus, createLShape, createPlank, createCube });
    int curAction = 0;
    int curFigure = 0;
    for(int i = 0; i < 2022; ++i) {
        initialize();
        int ptr = (int) tetris.size();
        figures[(curFigure++) % figures.size()]();
        while(true) {
            char action = pattern[(curAction++) % pattern.size()];
            if (action == '<') {
                moveLeft(ptr);
            } else {
                moveRight(ptr);
            }
            if (!moveDown(ptr)) {
                mark_done(ptr);
                break;
            }
            --ptr;
        }
    }

    cout << tetris.size() << endl;
//    for(int i = tetris.size() - 1; i >= 0; --i) {
//        cout << '|';
//        for (int j = 0; j < 7; ++j) cout << (tetris[i][j] == 0 ? '.' : '#');
//        cout << '|' << endl;
//    }
}
