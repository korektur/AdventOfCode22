#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <stack>

using namespace std;

size_t solve1(const vector<vector<size_t>> &grid) {
    vector<vector<size_t>> gridCol(grid);
    vector<vector<size_t>> gridRow(grid);
    vector<vector<size_t>> dist;
    dist.resize(grid.size());
    for (int i = 0; i < grid.size(); ++i) dist[i].resize(grid[i].size(), 1);
    for (int i = 1; i < gridCol[0].size() - 1; ++i) {
        size_t curMax = grid[0][i];
        for (int j = 1; j < gridCol.size() - 1; ++j) {

            gridCol[j][i] = curMax;
            if (curMax <= grid[j][i]) {
                curMax = grid[j][i];
            }
        }

        curMax = gridCol[gridCol.size() - 1][i];
        for (int j = gridCol.size() - 2; // NOLINT(cppcoreguidelines-narrowing-conversions)
             j >= 0 && curMax < gridCol[j][i]; --j) {
            gridCol[j][i] = curMax;
            curMax = max(curMax, grid[j][i]);
        }
    }

    for (int i = 1; i < gridRow.size() - 1; ++i) {
        size_t curMin = gridRow[i][0];
        for (int j = 1; j < gridRow[i].size() - 1; ++j) {
            gridRow[i][j] = curMin;
            curMin = max(curMin, grid[i][j]);
        }
        curMin = gridRow[i][gridCol[i].size() - 1];
        for (int j = gridRow[i].size() - 2; // NOLINT(cppcoreguidelines-narrowing-conversions)
             j >= 0 && curMin < gridRow[i][j]; --j) {
            gridRow[i][j] = curMin;
            curMin = max(curMin, grid[i][j]);
        }
    }
    size_t ans1 = grid.size() * 2 + grid[0].size() * 2 - 4;
    for (int i = 1; i < grid.size() - 1; ++i) {
        for (int j = 1; j < grid[i].size() - 1; ++j) {
            if (grid[i][j] > gridCol[i][j] || grid[i][j] > gridRow[i][j]) {
                ans1++;
            }
        }
    }
    return ans1;
}

size_t solve2(const vector<vector<size_t>> &grid) {
    size_t max = 0;
    vector<vector<size_t>> dist;
    for (int i = 0; i < grid.size(); ++i) {
        stack<pair<size_t, size_t>> st;
        dist.emplace_back(grid[i].size(), 1);
        st.emplace(SIZE_MAX, 0);
        for (int j = 0; j < grid[i].size(); ++j) {
            while (!st.empty() && st.top().first < grid[i][j]) st.pop();
            dist[i][j] *= j - st.top().second;
            st.emplace(grid[i][j], j);
        }

        st = stack<pair<size_t, size_t>>();
        st.emplace(SIZE_MAX, 0);
        for (int j = 0; j < grid[i].size(); ++j) {
            while (!st.empty() && st.top().first < grid[i][grid[i].size() - j - 1]) st.pop();
            dist[i][grid.size() - j - 1] *= j - st.top().second;
            st.emplace(grid[i][grid[i].size() - j - 1], j);
        }
    }

    for (int i = 0; i < grid[0].size(); ++i) {
        stack<pair<size_t, size_t>> st;
        st.emplace(SIZE_MAX, 0);
        for (int j = 0; j < grid.size(); ++j) {
            while (!st.empty() && st.top().first < grid[j][i]) st.pop();
            dist[j][i] *= j - st.top().second;
            st.emplace(grid[j][i], j);
        }

        st = stack<pair<size_t, size_t>>();
        st.emplace(SIZE_MAX, 0);
        for (int j = 0; j < grid.size(); ++j) {
            while (!st.empty() && st.top().first < grid[grid.size() - j - 1][i]) st.pop();
            dist[grid.size() - j - 1][i] *= j - st.top().second;
            st.emplace(grid[grid.size() - j - 1][i], j);
            max = std::max(max, dist[grid.size() - j - 1][i]);
        }
    }
    return max;
}

int main() {
    ifstream infile("day8/day8.in");
    string line;
    vector<vector<size_t>> grid;

    while (getline(infile, line)) {
        vector<size_t> row;
        row.reserve(line.length());
        for (char i: line) row.emplace_back(i - '0');
        grid.emplace_back(std::move(row));
    }

    cout << "Answer1: " << solve1(grid) << endl;
    cout << "Answer2: " << solve2(grid) << endl;
}
