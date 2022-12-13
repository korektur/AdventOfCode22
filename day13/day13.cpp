#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Element;

int compare(const Element &fst, const Element &snd);

struct Element {
    int val;
    shared_ptr<vector<Element>> arr;

    explicit Element(shared_ptr<vector<Element>> arr) : val(-1), arr(std::move(arr)) {}

    explicit Element(int val) : val(val), arr(new vector<Element>()) {
        arr->emplace_back(*this);
    }

    bool operator<(const Element &other) const {
        return compare(*this, other) < 0;
    }

    bool operator==(const Element &other) const {
        return compare(*this, other) == 0;
    }
};

int compare(const Element &fst, const Element &snd) {
    if (fst.val != -1 && snd.val != -1) {
        if (fst.val > snd.val) return 1;
        return fst.val < snd.val ? -1 : 0;
    }

    size_t i = 0, j = 0;
    while (i < fst.arr->size() && j < snd.arr->size()) {
        int c = compare(fst.arr->at(i++), snd.arr->at(j++));
        if (c != 0) return c;
    }
    if (i < fst.arr->size()) return 1;
    return j < snd.arr->size() ? -1 : 0;
}

Element parse(const string &line, size_t &offset) {
    if (line[offset] == ',') offset++;

    if (line[offset] == '[') {
        offset++;
        shared_ptr<std::vector<Element>> v(new vector<Element>);
        while (line[offset] != ']') {
            v->emplace_back(parse(line, offset));
        }
        ++offset;
        return std::move(Element(v));
    }
    size_t x = 0;
    while (line[offset + x] != ']' && line[offset + x] != ',') ++x;
    Element res(stoi(line.substr(offset, x)));
    offset += x;
    return std::move(res);
}

int main() {
    ifstream infile("day13/day13.in");

    size_t i = 1, ans = 0;
    vector<Element> packets;
    size_t offset = 0;
    while (true) {
        string line1, line2;
        offset = 0;
        getline(infile, line1);
        Element l1 = parse(line1, offset);
        offset = 0;
        getline(infile, line2);
        Element l2 = parse(line2, offset);

        if (compare(l1, l2) < 0) ans += i;
        ++i;
        packets.emplace_back(std::move(l1));
        packets.emplace_back(std::move(l2));
        if (!getline(infile, line1)) break;
    }
    cout << "Answer1: " << ans << endl;

    offset = 0;
    Element p1 = parse(string("[[2]]"), offset);
    offset = 0;
    Element p2 = parse(string("[[6]]"), offset);
    packets.emplace_back(p1);
    packets.emplace_back(p2);
    sort(packets.begin(), packets.end());

    size_t d1 = distance(begin(packets), find(begin(packets), end(packets), p1)) + 1;
    size_t d2 = distance(begin(packets), find(begin(packets), end(packets), p2)) + 1;
    cout << "Answer2: " << d2 * d1 << endl;
}
