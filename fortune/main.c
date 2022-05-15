#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "../common/TinyC_Err.h" // GetLocalTime
#include "../common/TinyC_Memory.h" // GetLocalTime
#include "../common/TinyC_Time.h"   // StrIEq
#include "../common/TinyC_String.h" // GetLocalTime
#include "../common/TinyC_Args.h"   // HasArg,PrintArgs
#include "../common/TinyC_File.h"   // ReadFileIntoBuffer
#include "../common/TinyC_SAL.h"   // ReadFileIntoBuffer

void PrintUsage()
{
    printf("USAGE:\n");
    printf("    fortune.exe [fortune-filepath]\n");
}

tc_err_t
ParseFortunesFromFileContents(
    tc_buffer_t fortuneFileBuffer,
    _Out_ size_t* outFortuneCount,
    _Outptr_ char*** outFortuneList,
    _Out_ tc_buffer_t* outSerializedFortunesBuffer
    )
{
    // Rather than calculate the least amount of memory we need, just make sure we allocate enough. 2X the original
    // file size should be enough since it leaves room to copy every character into their own string buffer
    const size_t maxFortunes = fortuneFileBuffer.len / 2;
    const size_t maxSerializedFortuneSize = fortuneFileBuffer.len * 2;
    const size_t maxBufferSize =
        (maxFortunes * sizeof(char*)) + // allocate space for each fortune pointer
        maxSerializedFortuneSize;       // allocate space for each serialized fortune

    tc_err_t err = BufferAlloc(maxBufferSize, outSerializedFortunesBuffer);
    if (TC_FAILED(err))
    {
        return err;
    }

    size_t fortuneCount = 0;
    char** nextFortunePtrSlot = outSerializedFortunesBuffer->data;

    char* currentFortuneWritePtrStart = (char*)((char**)(outSerializedFortunesBuffer->data) + maxFortunes);
    char* currentFortuneWritePtr = currentFortuneWritePtrStart;
    size_t remainingSerializedFortuneBufferSize = maxSerializedFortuneSize;

    const char* remainingFortuneFileBuffer = fortuneFileBuffer.data;
    size_t remainingFortuneFileBufferSize = fortuneFileBuffer.len;
    while (remainingFortuneFileBufferSize != 0)
    {
        //
        // calculate the next line
        //
        const char* nextUnixNewLine = strstr(remainingFortuneFileBuffer, "\n");
        const char* nextWindowsNewLine = strstr(remainingFortuneFileBuffer, "\r\n");

        const char* srcLineStart = remainingFortuneFileBuffer;
        const char* srcLineEnd;
        size_t srcLineLength;
        if (nextUnixNewLine != NULL && nextWindowsNewLine != NULL)
        {
            if (nextUnixNewLine < nextWindowsNewLine)
            {
                srcLineEnd = nextUnixNewLine + 1;
                srcLineLength = nextUnixNewLine - srcLineStart;
            }
            else
            {
                srcLineEnd = nextWindowsNewLine + 2;
                srcLineLength = nextWindowsNewLine - srcLineStart;
            }
        }
        else if (nextUnixNewLine != NULL)
        {
            srcLineEnd = nextUnixNewLine + 1;
            srcLineLength = nextUnixNewLine - srcLineStart;
        }
        else if (nextWindowsNewLine != NULL)
        {
            srcLineEnd = nextWindowsNewLine + 2;
            srcLineLength = nextWindowsNewLine - srcLineStart;
        }
        else
        {
            srcLineEnd = (remainingFortuneFileBuffer + remainingFortuneFileBufferSize);
            srcLineLength = remainingFortuneFileBufferSize;
        }

        const size_t bytesProcessed = (srcLineEnd - srcLineStart);
        if (srcLineLength != 0)
        {
            // write to the serialized buffer
            errno_t memCpyErr = memcpy_s(
                currentFortuneWritePtr,
                remainingSerializedFortuneBufferSize,
                remainingFortuneFileBuffer,
                remainingFortuneFileBufferSize);
            assert(memCpyErr == 0);
            currentFortuneWritePtr += bytesProcessed;
            remainingSerializedFortuneBufferSize -= bytesProcessed;
        }
        remainingFortuneFileBuffer += bytesProcessed;
        remainingFortuneFileBufferSize -= bytesProcessed;

        if (srcLineLength == 0 || remainingFortuneFileBufferSize == 0)
        {
            // If we haven't written anything into our fortune buffer, don't record a new fortune.
            if (currentFortuneWritePtrStart != currentFortuneWritePtr)
            {
                // Null-terminate the last fortuned we'd finished processing
                currentFortuneWritePtr[0] = '\0';
                currentFortuneWritePtr += 1;
                remainingSerializedFortuneBufferSize -= 1;

                // Record the filled in fortune buffer
                *nextFortunePtrSlot = currentFortuneWritePtrStart;
                currentFortuneWritePtrStart = currentFortuneWritePtr;
                nextFortunePtrSlot += 1;
                fortuneCount += 1;
            }
        }
    }

    *outFortuneCount = fortuneCount;
    *outFortuneList = outSerializedFortunesBuffer->data;
    return TC_ERR_SUCCESS;
}

int main(
    int argc,
    char** argv)
{
    if (argc < 2)
    {
        PrintUsage();
        return 1;
    }

    const char* fortuneFilePath = argv[1];
    printf("Trying to open %s\n", fortuneFilePath);

    //
    // First, read our file into memory
    //
    tc_buffer_t fortuneFileBuffer;
    tc_err_t err = ReadFileIntoBuffer(fortuneFilePath, &fortuneFileBuffer);
    if (TC_FAILED(err))
    {
        printf("ReadFileIntoBuffer failed! (err=%s)\n", GetErrString(err));
        return 1;
    }
    printf("Read file into buffer!\n");

    //
    // Next, parse it for fortunes
    //
    size_t fortuneCount;
    char** fortunes;
    tc_buffer_t fortunesBuffer;
    err = ParseFortunesFromFileContents(fortuneFileBuffer, &fortuneCount, &fortunes, &fortunesBuffer);
    BufferFree(&fortuneFileBuffer); // regardless of success or failure we can free the file buffer
    if (TC_FAILED(err))
    {
        printf("ParseFortunesFromFileContents failed! (err=%s)\n", GetErrString(err));
        return 1;
    }

    printf("Parsed fortunes! fortuneCount=%zu\n", fortuneCount);
    for (size_t i = 0; i < fortuneCount; ++i)
    {
        printf("Fortune #%03zu: %s", i, fortunes[i]);
    }

    //
    // Finally, select and print a fortune
    //
    srand(time(NULL));
    size_t selectedFortuneIndex = rand() % fortuneCount;
    printf("Summoning a fortune...\n");
    printf("%s\n", fortunes[selectedFortuneIndex]);
    BufferFree(&fortunesBuffer); // regardless of success or failure we can free the file buffer

    return 0;
}
