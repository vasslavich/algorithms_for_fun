#pragma once

class Solution {
  static constexpr int bitsForObstacle = 2;
  static constexpr int bitsForNumPaths = sizeof(int) * 8 - bitsForObstacle;
  static constexpr int isZeroPaths = ((int)-1) >> bitsForObstacle;
  static constexpr int maxNumPaths = isZeroPaths;

  int m = -1;
  int n = -1;

  inline bool hasNumPaths(int v) {
    int maskPathsVal = v >> bitsForObstacle;
    return maskPathsVal > 0 && maskPathsVal != isZeroPaths;
  }
  inline int setNumPaths(int v, int numPaths) {
    return v | (numPaths << bitsForObstacle);
  }
  inline int getNumPaths(int v) {
    int h = v >> bitsForObstacle;
    return h == isZeroPaths ? 0 : h;
  }

  inline bool isObstacle(int v) {
    int v_obstacleBits = (((int)(-1)) >> bitsForNumPaths) & v;
    return v_obstacleBits > 0;
  }

  int numSubpaths(std::vector<std::vector<int>> &field, int i, int j) {
    if (hasNumPaths(field[i][j]))
      return getNumPaths(field[i][j]);

    if (isObstacle(field[i][j])) {
      setNumPaths(field[i][j], 0);
      return 0;
    }

    if (i == (m - 1) && j == (n - 1))
      return 1;

    int numSubpathsAcc = 0;
    if (i < (m - 1))
      numSubpathsAcc += numSubpaths(field, i + 1, j);

    if (j < (n - 1))
      numSubpathsAcc += numSubpaths(field, i, j + 1);

    return numSubpathsAcc;
  }

public:
  int uniquePathsWithObstacles(std::vector<std::vector<int>> &field) {
    m = field.size();
    if (m == 0)
      return 0;

    n = field.at(0).size();
    if (n == 0)
      return 0;

    return numSubpaths(field, 0, 0);
  }
};

struct TestCaseUnquePath {
  std::vector<std::vector<int>> field;
  int numValidPaths;
};

class TestsUniquePath {
public:
  static std::vector<TestCaseUnquePath> getTests() {
    std::vector<TestCaseUnquePath> tests;

    tests.push_back(
        {.field = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}, .numValidPaths = 2});
    tests.push_back(
        {.field = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}}, .numValidPaths = 0});
    return tests;
  }
};

bool RunTestUniquePaths() {
  for (auto [field, numValid] : TestsUniquePath::getTests()) {
    if (Solution{}.uniquePathsWithObstacles(field) != numValid)
      return false;
  }

  return true;
}