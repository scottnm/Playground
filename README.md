# GBA Dev Notes

## Compiler quirks

The GBA compiler used for this (devkitadv) is locked on a really old version of GCC (3.2). This has a few unfortunate caveats:

1. c99 is the highest standard supported
2. `#pragma once` isn't supported
