#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
    ifstream infile("day2/day2.in");

    string line;
    int score = 0;
    int score2 = 0;
    while (getline(infile, line)) {
        int fst = line[0] - 'A';
        int snd = line[2] - 'X';
        //part1
        score += snd + 1 + 3 * ((snd - fst + 3 + 1) % 3);

        //part2
        score2 += 3 * snd + (fst + (snd - 1) + 3) % 3 + 1;
    }

    cout << "Answer1: " << score << endl;
    cout << "Answer2: " << score2 << endl;
}