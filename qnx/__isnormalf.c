#include <math.h>
#include <stdint.h>

int __isnormalf(float x)
{
	union {float f; uint32_t i;} u = {x};
	return ((u.i+0x00800000) & 0x7fffffff) >= 0x01000000;
}
