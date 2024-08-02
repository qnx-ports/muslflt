#include "stdio_shim.h"
#include <stdarg.h>
#undef __FILE_T
#include <wchar.h>

int __musl_vfwprintf(FILE *restrict f, const wchar_t *restrict fmt, va_list ap);
