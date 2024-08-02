/*
 * Replacement wchar-related functions for use internally
 * Mostly those related to the FILE structure, since
 * we have an alternative shim definition here.
 */
#include "wchar_override.h"
#include <limits.h>
#include <ctype.h>

size_t __musl_wcsnlen(const wchar_t *s, size_t n)
{
	const wchar_t *z = wmemchr(s, 0, n);
	if (z) n = z-s;
	return n;
}

int __musl_fwide(FILE *f, int mode)
{
	FLOCK(f);
	if (mode) {
		/*if (!f->locale) f->locale = MB_CUR_MAX==1
			? C_LOCALE : UTF8_LOCALE;*/
		if (!f->mode) f->mode = mode>0 ? 1 : -1;
	}
	mode = f->mode;
	FUNLOCK(f);
	return mode;
}

wint_t __musl_fputwc_unlocked(wchar_t c, FILE *f)
{
	char mbc[MB_LEN_MAX];
	int l;
	//locale_t *ploc = &CURRENT_LOCALE, loc = *ploc;

	if (f->mode <= 0) fwide(f, 1);
	//*ploc = f->locale;

	if (isascii(c)) {
		c = putc_unlocked(c, f);
	} else if (f->wpos + MB_LEN_MAX < f->wend) {
		l = wctomb((void *)f->wpos, c);
		if (l < 0) c = WEOF;
		else f->wpos += l;
	} else {
		l = wctomb(mbc, c);
		if (l < 0 || __fwritex((void *)mbc, l, f) < l) c = WEOF;
	}
	if (c==WEOF) f->flags |= F_ERR;
	//*ploc = loc;
	return c;
}

wint_t __musl_fputwc(wchar_t c, FILE *f)
{
	FLOCK(f);
	c = __musl_fputwc_unlocked(c, f);
	FUNLOCK(f);
	return c;
}
