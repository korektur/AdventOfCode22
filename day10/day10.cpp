#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int x = 1;
int ans = 0;
int iteration = 0;
int crt = 0;
string crtLine;

void incrementIteration() {
    crtLine += abs(crt++ - x) <= 1 ? "#" : ".";
    if (crtLine.size() == 40) {
        crt = 0;
        cout << crtLine << endl;
        crtLine = string();
        crtLine.reserve(40);
    }
    if (++iteration == 20 || (iteration - 20) % 40 == 0) ans += x * iteration;
}

int main() {
    ifstream infile("day10/day10.in");
    string line;
    crtLine.reserve(40);
    while (getline(infile, line)) {
        if (line[0] == 'n') {
            incrementIteration();
        } else {
            incrementIteration();
            incrementIteration();
            x += stoi(line.substr(5));
        }
    }

    cout << "Answer1: " << ans << endl;
}
