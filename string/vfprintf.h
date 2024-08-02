#include "stdio_shim.h"
#include <stdarg.h>

int __musl_vfprintf(FILE *restrict f, const char *restrict fmt, va_list ap);
