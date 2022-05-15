//
// util.h
// This file provides helper macros and non-gba c utilities.
//

#pragma once

/////////////////
// UTILITY MACROS
#define STATEMENT(X) do { (X); } while(0)
#define UNREF(X) STATEMENT((void)(X))
#define TODO(TODO_MSG) dbg_assert(false && "Todo! " TODO_MSG)

///////////////
// TYPE HELPERS
#define STRUCT_MEMBER(structName, member) \
    ( ((structName*)0)->member )

#define ASSERT_STRONG_TYPEDEF_SIZE(structName) \
    static_assert( \
        sizeof(structName) == sizeof(STRUCT_MEMBER(structName, value)), \
        "Strong typedef failed! sizeof '" #structName "' is larger than the size of it's wrapped value field")

//////////////////
// BUFFER HELPERS
#ifndef ARRAYSIZE
// Bug #4: ARRAYSIZE doesn't properly reject pointers
#define ARRAYSIZE(arr)  (sizeof(arr) / sizeof((arr)[0]))
#endif // !ARRAYSIZE

//////////////////
// STRING HELPERS
typedef struct str_t {
    char* bytes;
    size_t len;
} str_t;

#define cstr(s) { .bytes=(s), .len=ARRAYSIZE(s)-1 }
static inline bool cstr_eq(const char* a, const char* b) { return strcmp(a, b) == 0; }
static inline bool mem_eq(const void* a, const void* b, size_t len) { return memcmp(a, b, len) == 0; }
static inline bool str_eq(str_t a, str_t b) { return a.len == b.len && mem_eq(a.bytes, b.bytes, a.len); }

bool
str_starts_with(
    str_t s,
    str_t prefix);

str_t
make_str(
    const char* s);

///////////////
// SPAN HELPERS
#define DEF_SPAN_T(type, typename) \
    typedef struct typename { \
        type* data; \
        size_t count; \
    } typename

DEF_SPAN_T(char, char_span_t);
DEF_SPAN_T(uint8_t, u8_span_t);
DEF_SPAN_T(uint16_t, u16_span_t);

DEF_SPAN_T(const char, cchar_span_t);
DEF_SPAN_T(const uint8_t, cu8_span_t);
DEF_SPAN_T(const uint16_t, cu16_span_t);

#undef DEF_SPAN_T

#define as_span(spanType, arr) (spanType) { .data=(arr), .count=ARRAYSIZE(arr) }

#define SPAN_ADV(span, advCount) \
    do { \
        const size_t localAdvCount = (advCount); \
        (span).data += localAdvCount; \
        (span).count -= localAdvCount; \
    } while (0) \

#define SPAN_FIRST(span, cnt) \
    { \
        .data = (span).data, \
        .count = min((span).count, cnt), \
    }

char_span_t
get_first_split(
    char_span_t buffer,
    char split_char);

char_span_t
get_next_split(
    const char_span_t current_split,
    const char_span_t full_span,
    char split_char);

#define split_by(span_t, iter_var_name, buffer, split_char) \
    for ( \
        span_t iter_var_name = get_first_split((buffer), (split_char)); \
        iter_var_name.data != NULL; \
        iter_var_name = get_next_split((iter_var_name), (buffer), (split_char)) \
        ) \

static inline void memset_u8(u8_span_t dest, uint8_t byte) { memset(dest.data, byte, dest.count); }

#define DECL_SPAN_MEMCPY(funcName, destSpanType, srcSpanType) \
    void \
    funcName( \
        destSpanType dest, \
        srcSpanType src);

DECL_SPAN_MEMCPY(memcpy_u8, u8_span_t, cu8_span_t)
DECL_SPAN_MEMCPY(memcpy_u16, u16_span_t, cu16_span_t)

#undef DECL_SPAN_MEMCPY
