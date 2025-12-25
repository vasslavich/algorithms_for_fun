#include "summerz.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>

int diff(int l, int r) { return std::abs(l - r); }

int v3(std::vector<int> v, int p) {
  if (v.size() < 2)
    return 0;

  std::sort(v.begin(), v.end());

  auto count = [](const std::vector<int> & v, int max_diff){
    size_t i = 0;
    int count = 0;
    while(i < (v.size() - 1)){
      if(diff(v[i], v[i+1]) <= max_diff){
        i++;
        ++count;
      }

      i++;
    }

    return count;
  };

  int low = 0;
  int high = v.back() - v.front();
  int test_diff = high;

  while (low <= high){
    test_diff = low + (high - low) / 2;

    if(count(v, test_diff) >= p){
      high = test_diff - 1;
    }else{
      low = test_diff + 1;
    }
  }

  return test_diff;
}

int v2(std::vector<int> v, int p) {
  std::sort(v.begin(), v.end());
  std::map<int, int> diff_sh0;
  std::map<int, int> diff_sh1;

  for (size_t i = 0; i < v.size() - 1; i += 2) {
    diff_sh0[diff(v[i], v[i + 1])]++;
    if (i > 0)
      diff_sh1[diff(v[i - 1], v[i])]++;
  }

  diff_sh1[diff(v[v.size() - 2], v[v.size() - 1])]++;

  int min_test = 0;
  int p0 = p;
  auto it_0 = diff_sh0.begin();
  while (p0 > 0) {
    p0 -= it_0->second;
    min_test = it_0->first;

    it_0++;
  }

  int min_test2 = 0;
  int p1 = p;
  auto it_1 = diff_sh1.begin();
  while (p1 > 0) {
    p1 -= it_1->second;
    min_test2 = it_1->first;

    it_1++;
  }

  min_test = std::min(min_test, min_test2);

  std::cout << "diff 0: " << std::endl;
  for (auto [v, n] : diff_sh0) {
    std::cout << "    " << v << ":" << n << std::endl;
  }
  std::cout << "diff 1: " << std::endl;
  for (auto [v, n] : diff_sh1) {
    std::cout << "    " << v << ":" << n << std::endl;
  }

  return min_test;
}

int v1(std::vector<int> v, int p) {
  std::set<size_t> used;
  int count = 0;

  int mindiff_accm = 0;
  while (count < p) {
    int mindiff = -1;
    int x = -1, y = -1;

    for (size_t i = 0; i < (v.size() - 1); ++i) {
      if (used.count(i))
        continue;

      for (size_t j = i + 1; j < v.size(); ++j) {
        if (used.count(j))
          continue;

        int test_diff = diff(v[i], v[j]);
        if (mindiff == -1 || mindiff > test_diff) {
          mindiff = test_diff;
          x = i;
          y = j;
        }
      }
    }

    if (mindiff >= 0) {
      used.insert(x);
      used.insert(y);

      ++count;
      mindiff_accm = mindiff;
    }
  }

  assert(count == p);

  return mindiff_accm;
}

int Execute::summerz(const std::vector<int> &v, int p) { return v3(v, p); }