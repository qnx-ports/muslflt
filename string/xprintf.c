/* snprintf, sprintf etc. exported for use downstream */
#include "stdio_shim.h"
#include "vfprintf.h"

#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

int __musl_fprintf(FILE *restrict f, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __musl_vfprintf(f, fmt, ap);
	va_end(ap);
	return ret;
}

struct cookie {
	char *s;
	size_t n;
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t sn_write(FILE *f, const unsigned char *s, size_t l)
{
	struct cookie *c = f->cookie;
	size_t k = MIN(c->n, f->wpos - f->wbase);
	if (k) {
		memcpy(c->s, f->wbase, k);
		c->s += k;
		c->n -= k;
	}
	k = MIN(c->n, l);
	if (k) {
		memcpy(c->s, s, k);
		c->s += k;
		c->n -= k;
	}
	*c->s = 0;
	f->wpos = f->wbase = f->buf;
	/* pretend to succeed, even if we discarded extra data */
	return l;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	unsigned char buf[1];
	char dummy[1];
	struct cookie c = { .s = n ? s : dummy, .n = n ? n-1 : 0 };
	FILE f = {
		.lbf = EOF,
		.write = sn_write,
		.lock = -1,
		.buf = buf,
		.cookie = &c,
	};

	if (n > INT_MAX) {
		errno = EOVERFLOW;
		return -1;
	}

	*c.s = 0;
	return __musl_vfprintf(&f, fmt, ap);
}

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	return vsnprintf(s, INT_MAX, fmt, ap);
}

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return ret;
}

int sprintf(char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsprintf(s, fmt, ap);
	va_end(ap);
	return ret;
}
