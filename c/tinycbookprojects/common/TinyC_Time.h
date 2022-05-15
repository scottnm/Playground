#pragma once

#include <time.h>
#include <assert.h>

static
inline
struct tm
GetLocalTime()
{
    const time_t now = time(NULL);

    struct tm timeData;
#ifdef _WIN32
    errno_t err = localtime_s(&timeData, &now); // Windows
    assert(err == 0);
#else  // _WIN32
    localtime_r(&now, &timeData); // Unix
#endif // _WIN32

    return timeData;
}
