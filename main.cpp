
#include <stdio.h>
#include "cputemp.h"

int main()
{
	float temp = get_cpu_temperature();

	printf("%f\n", temp);
}

