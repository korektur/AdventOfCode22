#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <utility>

using namespace std;
#define ll unsigned long long

ll ans1 = 0;
ll ans2 = ULLONG_MAX;
ll missing_capacity = 0;

class Node {
    string name;
public:
    explicit Node(string name) : name(std::move(name)) {}

    virtual ll get_size() = 0;

    [[nodiscard]] const string &get_name() const { return name; }
};

class File : public Node {
    const ll size;

public:
    File(string name, const size_t size) : Node(std::move(name)), size(size) {}

    ll get_size() override { return size; }
};

class Dir : public Node {
    unordered_map<string, shared_ptr<Node>> children;
    ll size = -1;
public:
    explicit Dir(const string &name, shared_ptr<Node> parent) : Node(name) {
        children[".."] = std::move(parent);
    }

    void add_child(shared_ptr<Node> &&node) {
        children[node->get_name()] = node;
    }

    shared_ptr<Node> get_child(const string& name) {
        return children.contains(name) ? children[name] : nullptr;
    }

    ll get_size() override {
        for (const auto &item: children) {
            if (item.first == "..") continue;
            size += item.second->get_size();
        }
        if (size < 100000) ans1 += size;
        if (size > missing_capacity) ans2 = min(ans2, size);
        return size;
    }
};

int main() {
    ifstream infile("day7/day7.in");
    string line;
    shared_ptr<Dir> root(new Dir("", nullptr));
    root->add_child(make_shared<Dir>("/", root));
    shared_ptr<Dir> cur(root);
    while (getline(infile, line)) {
        if (line[0] == '$') {
            if (line.starts_with("$ cd")) {
                auto name = line.substr(5);
                cur = dynamic_pointer_cast<Dir, Node>(cur->get_child(name));
            }
        } else {
            if (line.starts_with("dir")) {
                cur->add_child(std::make_shared<Dir>(line.substr(4), cur));
            } else {
                size_t ind = line.find_first_of(' ');
                size_t size = stoi(line.substr(0, ind));
                string filename = line.substr(ind + 1);
                cur->add_child(std::make_shared<File>(filename, size));
            }
        }
    }
    missing_capacity = 30000000 - (70000000 - root->get_size());
    ans2 = ULLONG_MAX;
    root->get_size();
    cout << "Answer1: " << ans1 << endl;
    cout << "Answer2: " << ans2 << endl;
}
