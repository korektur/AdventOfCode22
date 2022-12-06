#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

size_t find_marker(string& line, int marker_size) {
    int start = 0;
    while(start < line.length() - marker_size) {
        int i = 0;
        for(; i < marker_size - 1; ++i) {
            auto end = line.begin() + start + marker_size;
            if (find(line.begin() + start + i + 1, end, line[start + i]) != end) {
                break;
            }
        }
        if (i == marker_size - 1) break;
        start += i + 1;
    }
    return start + marker_size;
}

int main() {
    ifstream infile("day6/day6.in");
    string line;
    getline(infile, line);

    cout << "Answer1: " << find_marker(line, 4) << endl;
    cout << "Answer2: " << find_marker(line, 14) << endl;
}
