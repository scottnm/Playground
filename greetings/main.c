#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "../common/TinyC_Time.h" // StrIEq
#include "../common/TinyC_String.h" // GetLocalTime

typedef enum DayPhase
{
    DayPhase_Morning = 0,
    DayPhase_Afternoon = 1,
    DayPhase_Evening = 2,
    DayPhase_DeepNight = 3,
} DayPhase;

DayPhase
GetDayPhase()
{
    const struct tm timeData = GetLocalTime();
    const int hour = timeData.tm_hour;
    if (hour < 5)
    {
        return DayPhase_DeepNight;
    }
    else if (hour < 12)
    {
        return DayPhase_Morning;
    }
    else if (hour < 18)
    {
        return DayPhase_Afternoon;
    }
    else if (hour < 23)
    {
        return DayPhase_Evening;
    }
    else // hour == 23
    {
        return DayPhase_DeepNight;
    }
}

const char*
GetGreetingForDayPhase(
    DayPhase phase
    )
{
    switch (phase)
    {
        case DayPhase_Morning:
            return "Good morning";
        case DayPhase_Afternoon:
            return "Good afternoon";
        case DayPhase_Evening:
            return "Getting late";
        case DayPhase_DeepNight:
            return "Go to bed";
        default:
            assert(false);
            return "What time is it";
    }
}

typedef enum MoonPhase
{
    MoonPhase_NewMoon        = 0,
    MoonPhase_WaxingCrescent = 1,
    MoonPhase_FirstQuarter   = 2,
    MoonPhase_WaxingGibbous   = 3,
    MoonPhase_FullMoon       = 4,
    MoonPhase_WaningGibbous   = 5,
    MoonPhase_LastQuarter    = 6,
    MoonPhase_WaningCrescent = 7,
    MoonPhase_NumPhases = 8,
} MoonPhase;

MoonPhase
CalculateMoonPhase()
{
    const struct tm timeData = GetLocalTime();
    const int day = timeData.tm_mday;
    const int month = timeData.tm_mon;
    const int year = timeData.tm_year;

    int d = day;
    if (month == 2)
    {
        d += 31;
    }
    else if (month > 2)
    {
        d += 59 + ((month - 3) * 30.6) + 0.5;
    }

    int g = (year - 1900) % 19;

    int e = ((11 * g) + 29) % 30;
    if (e == 25 || e == 24)
    {
        e += 1;
    }

    // [0,7] -> ["waxing crescent","new"]
    int phaseValue = (((((e + d) * 6) + 5) % 177)/ 22) & 7;

    // shift our phase value into our enum range
    // [0,7] -> ["new","waning crescent"]
    // i.e. shift forward 1 so we start on "new" instead of ending on it
    return (phaseValue + 1) % MoonPhase_NumPhases;
}

const char*
GetMoonPhaseName(
    MoonPhase phase)
{
    switch (phase)
    {
        case MoonPhase_NewMoon:
            return "New Moon";
        case MoonPhase_WaxingCrescent:
            return "Waxing Crescent";
        case MoonPhase_FirstQuarter:
            return "First Quarter";
        case MoonPhase_WaxingGibbous:
            return "Waxing Gibbous";
        case MoonPhase_FullMoon:
            return "Full Moon";
        case MoonPhase_WaningGibbous:
            return "Waning Gibbous";
        case MoonPhase_LastQuarter:
            return "Last Quarter";
        case MoonPhase_WaningCrescent:
        default:
            return "Waning Crescent";
    }
}

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

int main(
    int argc,
    char** argv)
{
// #define DBG_PRINT_ARGS
#ifdef DBG_PRINT_ARGS
    PrintArgs(argc, argv);
#endif // DBG_PRINT_ARGS

    const char* name = argc < 2 ? "keyboard-peasant" : argv[1];

    const DayPhase dayPhase = GetDayPhase();
    const char* greeting = GetGreetingForDayPhase(dayPhase);

    printf("%s, %s\n", greeting, name);

    const bool showMoonPhase = HasArg(argc, argv, "/moon");
    if (showMoonPhase)
    {
        const MoonPhase moonPhase = CalculateMoonPhase();
        printf("Current moon phase: %s\n", GetMoonPhaseName(moonPhase));
    }

    printf("Session starting...\n");
    return 0;
}
