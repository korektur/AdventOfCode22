#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

constexpr inline size_t charToNum(const char &c) {
    return c > 'Z' ? c - 'a' : (c - 'A' + 26);
}

int main() {
    ifstream infile("day3/day3.in");
    string line;
    size_t score = 0, score2 = 0, c, lineNum = 0;
    bool visited[52];
    size_t cnt[52];
    while (getline(infile, line)) {
        //part1
        fill_n(visited, 52, false);
        size_t i = 0;
        for (; i < line.size() / 2; ++i) visited[charToNum(line[i])] = true;

        for (; i < line.size(); ++i) {
            c = charToNum(line[i]);
            if (visited[c]) {
                score += c + 1;
                break;
            }
        }

        //part2
        if (lineNum++ % 3 == 0) {
            fill_n(cnt, 52, 0);
        }
        fill_n(visited, 52, false);
        for (i = 0; i < line.length(); ++i) {
            size_t j = charToNum(line[i]);
            if (!visited[j] && ++cnt[j] == 3) score2 += j + 1;
            visited[j] = true;
        }
    }

    cout << "Answer1: " << score << endl;
    cout << "Answer2: " << score2 << endl;
}

void solvePart1() {

}