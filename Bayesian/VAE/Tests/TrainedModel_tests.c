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
    //registerExitHandler();
    //const char *text_filename = "../Bayesian/VAE/Tests/data/vae_weights.txt";

    //Init_preTrained_VAE_model();

    //int rows = 0;
    //int columns = 0;
    //double file_value;
    //double ref_values[INPUT_DIM*ENCODER_HIDDEN_DIM];
    //double weight_values[INPUT_DIM*ENCODER_HIDDEN_DIM];
    //size_t offset = 0;


    //for(int i = 0; i < ENCODER_LAYERS + DECODER_LAYERS; i++)
    //{
    //    offset += read_reference_text_file(text_filename, ref_values, 2, offset);
    //    CU_ASSERT_TRUE(offset != 0);
    //    rows = ref_values[0];
    //    columns = ref_values[1];

    //    offset += read_reference_text_file(text_filename, ref_values, rows*columns, offset);
    //    CU_ASSERT_TRUE(offset != 0);

    //    get_model_weights(i, 'A', weight_values, rows, columns);

    //    offset += read_reference_text_file(text_filename, ref_values, 2, offset);
    //    CU_ASSERT_TRUE(offset != 0);
    //    rows = ref_values[0];
    //    columns = ref_values[1];

    //    offset += read_reference_text_file(text_filename, ref_values, rows*columns, offset);
    //    CU_ASSERT_TRUE(offset != 0);

    //    get_model_weights(i, 'b', weight_values, rows, columns);
    //}

    //FILE *f = fopen(text_filename, "r");
    //if(!f)
    //{
    //	fprintf(stderr, "Error opening comparison file\n");
    //	exit(1);
    //}

    //int res = 1;
    //int layer = 0;
    //for(int k = 0; k < ENCODER_LAYERS + DECODER_LAYERS; k++)
    //{
    //    res = fscanf(f, "%lf", &file_value);
    //    rows = file_value;
    //    res = fscanf(f, "%lf", &file_value);
    //    columns = file_value;

    //    for(int i=0; i < rows; i++)
    //    {
    //    	for(int j = 0; j < columns; j++)
    //    	{
    //    		if(fscanf(f, "%lf", &file_value) != 1)
    //    		{
    //    			break;
    //    		}

    //    		CU_ASSERT_EQUAL(dump_layer_weights(non_linear_model, layer, 'A', i, j), file_value);
    //    	}
    //    }
    //    res = fscanf(f, "%lf", &file_value);
    //    rows = file_value;
    //    res = fscanf(f, "%lf", &file_value);
    //    columns = file_value;
    //    double imported_model_val;

    //    for(int i = 0; i < rows; i++)
    //    {
    //    	for(int j = 0; j < columns; j++)
    //    	{
    //    		if(fscanf(f, "%lf", &file_value) != 1)
    //    		{
    //    			break;
    //    		}
    //    		imported_model_val = dump_layer_weights(non_linear_model, layer, 'b', i, j);
    //    		CU_ASSERT_EQUAL(imported_model_val, file_value);
    //    		if(imported_model_val != file_value)
    //    		{
    //    			printf("Expected val: %.10f, read val: %.10f\n", file_value, imported_model_val);
    //    		}
    //    	}
    //    }
    //    layer++;
    //}
    //fclose(f);
    //freeTrainedNonLinear(non_linear_model);
}
#define SIZE 5000

void test_encode_decode(void)
{
    registerExitHandler();
    Init_preTrained_VAE_model("../Bayesian/VAE/Models/vae_weights.bin");

    const char *encode_filename = "../Bayesian/VAE/Tests/data/vae_encode.txt";
    const char *decode_filename = "../Bayesian/VAE/Tests/data/vae_decode.txt";

    double ref_values[INPUT_DIM];
    double norm_img[INPUT_DIM];
    double file_value;

    double z_data[LATENT_DIM];
    double logvar_data[LATENT_DIM];
    double x_hat_data[INPUT_DIM];

    Matrix *z_expected = 	allocateMatrix(1,LATENT_DIM);
    Matrix *z = 		allocateMatrix(1,LATENT_DIM);
    Matrix *logvar_expected = 	allocateMatrix(1,LATENT_DIM);
    Matrix *logvar = 		allocateMatrix(1,LATENT_DIM);
    Matrix *x_hat_expected = 	allocateMatrix(1,INPUT_DIM);
    Matrix *x_hat = 		allocateMatrix(1,INPUT_DIM);

    size_t offset = 0;
    offset += read_reference_text_file(encode_filename, ref_values, LATENT_DIM, offset);
    CU_ASSERT_TRUE(offset != 0);
    fillMatrix(z_expected, ref_values);

    offset += read_reference_text_file(encode_filename, ref_values, LATENT_DIM, offset);
    CU_ASSERT_TRUE(offset != 0);
    fillMatrix(logvar_expected, ref_values);
    load_test_img_data(norm_img);

    encode_data(norm_img, z_data, logvar_data);

    fillMatrix(z, z_data);
    fillMatrix(logvar, logvar_data);
    CU_ASSERT_TRUE(cmpMatrix(*z, *z_expected));
    CU_ASSERT_TRUE(cmpMatrix(*logvar, *logvar_expected));

    decode_data(z_data, x_hat_data);

    offset = 0;
    offset += read_reference_text_file(decode_filename, ref_values, INPUT_DIM, offset);
    CU_ASSERT_TRUE(offset != 0);
    fillMatrix(x_hat_expected, ref_values);
    fillMatrix(x_hat, x_hat_data);

    CU_ASSERT_TRUE(cmpMatrix(*x_hat, *x_hat_expected));

    freeMatrix(z_expected);
    freeMatrix(z);
    freeMatrix(logvar_expected);
    freeMatrix(logvar);
    freeMatrix(x_hat_expected);
    freeMatrix(x_hat);
    close_preTrained_VAE_model();
}

static size_t read_reference_text_file(const char* filename, double *values, size_t size, size_t offset)
{
    double file_value;
    FILE *f = fopen(filename, "r");

    if(!f)
    {
    	fprintf(stderr, "Error opening comparison file\n");
        return 0;
    }

    for(size_t i = 0; i < offset; i++)
    {
        if (fscanf(f, "%lf", &file_value) != 1)
        {
            fprintf(stderr, "Error reading file at offset\n");
            fclose(f);
            return 0;
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
