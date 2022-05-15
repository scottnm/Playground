#pragma once

#include <string.h>
#include <stdbool.h>

static
inline
bool
StrIEq(
    const char* a,
    const char* b)
{
#ifdef _WIN32
    return _stricmp(a, b) == 0;
#else // _WIN32
    return strcasecmp(a, b) == 0;
#endif // _WIN32
}
