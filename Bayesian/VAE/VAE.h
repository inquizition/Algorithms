typedef struct
{
	bool output_init;
	LM* encoder_fc1;
	LM* encoder_fc2_mean;
	LM* encoder_fc2_logsigma_2;

	LM* decoder_fc1;
	LM* decoder_fc2;
	Matrix* logsigma2;
}VAE;

NLM *InitNLM(int a, int b, int c);


