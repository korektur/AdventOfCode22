#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <list>

using namespace std;

list<pair<int, int64_t>> nums;

int64_t solve(list<pair<int, int64_t>> numbers, int iterations = 1, long multiplier = 1) {
    while(iterations-- > 0) {
        for (size_t i = 0; i < numbers.size(); ++i) {
            auto it = numbers.begin();
            int64_t ind = 0;
            while (it->first != i) { it++; ind++; }
            pair<int, int64_t> p = *it;
            numbers.erase(it);
            int64_t second = p.second * multiplier;
            if (second < 0) second += (abs(second) / numbers.size() + 1) * numbers.size();
            int64_t newPosInd = (ind + second) % (int64_t) numbers.size();
            if (newPosInd == 0) newPosInd = (int) numbers.size();
            auto newPos = numbers.begin();
            advance(newPos, newPosInd);
            numbers.insert(newPos, p);
        }
    }

    int64_t ans = 0;
    auto it = find_if(numbers.begin(), numbers.end(), [=](auto &p) { return p.second == 0; });
    for(int i = 1; i <= 3000; ++i) {
        it++;
        if (it == numbers.end()) it = numbers.begin();
        if (i % 1000 == 0) ans += it->second * multiplier;
    }
    return ans;
}


int main() {
    ifstream infile("day20/day20.in");
    string line;

    int i = 0;
    while (getline(infile, line)) {
        int num = stoi(line);
        nums.emplace_back(i++, num);
    }

    cout << "Answer1: " << solve(nums) << endl;
    cout << "Answer2: " << solve(nums, 10, 811589153) << endl;
}
