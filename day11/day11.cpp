#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <cmath>
using namespace std;

uint64_t modulo = 1;

struct Item {
    uint64_t worry_level;

    Item(size_t worryLevel) : worry_level(worryLevel) {}
};

struct Monkey {
    size_t id;
    vector<shared_ptr<Item>> items;
    function<uint64_t(uint64_t)> op;
    uint64_t div_by;
    size_t case1;
    size_t case2;
    uint64_t inspected_cnt = 0;

    Monkey(size_t id, const vector<shared_ptr<Item>> &items, const function<uint64_t(uint64_t)> &op, size_t divBy, size_t case1,
           size_t case2) : id(id), items(items), op(op), div_by(divBy), case1(case1), case2(case2) {}
};

shared_ptr<Monkey> readMonkey(ifstream& infile) {
    string line;
    if(!getline(infile, line)) return nullptr;
    size_t id = stoi(line.substr(7, line.size() - 8));
    getline(infile, line);
    line = line.substr(strlen("  Starting items: "));
    stringstream stream(line);
    vector<shared_ptr<Item>> items;
    while(stream) {
        size_t worryLevel;
        char c;
        stream >> worryLevel >> c;
        items.emplace_back(make_shared<Item>(worryLevel));
    }
    getline(infile, line);
    char sign = line.substr(strlen("  Operation: new = old "), 1)[0];
    auto amount = line.substr(strlen("  Operation: new = old + "));
    function<uint64_t(uint64_t)> op;
    if (sign == '*') {
        if (amount == "old") {
            op = [](uint64_t x) { return x * x; };
        } else {
            int a = stoi(amount);
            op = [a](uint64_t x) { return x * a; };
        }
    } else {
        if (amount == "old") {
            op = [](uint64_t x) { return x + x; };
        } else {
            int a = stoi(amount);
            op = [a](uint64_t x) { return x + a; };
        }
    }
    getline(infile, line);
    size_t div_by = stoi(line.substr(strlen("  Test: divisible by ")));
    modulo *= div_by;
    getline(infile, line);
    size_t case1 = stoi(line.substr(::strlen("    If true: throw to monkey ")));
    getline(infile, line);
    size_t case2 = stoi(line.substr(::strlen("    If false: throw to monkey ")));
    getline(infile, line);
    return make_shared<Monkey>(id, items, op, div_by, case1, case2);
}

void round(unordered_map<size_t, shared_ptr<Monkey>>& monkeys) {
    for(size_t i = 0; i < monkeys.size(); ++i) {
        auto& monkey = monkeys[i];
        while(!monkey->items.empty()) {
            auto item = monkey->items.back();
            monkey->items.pop_back();
            item->worry_level = (monkey->op(item -> worry_level) % modulo);
            if (item->worry_level % monkey->div_by == 0) {
                monkeys[monkey->case1]->items.push_back(item);
            } else {
                monkeys[monkey->case2]->items.push_back(item);
            }
            monkey->inspected_cnt++;
        }
    }
}

int main() {
    ifstream infile("day11/day11.in");
    string line;
    unordered_map<size_t, shared_ptr<Monkey>> monkeys;
    while(true) {
        const shared_ptr<Monkey> &monkey = readMonkey(infile);
        if (monkey == nullptr) break;
        monkeys[monkey->id] = monkey;
    }
    for(int i = 0; i < 10000; ++i) {
        round(monkeys);
    }
    uint64_t max1 = 0, max2 = 0;
    for(size_t i = 0; i < monkeys.size(); ++i) {
        size_t cnt = monkeys[i]->inspected_cnt;
        if (cnt > max1) {
            max2 = max1;
            max1 = cnt;
        } else if (cnt > max2) {
            max2 = cnt;
        }
    }
    cout << max2 * max1 << endl;
}
