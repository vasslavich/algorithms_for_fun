#pragma once

/*

0   1  2  3       12  8  4  0
4   5  6  7       13  9  5  1
8   9  10 11      14  10 6  2
12  13 14 15      15  11 7  3


M,N
i0,j0  -  i1,j1

2,0(8)    0,1
3,1(9)    1,1
0,3(3)    3,3

i1 = j0
j1 = N-1 - i0

j = 0; j < M - 1 - j; ++j
i = j; i < N - 1 - i; ++i
*/

struct coord {
  int i;
  int j;
};
bool operator==(const coord &l, const coord &r) {
  return l.i == r.i && l.j == r.j;
}
auto getNewIJ(coord c, int n) {
  auto i0 = c.i;
  auto j0 = c.j;

  auto i1 = j0;
  auto j1 = n - 1 - i0;

  return coord{.i = i1, .j = j1};
}
auto getValue(auto &m2d, coord c) { return m2d[c.i][c.j]; }
void setValue(auto &m2d, coord c, auto newVal) { m2d[c.i][c.j] = newVal; }
void updateChain(coord start, auto &m2d, coord c, auto cNewVal) {
  auto cOld = getValue(m2d, c);
  m2d[c.i][c.j] = cNewVal;

  if (start == c)
    return;

  updateChain(start, m2d, getNewIJ(c, m2d.size()), cOld);
}
void updateChain(auto &m2d, coord c) {
  updateChain(c, m2d, getNewIJ(c, m2d.size()), getValue(m2d, c));
}

class Rt {
public:
  int m = -1;
  int n = -1;

  inline int &at(auto &m2d, int i, int j) { return m2d[i][j]; }

  void rotate(std::vector<std::vector<int>> &m2d) {
    m = m2d.size();
    n = m2d.at(0).size();

    int i = 0;
    while (i < m / 2) {
      int j = i;
      while (j < (n - 1 - i)) {
        updateChain(m2d, coord{.i = i, .j = j});
        ++j;
      }
      ++i;
    }
  }
};

class RtTest {
public:
  std::vector<std::vector<int>> m2d;
  std::vector<std::vector<int>> golden;
};

class RtTestList {
public:
  std::vector<RtTest> generateTests() {
    std::vector<RtTest> ret;
    ret.push_back(
        {.m2d = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}},
         .golden = {
             {12, 8, 4, 0}, {13, 9, 5, 1}, {14, 10, 6, 2}, {15, 11, 7, 3}}});
    return ret;
  }
};

void printM2d(const auto &m, int M, int N) {
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      std::cout << m[i][j] << " ";
    }

    std::cout << std::endl;
  }
}

bool RunRtTest() {
  for (auto &[m2d, golden] : RtTestList{}.generateTests()) {
    Rt{}.rotate(m2d);
    if (m2d != golden) {
      std::cout << "false test:\n";
      printM2d(m2d, m2d.size(), m2d.at(0).size());
      return false;
    }
  }

  return true;
}
