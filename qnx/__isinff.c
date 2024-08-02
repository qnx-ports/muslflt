#include <math.h>
#include <stdint.h>

int __isinff(float x)
{
	union {float f; uint32_t i;} u = {x};
	return (u.i & 0x7fffffff) == 0x7f800000;
}
