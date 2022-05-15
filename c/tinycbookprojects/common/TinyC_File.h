#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "TinyC_Memory.h" // tc_buffer_t

static inline
tc_err_t ReadFileSize(
    FILE* f,
    _Out_ size_t* outSize)
{
    *outSize = 0;

    int startingPosition = ftell(f);
    if (fseek(f, 0, SEEK_END) != 0)
    {
        return TC_ERR_FILE_UNKNOWN_ERR;
    }

    size_t size = ftell(f);
    if (fseek(f, 0, SEEK_SET) != 0)
    {
        return TC_ERR_FILE_UNKNOWN_ERR;
    }

    if (fseek(f, startingPosition, SEEK_SET) != 0)
    {
        return TC_ERR_FILE_UNKNOWN_ERR;
    }

    *outSize = size;
    return TC_ERR_SUCCESS;
}

static inline
tc_err_t ReadFileIntoBuffer(
    const char* filePath,
    _Out_ tc_buffer_t* outBuffer)
{
    *outBuffer = (tc_buffer_t){ 0 };

    tc_err_t result = TC_ERR_SUCCESS;

    FILE* file;
    errno_t err = fopen_s(&file, filePath, "rb");
    if (err == 0)
    {
        size_t size;
        result = ReadFileSize(file, &size);
        if (TC_SUCCEEDED(result))
        {
            tc_buffer_t buffer;
            result = BufferAlloc(size, &buffer);
            if (TC_SUCCEEDED(result))
            {
                size_t bytesRead = fread(buffer.data, 1, buffer.len, file);
                if (bytesRead == buffer.len)
                {
                    BufferSwap(&buffer, outBuffer);
                }
                else
                {
                    result = TC_ERR_FILE_READ_ERR;
                    BufferFree(&buffer);
                }
            }
        }

        fclose(file);
    }
    else
    {
        result = TC_ERR_FILE_OPEN_FAILED;
    }

    return result;
}
