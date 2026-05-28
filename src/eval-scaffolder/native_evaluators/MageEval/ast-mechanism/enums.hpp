#include <string>
#include <unordered_map>

enum class MageEval {
    TEST
};

const std::unordered_map<MageEval, std::string> EvalString = {
    {MageEval::TEST, "test"}
};