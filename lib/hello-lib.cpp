#include "hello-lib.h"

#include <iostream>
#include <fmt/core.h>

using std::cout;
using std::endl;
using std::string;

namespace hello {

HelloLib::HelloLib(const string& greeting) : greeting_(new string(greeting)) {
}

void HelloLib::greet(const string& thing) {
  cout << *greeting_ << " " << thing << endl;
}

int Tests::findMissing(std::vector<int> & arr){
  int n = arr.size() + 1;
  fmt::print("Test: =============== 1...{}  ==================\n", n);
  int sum = 0;
  int half = ((1 + n)/2)*n;
  for(int i = 0; i < n; ++i)
    sum += (i+1);

  fmt::print("      check ... {}, {}\n", sum, half);

  int sum2 = 0;
  for(int i = 0; i < arr.size(); ++i){
    sum2 += arr[i];
  }

  return half - sum2;
}

int Tests::xxc(int a, int b){
  int l[2] = {0, -1};
  int e[2] = {0, 0};
  int g[2] = {0, 1};
  // -10, 10
  // 10, 11
  // 11, 10
  // 10, -10
  int f1 = (a - b) >> 31;  // 0, -1
  int f2 = (b - a) >> 31;  // 0, -1
  int cmp1 = f1 & (1^f2); // a < b 
  int cmp2 = (1^f1) & (1^f2); // a == b 
  int cmp3 = (1^f1) & f2; // a > b

  fmt::print("f1={}, f2={}, c1={}, c2={}, c3={}\n", f1, f2, cmp1, cmp2, cmp3);
  return l[cmp1] + e[cmp2] + g[cmp3];
}
  
}  // namespace hello