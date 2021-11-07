#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#include "../common/TinyC_Err.h" // GetLocalTime
#include "../common/TinyC_Memory.h" // GetLocalTime
#include "../common/TinyC_Time.h"   // StrIEq
#include "../common/TinyC_String.h" // GetLocalTime
#include "../common/TinyC_Args.h"   // HasArg,PrintArgs
#include "../common/TinyC_File.h"   // ReadFileIntoBuffer
#include "../common/TinyC_SAL.h"   // ReadFileIntoBuffer

static
void
PrintUsage(
    _In_opt_ const char* err)
{
    if (err != NULL)
    {
        printf("ERR: %s!\n\n", err);
    }

    printf("USAGE:\n");
    printf("    caesar.exe\n");
    printf("    caesar.exe <shift-from> <shift-to>\n");
}

bool
GetCaesarArg(
    const char* arg,
    _Out_ char* caesarArg
    )
{
    if (strlen(arg) != 1)
    {
        return false;
    }

    *caesarArg = (char)tolower(arg[0]);
    return true;
}

char
CaesarTransform(
    char input,
    int shift)
{
    if (!isalpha(input))
    {
        return input;
    }

    const char rangeStart = input <= 'Z' ? 'A' : 'a';
    const char caesarOffset = ((input - rangeStart) + shift) % 26;
    return rangeStart + caesarOffset;
}

int main(
    int argc,
    char** argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    static const int DEFAULT_ROT13_SHIFT= 13;

    int caesarShift;
    switch (argc)
    {
        case 1:
        {
            caesarShift = DEFAULT_ROT13_SHIFT;
            break;
        }
        case 3:
        {
            char caesarArg1;
            if (!GetCaesarArg(argv[1], &caesarArg1))
            {
                PrintUsage("Arg 1 invalid! Must be a single alpha character");
                return 1;
            }

            char caesarArg2;
            if (!GetCaesarArg(argv[2], &caesarArg2))
            {
                PrintUsage("Arg 2 invalid! Must be a single alpha character");
                return 1;
            }

            if (caesarArg2 >= caesarArg1)
            {
                caesarShift = caesarArg2 - caesarArg1;
            }
            else
            {
                caesarShift = (caesarArg2 + 26) - caesarArg1;
            }
            break;
        }
        default:
        {
            PrintUsage("Invalid number of arguments");
            return 1;
        }
    }

    while (true)
    {
        int nextInput = getchar();
        if (nextInput == EOF)
        {
            break;
        }
        else if (nextInput < 0 || nextInput > 255)
        {
            printf("ERR: invalid input '%i'\n", nextInput);
            break;
        }

        char nextChar = (char)nextInput;

        char newChar = CaesarTransform(nextChar, caesarShift);
        putchar(newChar);
    }

    return 0;
}
