#pragma once

typedef enum
{
    TC_ERR_SUCCESS = 0,
    // FILE
    TC_ERR_FILE_UNKNOWN_ERR,
    TC_ERR_FILE_OPEN_FAILED,
    TC_ERR_FILE_READ_ERR,
    // MEMORY
    TC_ERR_OOM,
} tc_err_t;

#define TC_SUCCEEDED(err) ((err) == (TC_ERR_SUCCESS))
#define TC_FAILED(err) (!(TC_SUCCEEDED((err))))

static inline
const char*
GetErrString(
    tc_err_t err)
{
    switch (err)
    {
        case TC_ERR_SUCCESS:
            return "success";
        case TC_ERR_FILE_UNKNOWN_ERR:
            return "unknown file err";
        case TC_ERR_FILE_OPEN_FAILED:
            return "failed to open file";
        case TC_ERR_FILE_READ_ERR:
            return "failed to read file";
        case TC_ERR_OOM:
            return "out of memory";
        default:
            return "unknown err";
    }
}
