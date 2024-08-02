#include <math.h>
#include <stdint.h>

int __isfinitel(long double x)
{
	return __fpclassifyl(x) > FP_INFINITE;
}
