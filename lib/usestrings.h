#pragma once

#include <array>
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert>

class Anagrams {
  static std::array<int8_t, 32> string2key(const std::string &s) {
    std::array<int8_t, 32> key{0};
    for (auto c : s) {
      int pos = static_cast<int>(1 + c - 'a');
      assert(pos < 32);

      key[pos]++;
    }

    return key;
  }

public:
  std::vector<std::vector<std::string>>
  getAnagrams(std::vector<std::string> all) {
    std::unordered_set<std::array<int8_t, 32>, std::vector<int>> distribution;
    for (size_t wordPos = 0; wordPos < all.size(); ++wordPos) {
      distribution[string2key(all[wordPos])].push_back(wordPos);
    }

    std::vector<std::vector<std::string>> ret;
    for (auto &[key, wordsIdxs] : distribution) {
      std::vector<std::string> anagrams(wordsIdxs.size());
      for (size_t wId = 0; wId < wordsIdxs.size(); ++wId) {
        anagrams[wId] = std::move(all[wordsIdxs[wId]]);
      }

      ret.push_back(std::move(anagrams));
    }

    return ret;
  }
};