#include <vector>

class Jump{
    std::vector<int> canBeReached;
    int lastIndex;
    
    bool testTail(const std::vector<int> & maxSteps, int fromPos);
public:
    bool test(std::vector<int> maxSteps);
};
