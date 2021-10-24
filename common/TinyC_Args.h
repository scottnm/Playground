#pragma once

static inline
bool HasArg(
    int argc,
    char** argv,
    const char* arg)
{
    for (int i = 1; i < argc; ++i) // skip the program name
    {
        if (StrIEq(argv[i], arg))
        {
            return true;
        }
    }
    return false;
}

static inline
void PrintArgs(
    int argc,
    char** argv)
{
    printf("Args (%i):\n", argc);
    for (int i = 0; i < argc; ++i)
    {
        printf("    %i) %s\n", i, argv[i]);
    }
}

