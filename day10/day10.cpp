#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int x = 1;
int ans = 0;
int iteration = 0;
int crt = 0;

void incrementIteration() {
    if (++iteration == 20 || (iteration - 20) % 40 == 0) {
        ans += x * iteration;
//        cout << iteration << " " << ans << " " << x << endl;
    }
}

int main() {
    ifstream infile("day10/day10.in");
    string line;
    while(getline(infile, line)) {
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
