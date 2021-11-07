#pragma once

#ifndef _Out_
#define _Out_
#endif // _Out_

#ifndef _Outptr_
#define _Outptr_
#endif // _Outptr_

#define UNREFERENCED(x) do { ((void)(x)); } while (0)
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER UNREFERENCED
#endif // UNREFERENCED_PARAMETER
