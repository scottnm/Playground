#include <string>

namespace CppTestLib
{
    struct SomeStruct
    {
        int ival;
        char cval;
    };

    void TakeSomeCppParams(const std::string& str);
    void TakeAPtrParam(const int* ip);
    void TakeAStructPtrParam(const SomeStruct* sp);
    void OutStructPtrParam(SomeStruct* osp);
    int ReturnSomePodValue(int i);
}