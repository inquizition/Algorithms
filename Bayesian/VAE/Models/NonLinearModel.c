#include "Matrix.h"

typedef struct
{
	Matrix *decoder_fc1;
	Matrix *decoder_fc2;
	Matrix *logsigma2;
}NLM;

struct NLM *model;

void init_NLM()
{
	model.decoder_fc1 = allocateMatrix(2, 400);
	model.decoder_fc2 = allocateMatrix(400, 784);
	model.logsigma2 = allocateMatrix(1,1);
}

static void decode(NLM model, Matrix *res, double z)
{
	h1 = reLu_matrix(model.decoder_fc1, z);
	model.decoder_fc2 = h1;

}

