#include "jump.h"

bool Jump::test(std::vector<int> maxSteps) {
  canBeReached = std::vector<int>(maxSteps.size(), 0);
  lastIndex = canBeReached.size() - 1;

  canBeReached[lastIndex] = 1;
  return testTail(maxSteps, 0);
}

bool Jump::testTail(const std::vector<int> &maxSteps, int fromPos) {
  if (canBeReached[fromPos])
    return true;

  for (int step = maxSteps[fromPos]; step > 0; --step) {
    int stepPos = fromPos + step;
    if (stepPos == lastIndex){
      canBeReached[fromPos] = 1;
      return true;
    }

    if (stepPos > lastIndex)
      return false;

    if (canBeReached[stepPos]){
      canBeReached[fromPos] = 1;
      return true;
    }

    if (testTail(maxSteps, stepPos)){
      canBeReached[fromPos] = 1;
      return true;
    }
  }

  return false;
}
