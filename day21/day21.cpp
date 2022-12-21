#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include<sstream>

using namespace std;

struct Node;

unordered_map<string, shared_ptr<Node>> nodes;

struct Node {
    const string name;
    string left;
    string right;
    string op;
    int64_t val = 0;
    bool marker;

    explicit Node(const string& name) : name(name), marker(name == "humn") {}

    pair<int64_t, bool> eval() {
        if (op.empty()) return make_pair(val, marker);
        auto l = nodes[left]->eval();
        auto r = nodes[right]->eval();
        if (l.second || r.second) marker = true;

        switch (op[0]) {
            case '+': val = l.first + r.first; break;
            case '*': val = l.first * r.first; break;
            case '/': val = l.first / r.first; break;
            case '-': val = l.first - r.first; break;
        }
        return make_pair(val, marker);
    }

    void eval2(int64_t target) {
        if (name == "humn") {
            cout << "Answer2: " << target << endl;
            return;
        }
        auto fst = nodes[left];
        auto snd = nodes[right];
        if (fst->marker) swap(fst, snd);
        switch (op[0]) {
            case '+': snd->eval2(target - fst->val); return;
            case '*': snd->eval2(target / fst->val); return;
            case '/': left == fst->name ? snd->eval2(fst->val / target) : snd->eval2(fst->val * target); return;
            case '-': left == fst->name ? snd->eval2(fst->val - target) : snd->eval2(fst->val + target); return;
        }
    }
};

int main() {
    ifstream infile("day21/day21.in");
    string line;

    while (getline(infile, line)) {
        size_t i = line.find(':');
        string node_name = line.substr(0, i);
        i += 2;
        string expr = line.substr(i);
        shared_ptr<Node> ptr(new Node(node_name));
        if (expr[0] >= '0' && expr[0] <= '9') {
            ptr->val = stoi(expr);
        } else {
            istringstream ss(expr);
            getline(ss, ptr->left, ' ');
            getline(ss, ptr->op, ' ');
            getline(ss, ptr->right, ' ');
        }
        nodes[node_name] = ptr;
    }

    cout << "Answer1: " << nodes["root"]->eval().first << endl;
    nodes["root"]->op = "-";
    nodes["root"]->eval2(0);
}
