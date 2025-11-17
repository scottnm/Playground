#include "lib.h"
#include <cstdio>

namespace CppTestLib
{

void TakeSomeCppParams(const std::string& str)
{
    printf("=> %30s :: \"%s\"\n", __PRETTY_FUNCTION__, str.c_str());
}

void TakeAPtrParam(const int* ip)
{
    printf("=> %30s :: %i\n", __PRETTY_FUNCTION__, *ip);
}

void TakeAStructPtrParam(const SomeStruct* sp)
{
    printf("=> %30s :: {%i, '%c'}\n", __PRETTY_FUNCTION__, sp->ival, sp->cval);
}

void OutStructPtrParam(SomeStruct* osp)
{
    osp->cval = 'o';
    osp->ival = 10;
    printf("<= %30s :: {%i, '%c'}\n", __PRETTY_FUNCTION__, osp->ival, osp->cval);
}

int ReturnSomePodValue(int i)
{
    int val = i * i;
    printf("<= %30s :: %i\n", __PRETTY_FUNCTION__, val);
    return val;
}

}