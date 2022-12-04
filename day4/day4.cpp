#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdio>

using namespace std;

constexpr inline size_t charToNum(const char &c) {
    return c > 'Z' ? c - 'a' : (c - 'A' + 26);
}

int main() {
    ifstream infile("day4/day4.in");
    string line;
    int range1[2]{0, 0}, range2[2]{0, 0};
    int ans1 = 0, ans2 = 0;
    while (getline(infile, line)) {
        sscanf_s(line.c_str(), "%d-%d,%d-%d", &range1[0], &range1[1], &range2[0], &range2[1]);
        int *fst = range1, *snd = range2;
        if (fst[0] > snd[0] || fst[1] < snd[1]) swap(range1, range2);
        if (fst[0] <= snd[0] && fst[1] >= snd[1]) ++ans1;
        if ((fst[1] >= snd[0] && fst[1] <= snd[1]) || (fst[0] >= snd[0] && fst[0] <= snd[1])
            || (fst[0] <= snd[0] && fst[1] >= snd[1])) {
            ++ans2;
        }
    }

    cout << "Answer1: " << ans1 << endl;
    cout << "Answer2: " << ans2 << endl;
}