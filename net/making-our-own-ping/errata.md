# pt 2

- typo: x86 64-bit? I think meant x64 64-bit
- typo: wrote System32.dll instead of Kernel32.dll
- code sample shows "MessageBox\0" instead of "MessageBoxA\0" and says things should work

# pt 4

- `handle: *const c_void;` should that be a comma and not a semi-colon?
- NonNull loses constness. it'd be nice to call this out.