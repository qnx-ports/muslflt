/*
 * This file contains shims for types defined in
 * musl's stdio_impl.h. These shims are only intended
 * for use within this library, and should NOT be exported
 * because they will certainly not match with the system
 * libc version.
 *
 * In particular, the FILE structure defined in this file
 * is ONLY supposed to be used as a pseudo-FILE backed by
 * a string (char *). Any other use will cause undefined
 * behavior.
 */
#pragma once
#include <stdlib.h>

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32
#define F_SVB 64
#define F_APP 128

typedef size_t off_t;

struct _IO_FILE;

typedef struct _IO_FILE FILE;

struct _IO_FILE {
	unsigned flags;
	unsigned char *rpos, *rend;
	int (*close)(FILE *);
	unsigned char *wend, *wpos;
	unsigned char *mustbezero_1;
	unsigned char *wbase;
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	unsigned char *buf;
	size_t buf_size;
	FILE *prev, *next;
	int fd;
	int pipe_pid;
	long lockcount;
	int mode;
	volatile int lock;
	int lbf;
	void *cookie;
	off_t off;
	char *getln_buf;
	void *mustbezero_2;
	unsigned char *shend;
	off_t shlim, shcnt;
	FILE *prev_locked, *next_locked;
	struct __locale_struct *locale;
};

#define EOF (-1)

#define ferror(f) ((f)->flags & F_ERR)

size_t __musl_fwritex(const unsigned char *, size_t, FILE *);
#define __fwritex(s, l, f) __musl_fwritex(s, l, f)
int __musl_towrite(FILE *f);
#define __towrite(f) __musl_towrite(f)

/* File locks are no-op */
#define FFINALLOCK(f) ((void) 0)
#define FLOCK(f) ((void) 0)
#define FUNLOCK(f) ((void) 0)

#define putc_unlocked(c, f) \
	( *(f)->wpos++ = (unsigned char)(c) )
