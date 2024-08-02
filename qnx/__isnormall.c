#include <math.h>
#include <stdint.h>

int __isnormall(long double x)
{
	return __fpclassifyl(x) == FP_NORMAL;
}
