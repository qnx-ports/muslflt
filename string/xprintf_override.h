/* 
 * Note: This header is intended to be used internally
 * within this library. No header should be exported
 * by this library, as it intends to be 100% compatible
 * with libc headers
 */

#pragma once

int __musl_fprintf(FILE *restrict f, const char *restrict fmt, ...);

#define fprintf(f, ...) __musl_fprintf(f, __VA_ARGS__)

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...);
