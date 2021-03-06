#include "pch.h"

bool
str_starts_with(
    str_t s,
    str_t prefix)
{
    if (s.len < prefix.len)
    {
        return false;
    }
    str_t truncated_s = {.bytes = s.bytes, .len = prefix.len };
    return str_eq(truncated_s, prefix);
}

str_t
make_str(
    const char* s)
{
    if (s == NULL)
    {
        return (str_t) {0};
    }

    size_t len = strlen(s);
    char* buffer = (char*)malloc(len + 1);
    if (buffer == NULL)
    {
        free(buffer);
        return (str_t) {0};
    }

    memcpy(buffer, s, len);
    buffer[len] = 0;  // null terminate

    return (str_t) { .bytes = buffer, .len = len };
}

char_span_t
get_first_split(
    char_span_t buffer,
    char split_char)
{
    // Skip over any leading split chars
    while (buffer.count > 0 && buffer.data[0] == split_char)
    {
        SPAN_ADV(buffer, 1);
    }

    // The first split of an empty span is another empty span
    if (buffer.count == 0)
    {
        return (char_span_t){0};
    }

    dbg_assert(buffer.data != NULL);

    char* split_char_ptr = memchr(buffer.data, split_char, buffer.count);
    if (split_char_ptr == NULL)
    {
        // If there is no split character than the first split is the entire span
        return buffer;
    }

    return (char_span_t) { .data = buffer.data, .count=(split_char_ptr - buffer.data) };
}

char_span_t
get_next_split(
    const char_span_t current_split,
    const char_span_t full_span,
    char split_char)
{
    dbg_assert(current_split.count <= full_span.count);
    dbg_assert(current_split.data >= full_span.data);
    dbg_assert(current_split.data <= full_span.data + full_span.count);

    // Skip over any leading split chars
    size_t current_split_offset = current_split.data - full_span.data;
    char_span_t remaining_span = {
        .data = current_split.data + current_split.count,
        .count = full_span.count - current_split.count - current_split_offset };
    return get_first_split(remaining_span, split_char);
}

#define DEF_SPAN_MEMCPY(funcName, destSpanType, srcSpanType) \
    void \
    funcName( \
        destSpanType dest, \
        srcSpanType src) \
    { \
        /* dest should be bigger than the src */ \
        dbg_assert(dest.count >= src.count); \
        /* dest and src shouldn't overlap */ \
        dbg_assert( \
            src.data >= dest.data + dest.count || \
            dest.data >= src.data + src.count); \
                                                \
        memcpy(dest.data, src.data, src.count * sizeof(src.data[0])); \
    }

DEF_SPAN_MEMCPY(memcpy_u8, u8_span_t, cu8_span_t)
DEF_SPAN_MEMCPY(memcpy_u16, u16_span_t, cu16_span_t)

#undef DEF_SPAN_MEMCPY
