#include "lib.h"

int main()
{
    CppTestLib::TakeSomeCppParams("test123");

    int i = 5;
    CppTestLib::TakeAPtrParam(&i);

    CppTestLib::SomeStruct inVal{30, 'Z'};
    CppTestLib::TakeAStructPtrParam(&inVal);

    CppTestLib::SomeStruct outVal;
    CppTestLib::OutStructPtrParam(&outVal);

    const int rv = CppTestLib::ReturnSomePodValue(10);
    printf(".. received %i from CppTestLib::ReturnSomePodValue", rv);
}