#pragma once

static inline
int FindArg(
    int argc,
    char** argv,
    const char* arg)
{
    for (int i = 0; i < argc; ++i)
    {
        if (StrIEq(argv[i], arg))
        {
            return i;
        }
    }

    return -1;
}

static inline
bool HasArg(
    int argc,
    char** argv,
    const char* arg)
{
    return FindArg(argc, argv, arg) != -1;
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

