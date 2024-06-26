#include "TrainedModel_tests.h"

static void registerExitHandler(void)
{
    atexit(freeAllMatrices);
}

static void compare_matrix_with_file(Matrix *m, const char *filename);
static void compare_model_with_ground_truth(PTNLM *ptnlm, const char *filename);
static size_t read_reference_text_file(const char* filename, double *values, size_t size, size_t offset);
static void load_test_img_data(double *img_data);

void test_import_trained_model(void)
{
    registerExitHandler();
    const char *text_filename = "../Bayesian/VAE/Tests/data/vae_weights.txt";

    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");

    int rows = 0;
    int columns = 0;
    double file_value;

    FILE *f = fopen(text_filename, "r");
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

			CU_ASSERT_EQUAL(dump_layer_weights(non_linear_model, layer, 'A', i, j), file_value);
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
			CU_ASSERT_EQUAL(dump_layer_weights(non_linear_model, layer, 'b', i, j), file_value);
		}
	}
	layer++;
    }
    fclose(f);
    freeTrainedNonLinear(non_linear_model);
}
#define SIZE 5000

void test_encode(void)
{
    registerExitHandler();
    const char *text_filename = "../Bayesian/VAE/Tests/data/vae_encode.txt";


    double ref_values[LATENT_DIM];
    double norm_img[INPUT_DIM];
    double file_value;
    Matrix *mat_img = allocateMatrix(1,INPUT_DIM);
    Matrix *z_expected = allocateMatrix(1,LATENT_DIM);
    Matrix *z = allocateMatrix(1,LATENT_DIM);
    Matrix *logvar_expected = allocateMatrix(1,LATENT_DIM);
    Matrix *logvar = allocateMatrix(1,LATENT_DIM);
    size_t offset = 0;

    offset += read_reference_text_file(text_filename, ref_values, LATENT_DIM, offset);
    fillMatrix(z_expected, ref_values);

    offset += read_reference_text_file(text_filename, ref_values, LATENT_DIM, offset);
    fillMatrix(logvar_expected, ref_values);

    load_test_img_data(norm_img);
    fillMatrix(mat_img, norm_img);

    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");

    encode(*non_linear_model, mat_img, z, logvar);

    CU_ASSERT_TRUE(cmpMatrix(*z, *z_expected));
    CU_ASSERT_TRUE(cmpMatrix(*logvar, *logvar_expected));

    freeTrainedNonLinear(non_linear_model);
    freeMatrix(mat_img);
    freeMatrix(z);
    freeMatrix(logvar);
    freeMatrix(z_expected);
    freeMatrix(logvar_expected);
}

void test_decode(void)
{
    registerExitHandler();
    const char *text_filename = "../Bayesian/VAE/Tests/data/vae_decode.txt";


    double ref_values[INPUT_DIM];
    double norm_img[INPUT_DIM];
    double file_value;
    Matrix *mat_img = allocateMatrix(1,INPUT_DIM);
    Matrix *x_hat_expected = allocateMatrix(1,INPUT_DIM);
    Matrix *x_hat = allocateMatrix(1,INPUT_DIM);
    Matrix *z = allocateMatrix(1,LATENT_DIM);
    Matrix *logvar = allocateMatrix(1,LATENT_DIM);
    size_t offset = 0;

    offset += read_reference_text_file(text_filename, ref_values, INPUT_DIM, offset);
    fillMatrix(x_hat_expected, ref_values);

    load_test_img_data(norm_img);
    fillMatrix(mat_img, norm_img);

    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");

    encode(*non_linear_model, mat_img, z, logvar);
    decode(*non_linear_model, z, x_hat);

    CU_ASSERT_TRUE(cmpMatrix(*x_hat, *x_hat_expected));

    freeTrainedNonLinear(non_linear_model);
    freeMatrix(mat_img);
    freeMatrix(z);
    freeMatrix(logvar);
    freeMatrix(x_hat);
    freeMatrix(x_hat_expected);
}

static size_t read_reference_text_file(const char* filename, double *values, size_t size, size_t offset)
{
    double file_value;
    FILE *f = fopen(filename, "r");

    if(!f)
    {
    	fprintf(stderr, "Error opening comparison file\n");
    	exit(1);
    }

    for(size_t i = 0; i < offset; i++)
    {
        if (fscanf(f, "%lf", &file_value) != 1)
        {
            fprintf(stderr, "Error reading file at offset\n");
            fclose(f);
            exit(1);
        }
    }

    for(size_t i = 0; i < size; i++)
    {
	if(fscanf(f, "%lf", &file_value) != 1)
	{
		fclose(f);
		break;
	}
	values[i] = file_value;
    }
    fclose(f);
    return size;
}

static void load_test_img_data(double *img_data)
{

    FILE *file;
    int img[SIZE];
    
    file = fopen("../data/img.bin", "rb");
    fread(img, sizeof(int), SIZE, file);
    fclose(file);

    for(int i = 0; i < 28; i++)
    {
    	for(int j = 0; j < 28; j++)
    	{
       	    img_data[j + (i*28)] = img[j + (i*28)]/255.0;
    	}
    }
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
