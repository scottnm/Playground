#ifndef __DBG_H__
#define __DBG_H__

#ifdef DBG
#define dbg_assert(DBG_EXPR)                       assert((DBG_EXPR))
#else // DBG
#define dbg_assert(DBG_EXPR)
#endif // DBG

#endif // __DBG_H__

