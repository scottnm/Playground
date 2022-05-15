#pragma once

#include <stdint.h> // size_t

typedef struct
{
    void* data;
    size_t len;
} tc_buffer_t;

static inline
tc_err_t BufferAlloc(
    size_t size,
    _Out_ tc_buffer_t* outBuffer
    )
{
    void* buffer = malloc(size);
    if (buffer == NULL)
    {
        return TC_ERR_OOM;
    }

    *outBuffer = (tc_buffer_t){
        .data = buffer,
        .len = size,
    };

    return TC_ERR_SUCCESS;
}


static inline
void BufferFree(
    _Inout_ tc_buffer_t* buffer)
{
    free(buffer->data);
    *buffer = (tc_buffer_t) { 0 };
}

static inline
void BufferSwap(
    _Inout_ tc_buffer_t* a,
    _Inout_ tc_buffer_t* b)
{
    tc_buffer_t tmp = *a;
    *a = *b;
    *b = tmp;
}
