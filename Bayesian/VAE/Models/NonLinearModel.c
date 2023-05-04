#include "Matrix.h"
#include "NonLinearModel.h"

struct NLM *model;

void init_NLM()
{
	model.decoder_fc1 = InitLinear(2,400);
	model.decoder_fc2 = InitLinear(2,400);
	model.logsigma2 = allocateMatrix(1,1);
}

static void decode(NLM model, Matrix *res, Matrix *z)
{
	Linear(model.deccoder_fc1, z)
	h1 = reLu_matrix(model.decoder_fc1, z);
	model.decoder_fc2 = h1;

}

