#include <math.h>
#include <stdint.h>

int __isnormal(double x)
{
	union {double f; uint64_t i;} u = {x};
	return ((u.i+(1ULL<<52)) & -1ULL>>1) >= 1ULL<<53;
}
