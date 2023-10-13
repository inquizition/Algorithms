// Non linear model
typedef struct
{
	bool output_init;
	LM* decoder_fc1;
	LM* decoder_fc2;
	Matrix* logsigma2;
}NLM;

NLM *InitNLM(int a, int b, int c);

void freeNonLinear(NLM *nonLinear);
void decode(NLM model, Matrix *z);
void printNonLinear(NLM *nonLinear);
double cost_function(Matrix *X, NLM *model, int K);
