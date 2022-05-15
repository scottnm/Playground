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
    printf("    nato.exe [regular word]+\n");
    printf("    nato.exe -to-nato [regular word]+\n");
    printf("    nato.exe -from-nato [nato word]+\n");
}

// N.B. this map structured such that the indices are ordered relative
// to their character. 'a' is the first and 'z' is the last.
static const char* const g_charToNatoMap[] =
{
    "alfa",
    "bravo",
    "charlie",
    "delta",
    "echo",
    "foxtrot",
    "golf",
    "hotel",
    "india",
    "juliett",
    "kilo",
    "lima",
    "mike",
    "november",
    "oscar",
    "papa",
    "quebec",
    "romeo",
    "sierra",
    "tango",
    "uniform",
    "victor",
    "whiskey",
    "xray",
    "yankee",
    "zulu",
};

static
const char*
GetNatoWordFromAsciiChar(
    char c)
{
    assert(isalpha(c));
    return g_charToNatoMap[tolower(c) - 'a'];
}

static
char
GetAsciiCharFromNatoWord(
    const char* natoWord)
{
    for (char i = 0; i < ('z' - 'a'); ++i)
    {
        if (StrIEq(g_charToNatoMap[i], natoWord))
        {
            return 'a' + i;
        }
    }

    assert("nato keyword not found!");
    return '\0';
}

void
TranslateToNato(
    size_t wordCount,
    const char** words)
{
    for (size_t wordIndex = 0; wordIndex < wordCount; ++wordIndex)
    {
        const char* word = words[wordIndex];
        const size_t wordLength = strlen(word);

        for (size_t charIndex = 0; charIndex < wordLength; ++charIndex)
        {
            char character = word[charIndex];
            if (!isalpha(character))
            {
                continue;
            }

            printf("%c - %s\n", character, GetNatoWordFromAsciiChar(character));
        }

        const bool isLastWord = (wordIndex == wordCount - 1);

        // Add a new line between all words except the last
        if (!isLastWord)
        {
            printf("\n");
        }
    }
}

void
TranslateFromNato(
    size_t wordCount,
    const char** words)
{
    for (size_t wordIndex = 0; wordIndex < wordCount; ++wordIndex)
    {
        char asciiChar = GetAsciiCharFromNatoWord(words[wordIndex]);
        if (asciiChar == 0)
        {
            continue;
        }

        printf("%c", asciiChar);
    }
    printf("\n");
}

int main(
    int argc,
    char** argv)
{
    assert(argc > 0); // there's always at least one argument (the program name)

    switch (argc)
    {
        case 1:
        {
            PrintUsage(NULL);
            return 1;
        }
        case 2:
        {
            TranslateToNato(1, &argv[1]);
            break;
        }
        default:
        {
            assert(argc >= 3);

            int toNatoArgIndex = FindArg(argc, argv, "-to-nato");
            int fromNatoArgIndex = FindArg(argc, argv, "-from-nato");

            bool hasToNatoArg = toNatoArgIndex != -1;
            bool hasFromNatoArg = fromNatoArgIndex != -1;
            if (hasToNatoArg && hasFromNatoArg)
            {
                printf("To: %i (%s), From: %i (%s)\n", toNatoArgIndex, argv[toNatoArgIndex], fromNatoArgIndex, argv[fromNatoArgIndex]);
                PrintUsage("Cannot specify both '-to-nato' and '-from-nato'");
                return 1;
            }

            if (hasToNatoArg && toNatoArgIndex != 1)
            {
                PrintUsage("-to-nato must be the first argument if specified");
                return 1;
            }

            if (hasFromNatoArg && fromNatoArgIndex != 1)
            {
                PrintUsage("-from-nato must be the first argument if specified");
                return 1;
            }

            assert(!hasToNatoArg || !hasFromNatoArg);

            if (hasToNatoArg)
            {
                TranslateToNato((size_t)argc - 2, &argv[2]);
            }
            else if (hasFromNatoArg)
            {
                TranslateFromNato((size_t)argc - 2, &argv[2]);
            }
            else
            {
                TranslateToNato((size_t)argc - 1, &argv[1]);
            }
            break;
        }
    }

    return 0;
}
