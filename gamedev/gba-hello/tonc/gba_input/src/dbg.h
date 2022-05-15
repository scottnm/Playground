//
// dbg.h
// This file provides debug utilities
//
#pragma once

// BUG #1: assert statements aren't working?
#ifdef DBG
#define dbg_assert(DBG_EXPR)                       assert((DBG_EXPR))
#else // DBG
#define dbg_assert(DBG_EXPR)
#endif // DBG
