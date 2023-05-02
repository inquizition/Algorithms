// Non linear model
#include "LinearModel.h"

typedef struct
{
	bool output_init;
	LM* decoder_fc1;
	LM* decoder_fc2;
	Matrix* logsigma2;
}NLM;

