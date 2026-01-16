#pragma once

#include <array>
#include <concepts>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

/*
Algorithm: RadixSort(a[], n):
   // Find the maximum element of the list
   max = a[0]
   for (i=1 to n-1):
      if (a[i]>max):
         max=a[i]

   // applying counting sort for each digit in each number of
   //the input list
   For (pos=1 to max/pos>0):
      countSort(a, n, pos)
      pos=pos*10



Algorithm: countSort(a, n, pos)
   Initialize count[09] with zeroes
   for i = 0 to n:
      count[(a[i]/pos) % 10]++
   for i = 1 to 10:
      count[i] = count[i] + count[i-1]
   for i = n-1 to 0:
      output[count[(a[i]/pos) % 10]-1] = a[i]
      i--
   for i to n:
      a[i] = output[i]

*/

template <std::integral Iv> Iv findMax(std::vector<Iv> &v) {
  Iv maxv{};

  for (auto iv : v)
    maxv = std::max<Iv>(iv, maxv);
  return maxv;
}

template <std::integral Iv> int numDigits(Iv v) {
  Iv base = 1;
  int numbers = 0;
  while ((v / base)) {
    ++numbers;
    base *= 10;
  }
  return numbers;
}

//          0,  1, 2,  3, 4,  5,  6
//         [30, 1, 22, 4, 35, 14, 5]
//-------------------------------------
//  mod = 1 , % 10
//          0,  1,  2, 4,  5,  4,  5
//  pos:
//          0 ->    [0] : 1
//          1       [1] : 1
//          2       [2] : 1
//          3, 5    [4] : 2
//          4, 6    [5] : 2
//-------------------------------------
//         [30, 1, 22, 4, 14, 35, 5]
//  mod = 10 , % 10
//          3,  0, 2,  0, 1,  3,  0
//  pos:
//          1, 3, 6 -> [0] : 3
//          4          [1] : 1
//          2          [2] : 1
//          0, 5       [3] : 2
//-------------------------------------
//         [1, 4, 5, 14, 22, 30, 35]

template <std::integral Iv>
void countSort(std::vector<Iv> &v, Iv base, std::vector<Iv> &out) {
  std::array<Iv, 10> counters{};
  std::array<size_t, 10> offsets{};

  for (size_t i = 0; i < v.size(); ++i) {
    auto off = (v[i] / base) % 10;
    counters[off]++;
  }

  size_t base_off = 0;
  for (size_t i = 0; i < counters.size(); ++i) {
    offsets[i] = base_off;
    base_off += counters[i];
  }

  for (size_t i = 0; i < v.size(); ++i) {
    size_t off_id = (v[i] / base) % 10;
    size_t off_new = offsets[off_id];
    offsets[off_id]++;
    out[off_new] = v[i];
  }
}

template <std::integral Iv>
void rdxsrt(std::vector<Iv> &v, std::vector<Iv> &workspace) {
  if (workspace.size() < v.size())
    throw std::runtime_error{"workspace size is too small"};

  Iv maxVal = findMax(v);
  int numbers = numDigits(maxVal);
  Iv base = 1;
  int inId = 0;
  std::vector<Iv> *inout[2] = {std::addressof(v), std::addressof(workspace)};
  for (int n = 0; n < numbers; ++n) {
    countSort(*inout[inId], base, *inout[inId ^ 1]);

    inId = inId ^ 1;
    base *= 10;
  }

  // if the output is in the workspace buffer
  if (inId == 1)
    std::copy(workspace.begin(), workspace.begin() + v.size(), v.begin());
}

template <std::integral Iv> void rdxsrt(std::vector<Iv> &v) {
  std::vector<Iv> workspace(v.size());
  rdxsrt(v, workspace);
}

template <typename V> struct RdxSrtTest {
  std::vector<V> input;
  std::vector<V> golden;
};

template <std::integral Iv> class RdxSrtTests {
public:
  std::vector<RdxSrtTest<Iv>> makeTests() {
    std::vector<RdxSrtTest<Iv>> list;
    list.emplace_back(std::vector<Iv>{33, 0, 1, 23, 7, 39, 11, 4},
                      std::vector<Iv>{0, 1, 4, 7, 11, 23, 33, 39});
    list.emplace_back(std::vector<Iv>{33, 253, 36, 23, 7, 39, 11, 4, 3},
                      std::vector<Iv>{3, 4, 7, 11, 23, 33, 36, 39, 253});
    return list;
  }

  void run() {
    auto tests = makeTests();
    for (auto &[input, golden] : tests) {
      std::cout << "test <" << input.size() << "> to <" << golden.size()
                << ">\n";

      rdxsrt(input);
      if (input != golden)
        throw std::runtime_error{"test fail"};
    }
  }
};

// void countsort(int a[], int n, int pos) {
//   int output[n + 1];
//   int max = (a[0] / pos) % 10;
//   for (int i = 1; i < n; i++) {
//     if (((a[i] / pos) % 10) > max)
//       max = a[i];
//   }
//   int count[max + 1];
//   for (int i = 0; i < max; ++i)
//     count[i] = 0;
//   for (int i = 0; i < n; i++)
//     count[(a[i] / pos) % 10]++;
//   for (int i = 1; i < 10; i++)
//     count[i] += count[i - 1];
//   for (int i = n - 1; i >= 0; i--) {
//     output[count[(a[i] / pos) % 10] - 1] = a[i];
//     count[(a[i] / pos) % 10]--;
//   }
//   for (int i = 0; i < n; i++)
//     a[i] = output[i];
// }
// void radixsort(int a[], int n) {
//   int max = a[0];
//   for (int i = 1; i < n; i++)
//     if (a[i] > max)
//       max = a[i];
//   for (int pos = 1; max / pos > 0; pos *= 10)
//     countsort(a, n, pos);
// }
// int main() {
//   int a[] = {236, 15, 333, 27, 9, 108, 76, 498};
//   int n = sizeof(a) / sizeof(a[0]);
//   cout << "Before sorting array elements are: ";
//   for (int i = 0; i < n; ++i) {
//     cout << a[i] << " ";
//   }
//   radixsort(a, n);
//   cout << "\nAfter sorting array elements are: ";
//   for (int i = 0; i < n; ++i) {
//     cout << a[i] << " ";
//   }
//   cout << "\n";
// }
