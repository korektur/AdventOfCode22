#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <vector>
#include <deque>
#include <stack>

using namespace std;

int main() {
    ifstream infile("day5/day5.in");
    string line;

    vector<deque<char>> v;
    while (getline(infile, line) && line[1] != '1') {
        for (size_t i = 1; i < line.length(); i += 4) {
            if (v.size() <= i / 4) v.emplace_back();
            if (line[i] >= 'A' && line[i] <= 'Z') {
                v[i / 4].push_back(line[i]);
            }
        }
    }
    vector<deque<char>> v2(v);
    getline(infile, line);
    size_t cnt, from, to;
    stack<char> st;
    while(getline(infile, line)) {
        sscanf_s(line.c_str(), "move %d from %d to %d", &cnt, &from, &to);
        for (int i = 0; i < cnt; ++i) {
            //part1
            v[to - 1].emplace_back(v[from - 1].front());
            v[from - 1].pop_front();
            //part2
            st.emplace(v2[from - 1].front());
            v2[from -1].pop_front();
        }
        while(!st.empty()) {
            v2[to - 1].push_front(st.top());
            st.pop();
        }
    }

    for (const auto &item: v) {
        if (!item.empty()) cout << item.front();
    }
    cout << endl;
    for (const auto &item: v2) {
        if (!item.empty()) cout << item.front();
    }
}