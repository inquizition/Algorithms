
typedef struct
{
	bool output_init;
	Matrix *A;
	Matrix *b;
	Matrix *output;
}LM;

void freeLinear(LM *linear);
LM *InitLinear(int a, int b);
void Linear(LM *m, Matrix *input);
void print_linear(LM *m);
