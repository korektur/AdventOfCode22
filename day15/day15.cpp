#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <unordered_map>

using namespace std;

inline int64_t dist(const pair<int64_t, int64_t> &fst, const pair<int64_t, int64_t> &snd) {
    return abs(fst.first - snd.first) + abs(fst.second - snd.second);
}

int main() {
    ifstream infile("day15/day15.in");
    string line;
    vector<pair<int64_t, int64_t>> sensors;
    vector<int64_t> dists;
    unordered_map<int64_t, unordered_set<int64_t>> beacons;

    while (getline(infile, line)) {
        pair<int64_t, int64_t> sensor(0, 0);
        pair<int64_t, int64_t> beacon(0, 0);
        sscanf_s(line.c_str(), "Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld",
                 &sensor.first, &sensor.second, &beacon.first, &beacon.second);
        int64_t distance = dist(sensor, beacon);
        sensors.push_back(sensor);
        beacons[beacon.second].insert(beacon.first);
        dists.push_back(distance);
    }

    for(int64_t y = 0; y < 4000000; ++y) {
        uint64_t ans = 0;
        vector<pair<int64_t, int64_t>> segments;
        for (int64_t i = 0; i < sensors.size(); ++i) {
            int64_t rem = dists[i] - abs(sensors[i].second - y);
            if (rem < 0) continue;
            int64_t l = sensors[i].first - rem;
            int64_t r = sensors[i].first + rem;
            segments.emplace_back(l, r);
        }
        sort(segments.begin(), segments.end());
        int64_t start = segments[0].first;
        for (const auto &segment: segments) {
            if (start < segment.first && start > 0) {
                cout << "Answer2: " << ((uint64_t) start) * 4000000 + y << endl;
                if (y > 2000000) return 0;
            }
            start = max(start, segment.first);
            if (start < segment.second) {
                ans += segment.second - start + 1;
                for (const auto &beacon: beacons[y]) {
                    if (beacon >= start && beacon <= segment.second) {
                        --ans;
                    }
                }
                start = segment.second + 1;
            }
        }
        if (y == 2000000) cout << "Answer1: " << ans << endl;
    }
}
