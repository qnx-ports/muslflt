#include <math.h>
#include <stdint.h>

int __isinf(double x)
{
	union {double f; uint64_t i;} u = {x};
	return (u.i & -1ULL>>1) == 0x7ffULL<<52;
}
