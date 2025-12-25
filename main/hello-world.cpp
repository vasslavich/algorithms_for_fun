#include "lib/hello-lib.h"
#include "lib/jump.h"
#include "lib/summerz.h"
//#include "lib/usestrings.h"
#include "lib/largest_number.h"
#include "lib/unique_path.h"
#include "lib/word_search.h"
#include "lib/rotate_2d.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using hello::HelloLib;
using std::string;

void check(auto func, auto ok_pattern) {
  std::cout << "check ..." << func << " and " << ok_pattern << std::endl;
  if (func != ok_pattern) {
    std::cout << "    fail" << std::endl;
    throw std::runtime_error{"test fail"};
  }
}

void testFindMissing(std::vector<int> &arr, int expected) {
  hello::Tests t;
  auto missN = t.findMissing(arr);
  if (missN != expected) {
    std::cout << "fail : " << missN << ", expected is " << expected
              << std::endl;
  } else {
    std::cout << "ok" << std::endl;
  }
}

void testXcc(int a, int b, int expected) {
  hello::Tests t;
  if (t.xxc(a, b) != expected) {
    std::cout << "fail : " << a << " ? " << b << " -> " << t.xxc(a, b)
              << ", expected is " << expected << std::endl;
  } else {
    std::cout << "ok" << std::endl;
  }
}

void testMinMax() {
  static const std::vector<int> t1 = {0, 1, 10, 3, 8, 2, 4, 7, 10, 3};
  Execute exec;
  check(exec.summerz(t1, 2), 0);
  check(exec.summerz(t1, 3), 1);
}

template <typename F> void run(std::string_view name, F f) {
  auto res = f();
  std::cout << "test func " << name << ": " << std::boolalpha << res
            << std::endl;
}

void testJump() {
  run("jump", [] {
    Jump j;
    return !j.test({3, 2, 1, 0, 4});
  });
}

int main(int argc, char **argv) {
  // HelloLib lib("Hello");
  // string thing = "world";
  // if (argc > 1) {
  //   thing = argv[1];
  // }
  // lib.greet(thing);

  // testXcc(10, 10, 0);
  // testXcc(-10, 10, -1);
  // testXcc(10, -10, 1);
  // testXcc(10, 11, -1);
  // testXcc(11, 10, 1);

  // std::vector<int> v{1,2,4};
  // testFindMissing(v, 3);

  // testMinMax();
  // testJump();

  // run("word search", testWordSearch);
  // run("largest number", testLargestNumber);
  //bool upResult = RunTestUniquePaths();
  //std::cout << "unique paths result is ..... " << std::boolalpha << upResult << std::endl;


  bool rtResult = RunRtTest();
  std::cout << "rotate ..... " << std::boolalpha << rtResult
            << std::endl;
  return 0;
}