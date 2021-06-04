#ifndef __UTIL_H__
#define __UTIL_H__

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
        structName ## _size_eq_value_size)

//////////////////
// BUFFER HELPERS
#ifndef ARRAYSIZE
// Bug #4: ARRAYSIZE doesn't properly reject pointers
#define ARRAYSIZE(arr)  (sizeof(arr) / sizeof((arr)[0]));
#endif // !ARRAYSIZE

//////////////////
// STRING HELPERS
static_assert(sizeof(size_t) >= sizeof(uint32_t), size_t_atleast_u32);
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

#define AS_SPAN(arr) { .data=(arr), .count=ARRAYSIZE(arr) }

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

DEF_SPAN_T(char, char_span_t);
DEF_SPAN_T(uint8_t, u8_span_t);
DEF_SPAN_T(uint16_t, u16_span_t);

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

static inline void memset_u8(u8_span_t dest, uint8_t byte, size_t count)
{
    dbg_assert(dest.count >= count);
    memset(dest.data, byte, count);
}
#endif // __UTIL_H__

