#include <math.h>
#include <stdint.h>

int __isinfl(long double x)
{
	return __fpclassifyl(x) == FP_INFINITE;
}
