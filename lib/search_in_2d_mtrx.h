#pragma once

#include <algorithm>
#include <vector>

class Search2DMatrix {
public:
  bool contains(std::vector<std::vector<int>> &mtx, int target) {
    const int m = mtx.size();
    if (m == 0)
      return false;

    const int n = mtx.at(0).size();
    std::vector<int> rth_values(n);
    for (int i = 0; i < m; ++i)
      rth_values[i] = mtx[i][n - 1];

    auto it_greater =
        std::upper_bound(rth_values.begin(), rth_values.end(), target);
  }
};