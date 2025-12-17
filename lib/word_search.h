#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/* gelly

l l y b c e e
e g k f d o s
a b c e p q l

d b d f e e k
d b a l y q g
e d l e l l e
*/
class CharBoard {
  std::vector<bool> used;
  int m;
  int n;

  void setFlag(int i, int j, bool f) { used[i * n + j] = f; }

  bool getFlag(int i, int j) const { return used[i * n + j]; }

  bool unrollWord(const std::vector<std::string> &board, int i, int j,
                  const std::string &word, int pos) {
    if (pos >= word.size())
      return true;

    if (i < 0 || i >= board.size() || j < 0 || j >= board[i].size())
      return false;
    if (getFlag(i, j))
      return false;

    if (board[i][j] != word[pos])
      return false;

    setFlag(i, j, true);
    bool result = unrollWord(board, i, j + 1, word, pos + 1) ||
                  unrollWord(board, i + 1, j, word, pos + 1) ||
                  unrollWord(board, i, j - 1, word, pos + 1) ||
                  unrollWord(board, i - 1, j, word, pos + 1);
    setFlag(i, j, false);

    return result;
  }

public:
  bool test(std::vector<std::string> board, std::string word) {
    if (word.size() > board.size() * board.at(0).size())
      return false;

    m = board.size();
    n = board.at(0).size();

    used.resize(m * n, false);

    auto rvr_word = word;
    std::reverse(rvr_word.begin(), rvr_word.end());

    for (int i = 0; i < board.size(); ++i) {
      for (int j = 0; j < board[i].size(); ++j) {
        if (board[i][j] == word[0] && unrollWord(board, i, j, word, 0))
          return true;

        if (board[i][j] == rvr_word[0] && unrollWord(board, i, j, word, 0))
          return true;
      }
    }

    return false;
  }
};

auto board2str(const std::vector<std::string> &m) {
  std::ostringstream ss;
  for (auto s : m) {
    ss << s << "\n";
  }

  return ss.str();
}

struct Test {
  std::vector<std::string> mtx;
  std::string word;
  bool result;
};

class Tests {
public:
  std::vector<Test> getCases() {
    std::vector<std::string> m = {"abce", "sfcs", "adee"};

    std::vector<Test> ret;
    ret.push_back(Test{.mtx = m, .word = "abcced", .result = true});
    ret.push_back(Test{.mtx = m, .word = "see", .result = true});
    ret.push_back(Test{.mtx = m, .word = "abcb", .result = false});

    return ret;
  }
};

bool testWordSearch() {
  auto cases = Tests{}.getCases();
  for (auto &[m, w, r] : cases) {
    auto test = CharBoard{}.test(m, w);

    if (test ^ r) {
      std::cout << "fail:     \n";
      std::cout << "  board: \n" << board2str(m) << "\n";
      std::cout << "  word : " << w << "\n";
      std::cout << std::boolalpha << "  result " << test << ", expected " << r
                << std::endl;
      return false;
    }
  }

  return true;
}