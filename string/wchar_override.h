/*
 * This header replaces some functions in system's
 * wchar.h with our own versions, because of our
 * own definition of FILE.
 *
 * To use the replacements, include this header
 * AFTER the system one.
 *
 * This header SHALL NOT be exported.
 */
#pragma once
#include "stdio_shim.h"
#undef __FILE_T
#include <wchar.h>

size_t __musl_wcsnlen(const wchar_t *s, size_t n);
#define wcsnlen(s, n) __musl_wcsnlen(s, n)

int __musl_fwide(FILE *f, int mode);
#define fwide(f, mode) __musl_fwide(f, mode)

wint_t __musl_fputwc(wchar_t c, FILE *f);
#define fputwc(c, f) __musl_fputwc(c, f)
