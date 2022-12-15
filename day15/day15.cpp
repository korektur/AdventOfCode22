#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <unordered_set>

using namespace std;

inline int64_t dist(const pair<int64_t, int64_t> &fst, const pair<int64_t, int64_t> &snd) {
    return abs(fst.first - snd.first) + abs(fst.second - snd.second);
}

int main() {
    ifstream infile("day15/day15.in");
    string line;
    vector<pair<int64_t, int64_t>> sensors;
    vector<int64_t> dists;
    unordered_set<int64_t> beacons;

    int64_t y = 2000000;
    while (getline(infile, line)) {
        pair<int64_t, int64_t> sensor(0, 0);
        pair<int64_t, int64_t> beacon(0, 0);
        sscanf_s(line.c_str(), "Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld",
                 &sensor.first, &sensor.second, &beacon.first, &beacon.second);
        int64_t distance = dist(sensor, beacon);
        sensors.push_back(sensor);
        if (beacon.second == y) beacons.insert(beacon.first);
        dists.push_back(distance);
    }

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
    int64_t start = INT64_MIN;
    for (const auto &segment: segments) {
        start = max(start, segment.first);
        if (start < segment.second) {
            ans += segment.second - start + 1;
            for (const auto &beacon: beacons) {
                if (beacon >= start && beacon <= segment.second) {
                    --ans;
                }
            }
            start = segment.second + 1;
        }
    }
    cout << "Answer1: " << ans << endl;
}
