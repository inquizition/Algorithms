#include "TrainedModel_tests.h"

static void registerExitHandler(void)
{
    atexit(freeAllMatrices);
}

static void compare_matrix_with_file(Matrix *m, const char *filename);
static void compare_model_with_ground_truth(PTNLM *ptnlm, const char *filename);

void test_import_trained_model(void)
{
    registerExitHandler();
    const char *text_filename = "../Bayesian/VAE/Tests/data/vae_weights.txt";

    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");

//####################################
    //double file_value;

    //FILE *f = fopen(text_filename, "r");
    //if(!f)
    //{
    //	fprintf(stderr, "Error opening comparison file\n");
    //	exit(1);
    //}

    //while(fscanf(f, "%lf", &file_value) == 1)
    //{
    //    printf("True Value: %0.10f\n", file_value);
    //}
    //fclose(f);

    //uint8_t i;
    //for(i = 0; i < ENCODER_LAYERS + DECODER_LAYERS; i++)
    //{
    //    print_layer(non_linear_model, i);
    //}
    compare_model_with_ground_truth(non_linear_model, text_filename);
//####################################

    freeTrainedNonLinear(non_linear_model);
}

static void compare_model_with_ground_truth(PTNLM *ptnlm, const char *filename)
{
    int rows = 0;
    int columns = 0;
    double file_value;

    FILE *f = fopen(filename, "r");
    if(!f)
    {
    	fprintf(stderr, "Error opening comparison file\n");
    	exit(1);
    }

    int res = 1;
    int layer = 0;
    for(int k = 0; k < ENCODER_LAYERS + DECODER_LAYERS; k++)
    {
	res = fscanf(f, "%lf", &file_value);
	rows = file_value;
	res = fscanf(f, "%lf", &file_value);
	columns = file_value;

	for(int i=0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			if(fscanf(f, "%lf", &file_value) != 1)
			{
				fclose(f);
				break;
			}

			CU_ASSERT_EQUAL(dump_layer(ptnlm, layer, 'A', i, j), file_value);
		}
	}
	res = fscanf(f, "%lf", &file_value);
	rows = file_value;
	res = fscanf(f, "%lf", &file_value);
	columns = file_value;

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			if(fscanf(f, "%lf", &file_value) != 1)
			{
				fclose(f);
				break;
			}
			CU_ASSERT_EQUAL(dump_layer(ptnlm, layer, 'b', i, j), file_value);
		}
	}
	layer++;
    }
    fclose(f);
    return;
}

static void compare_matrix_with_file(Matrix *m, const char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f)
	{
		fprintf(stderr, "Error opening comparison file\n");
		exit(1);
	}

	int rows = m->rows;
	int columns = m->columns;
	double file_value;
	int mismatch_count = 0;

	if(fscanf(f, "%lf", &file_value) != 1)
	{
		fprintf(stderr, "Error reading value from file\n");
		fclose(f);
		exit(1);
	}

	for(int i=0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			if(fscanf(f, "%lf", &file_value) != 1)
			{
				fprintf(stderr, "Error reading value from file\n");
				fclose(f);
				exit(1);
			}
			if(m->data[i][j] != file_value)
			{
				printf("Mismatch at [%d][%d]: Matrix value = %.10f, file value = %.10f\n", i, j, m->data[i][j], file_value);
				mismatch_count++;
			}
		}
	}

	fclose(f);
	if(mismatch_count == 0)
	{
		printf("All values match between the matrix and the file.\n");
	}
	else
	{
		printf("Total mismatches: %d\n", mismatch_count);
	}
		
}
