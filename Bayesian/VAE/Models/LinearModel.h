
typedef struct
{
	Matrix *A;
	Matrix *b;
}LM;

void freeLinear(LM *linear);
LM *InitLinear(int a, int b);
