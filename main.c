#include "Matrix.h"
#include "LinearModel.h"


int main(void)
{
	printf("Start application.");

	LM *linear = InitLinear(4,4);

	print_matrix(*linear->A);
	print_matrix(*linear->b);

	freeLinear(linear);
	return 1;
}
