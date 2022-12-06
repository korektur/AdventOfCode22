#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

size_t find_marker(const string &line, size_t marker_size) {
    unordered_map<char, size_t> m;
    size_t start = 0, end = 0;
    while (end - start < marker_size) {
        char c = line[end];
        if (m.contains(c)) start = max(start, m[c] + 1);
        m[c] = end++;
    }
    return end;
}

int main() {
    ifstream infile("day6/day6.in");
    string line;
    getline(infile, line);

    cout << "Answer1: " << find_marker(line, 4) << endl;
    cout << "Answer2: " << find_marker(line, 14) << endl;
}
