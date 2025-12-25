#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

class LargestNumber {
public:
  // 2020, 2010: 20202010, 2010202
  // 30, 91    : 3091, 9130
  // 30, 910   : 30910, 91030
  // 3001, 30  : 300130, 303001
  // 3, 34     : 334 343
  // 3, 30, 34, 5, 9 : 9534330
  //                   9533034
  static bool isGreaterOrEq(std::string lft, std::string rth) {
    for (size_t l = 0; l < std::min(lft.size(), rth.size()); ++l) {
      if (lft[l] > rth[l])
        return true;
      if (lft[l] < rth[l])
        return false;
    }

    if (lft.size() == rth.size())
      return true;

    if (lft.size() > rth.size() && lft.back() > rth.front())
      return true;

    return false;
  }

  std::string get(std::vector<int> &numbers) {
    std::vector<std::string> snumbers(numbers.size());
    for (size_t i = 0; i < numbers.size(); ++i)
      snumbers[i] = std::to_string(numbers[i]);

    std::sort(snumbers.begin(), snumbers.end(),
              [](const auto &l, const auto &r) { return isGreaterOrEq(l, r); });
    std::string ret(snumbers.size() * 16, '-');
    auto outIt = ret.begin();
    for (auto &s : snumbers) {
      std::copy(s.begin(), s.end(), outIt);
      std::advance(outIt, s.size());
    }
    ret.erase(outIt, ret.end());
    return ret;
  }
};

struct TestLN {
  std::string sNum;
  std::vector<int> numbers;
};

std::vector<TestLN> getCases() {
  return std::vector<TestLN>{
      TestLN{.sNum = "210", .numbers = {10, 2}},
      TestLN{.sNum = "9534330", .numbers = {3, 30, 34, 5, 9}}};
}

bool testLargestNumber() {
  for (auto &[sNum, numbers] : getCases()) {
    auto s = LargestNumber{}.get(numbers);
    if (s != sNum) {
      std::cout << "wrong : " << s << ", expected " << sNum << std::endl;
      return false;
    }
  }

  return true;
}