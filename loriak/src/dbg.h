#ifndef __DBG_H__
#define __DBG_H__

#define STATIC_MSG(msg, l) STATIC_MSG2(msg, l)
#define STATIC_MSG2(msg,l) on_line_##l##__##msg
#define static_assert(x, msg) extern char STATIC_MSG(msg, __LINE__) [(x)?1:-1]

// BUG #1: assert statements aren't working?
#ifdef DBG
#define dbg_assert(DBG_EXPR)                       assert((DBG_EXPR))
#else // DBG
#define dbg_assert(DBG_EXPR)
#endif // DBG

#endif // __DBG_H__

