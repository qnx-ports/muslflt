#define _Addr long
#define _Int64 long
#define _Reg long

// BEGIN QNX
#define __BYTE_ORDER 1234
// END QNX

#define __LONG_MAX 0x7fffffffffffffffL

#ifndef __cplusplus
typedef unsigned wchar_t;
#endif
typedef unsigned wint_t;

typedef int blksize_t;
typedef unsigned int nlink_t;

typedef float float_t;
typedef double double_t;

// QNX: max_align_t removed
