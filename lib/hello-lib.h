#ifndef EXAMPLES_CPP_HELLO_LIB_H_
#define EXAMPLES_CPP_HELLO_LIB_H_

#include <string>
#include <memory>
#include <vector>

namespace hello {

class HelloLib {
 public:
  explicit HelloLib(const std::string &greeting);

  void greet(const std::string &thing);

 private:
  std::unique_ptr<const std::string> greeting_;
};

class Tests{

public:
  int findMissing(std::vector<int> & arr);
  int xxc(int a, int b);
};
}  // namespace hello

#endif  // EXAMPLES_CPP_HELLO_LIB_H_