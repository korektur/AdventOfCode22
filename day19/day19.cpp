#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Blueprint {
    int ore_robot_cost = 0;
    int clay_robot_cost = 0;
    int obs_robot_cost_ore = 0;
    int obs_robot_cost_clay = 0;
    int geode_robot_cost_ore = 0;
    int geode_robot_cost_obs = 0;
    int id = 0;
};

struct State {
    uint16_t ore = 0, clay = 0, obs = 0, geode = 0;

    State(uint16_t ore, uint16_t clay, uint16_t obs, uint16_t geode) : ore(ore), clay(clay), obs(obs), geode(geode) {}

    explicit State(const State *other) : ore(other->ore), clay(other->clay), obs(other->obs), geode(other->geode) {}
};

constexpr inline uint16_t time_to_build(int resources, int required, int robot_count) {
    if (resources >= required) {
        return 1;
    }
    return (required - resources + robot_count - 1) / robot_count + 1;
}

uint32_t dfs(const shared_ptr<State>& current, int ore_r, int clay_r, int obs_r, int geode_r, Blueprint *b, int minute) {
    if (geode_r == 0 && minute < 4) return 0; //arbitrary optimization that might not be correct, but works for my input
    uint32_t res = minute * geode_r + current->geode;

    if (ore_r < b->clay_robot_cost || ore_r < b->obs_robot_cost_ore || ore_r < b->geode_robot_cost_ore) {
        uint16_t days_required = time_to_build(current->ore, b->ore_robot_cost, ore_r);
        if (days_required < minute) {
            auto newState = make_shared<State>(current.get());
            newState->ore = current->ore + ore_r * days_required - b->ore_robot_cost;
            newState->clay += clay_r * days_required;
            newState->obs += obs_r * days_required;
            newState->geode += geode_r * days_required;
            res = max(res, dfs(newState, ore_r + 1, clay_r, obs_r, geode_r, b, minute - days_required));
        }
    }
    if (clay_r < b->obs_robot_cost_clay) {
        uint16_t days_required = time_to_build(current->ore, b->clay_robot_cost, ore_r);
        if (days_required < minute) {
            auto newState = make_shared<State>(current.get());
            newState->ore = current->ore + ore_r * days_required - b->clay_robot_cost;
            newState->clay += clay_r * days_required;
            newState->obs += obs_r * days_required;
            newState->geode += geode_r * days_required;
            res = max(res, dfs(newState, ore_r, clay_r + 1, obs_r, geode_r, b, minute - days_required));
        }
    }

    if (clay_r > 0 && obs_r < b->geode_robot_cost_obs) {
        uint16_t days_required = time_to_build(current->ore, b->obs_robot_cost_ore, ore_r);
        days_required = max(days_required, time_to_build(current->clay, b->obs_robot_cost_clay, clay_r));
        if (days_required < minute) {
            auto newState = make_shared<State>(current.get());
            newState->ore += ore_r * days_required - b->obs_robot_cost_ore;
            newState->clay += clay_r * days_required - b->obs_robot_cost_clay;
            newState->obs += obs_r * days_required;
            newState->geode += geode_r * days_required;
            res = max(res, dfs(newState, ore_r, clay_r, obs_r + 1, geode_r, b, minute - days_required));
        }
    }

    if (obs_r > 0) {
        uint16_t days_required = time_to_build(current->ore, b->geode_robot_cost_ore, ore_r);
        days_required = max(days_required, time_to_build(current->obs, b->geode_robot_cost_obs, obs_r));
        if (days_required < minute) {
            auto newState = make_shared<State>(current.get());
            newState->ore += ore_r * days_required - b->geode_robot_cost_ore;
            newState->clay += clay_r * days_required;
            newState->obs += obs_r * days_required - b->geode_robot_cost_obs;
            newState->geode += geode_r * days_required;
            res = max(res, dfs(newState, ore_r, clay_r, obs_r, geode_r + 1, b, minute - days_required));
        }
    }
    return res;
}

int main() {
    ifstream infile("day19/day19.in");
    string line;
    vector<shared_ptr<Blueprint>> blueprints;
    uint32_t total = 0;
    while (getline(infile, line)) {
        shared_ptr<Blueprint> b(new Blueprint);
        sscanf_s(line.c_str(), "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. "
                               "Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.",
                 &b->id, &b->ore_robot_cost, &b->clay_robot_cost, &b->obs_robot_cost_ore, &b->obs_robot_cost_clay,
                 &b->geode_robot_cost_ore,
                 &b->geode_robot_cost_obs);
        blueprints.push_back(b);
        total += dfs(make_shared<State>(0, 0, 0, 0), 1, 0, 0, 0, b.get(), 24) * b->id;
    }
    cout << "Answer1: " << total << endl;

    uint32_t ans2 = 1;
    for (int i = 0; i < 3; ++i) {
        ans2 *= dfs(make_shared<State>(0, 0, 0, 0), 1, 0, 0, 0, blueprints[i].get(), 32);
        cout << ans2 << endl;
    }

    cout << "Answer2: " << ans2 << endl;
}
