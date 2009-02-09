

#include <Fdo.h>
#include <wchar.h>

#ifdef _WIN32
#define OGR_API __declspec(dllexport)
#include <windows.h>
#else
#define OGR_API
#endif

#ifndef _WIN32
#define _wcsnicmp wcsncasecmp
#define _wcsicmp wcscasecmp
#endif

#include <ogrsf_frmts.h>


#ifdef _WIN32_DISABLE_XXX
#define W2A(x) \
    int len##x = (int)wcslen(x)*4+1; \
    char* mb##x = (char*)alloca(len##x); \
    WideCharToMultiByte(CP_UTF8, 0, x, -1, mb##x, len##x, 0, 0);

#define A2W(x) \
    int len##x = (int)strlen(x)*4+4; \
    wchar_t* w##x = (wchar_t*)alloca(len##x); \
    MultiByteToWideChar(CP_UTF8, 0, x, -1, w##x, len##x);
#else
//TODO: Fix this for Linux, so that it always does UTF8 conversion
#define A2W(x) \
    size_t len##x = strlen(x)*4+4; \
    wchar_t* w##x = (wchar_t*)alloca(len##x); \
    mbstowcs(w##x, x, len##x);
    
#define W2A(x) \
    size_t len##x = wcslen(x)*4+1; \
    char* mb##x = (char*)alloca(len##x); \
    wcstombs(mb##x, x, len##x);
#endif


