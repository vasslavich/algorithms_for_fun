#pragma once

#include <iostream>
#include <string>

// "0132" "4594" "45019342"
class IntlvStr {
  bool test(const std::string &s1, const std::string &s2, const std::string &s3,
            size_t s1_off, int s1_tiles, size_t s2_off, int s2_tiles) {

    if ((s1_off + s2_off) == s3.size()) {
      std::cout << "s1 tiles = " << s1_tiles << ", s2 tiles = " << s2_tiles
                << ", s3 size is " << s3.size() << std::endl;
      return std::abs(s1_tiles - s2_tiles) <= 1;
    }

    const size_t s3_off = s1_off + s2_off;

    for (size_t tail_off = 0; tail_off < (s3.size() - s1_off - s2_off);
         ++tail_off) {
      // test a block from s1
      if ((s1_off + tail_off) < s1.size() &&
          s1[s1_off + tail_off] == s3[s3_off + tail_off]) {
        if (test(s1, s2, s3, s1_off + tail_off + 1, s1_tiles + 1, s2_off,
                 s2_tiles))
          return true;

        continue;
      }
      
      // test a block from s2
      if ((s2_off + tail_off) < s2.size() &&
          s2[s2_off + tail_off] == s3[s3_off + tail_off]) {
        if (test(s1, s2, s3, s1_off, s1_tiles, s2_off + tail_off + 1,
                 s2_tiles + 1))
          return true;

        continue;
      }

      break;
    }

    return false;
  }

public:
  bool canBeInterleaved(std::string s1, std::string s2, std::string s3) {
    if ((s1.size() + s2.size() != s3.size()))
      return false;

    return test(s1, s2, s3, 0, 0, 0, 0);
  }
};

class IntlvStrTest {
public:
  std::string s1;
  std::string s2;
  std::string s3;
  bool resultValue = false;
};

class IntlvStrTestCollection {
public:
  std::vector<IntlvStrTest> getTests() {
    std::vector<IntlvStrTest> tests;
    tests.push_back({IntlvStrTest{
        .s1 = "0132", .s2 = "4594", .s3 = "45019342", .resultValue = true}});
    tests.push_back({IntlvStrTest{.s1 = "aabcc",
                                  .s2 = "dbbca",
                                  .s3 = "aadbbcbcac",
                                  .resultValue = true}});
    tests.push_back({IntlvStrTest{.s1 = "aabcc",
                                  .s2 = "dbbca",
                                  .s3 = "aadbbbaccc",
                                  .resultValue = false}});

    return tests;
  }
};

bool IntlvStrTestExectue() {
  for (auto &[s1, s2, s3, retVal] : IntlvStrTestCollection{}.getTests()) {
    auto testRet = IntlvStr{}.canBeInterleaved(s1, s2, s3);
    if (testRet ^ retVal) {
      std::cout << "wrong answer: the golden value should be <"
                << std::boolalpha << retVal << "> but got <" << std::boolalpha
                << testRet << ">" << std::endl;
      return false;
    }
  }

  return true;
}