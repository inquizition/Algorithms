#include "model.h"
static size_t load_weights(char *filename, Matrix *m, size_t size, size_t offset)
{
    double *weights = (double *)malloc(size * sizeof(double));
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Error opening weights file\n");
        exit(1);
    }
    fseek(f, offset * sizeof(double), SEEK_SET);
    size_t read_count = fread(weights, sizeof(double), size, f);
    fclose(f);

    int rows = m->rows;
    int columns = m->columns;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            m->data[i][j] = weights[i * columns + j];
        }
    }

    free(weights);
    return read_count;
}
PTNLM *InitTrainedNLM_Model(void)
{
    PTNLM *model;
    model = (PTNLM*) malloc(sizeof(PTNLM));
    model->encoder_layer_0 = InitLinear(784, 70);
    model->encoder_layer_1 = InitLinear(70, 2);
    model->encoder_layer_2 = InitLinear(70, 2);
    model->decoder_layer_0 = InitLinear(2, 50);
    model->decoder_layer_1 = InitLinear(50, 784);
    return model;
}
void decode(PTNLM model, Matrix *z, Matrix *output_mu)
{
    Linear(model.decoder_layer_0, *z);
    Matrix *output_relu = allocateMatrix( model.decoder_layer_0->output->rows, model.decoder_layer_0->output->columns);
    copyMatrix(*model.decoder_layer_0->output, output_relu);
    reLu_matrix(output_relu);
    Linear(model.decoder_layer_1, *output_relu);
    sigmoid_matrix(model.decoder_layer_1->output);
    copyMatrix(*model.decoder_layer_1->output, output_mu);
    freeMatrix(output_relu);
}
void encode(PTNLM model, Matrix *x, Matrix *output_mu, Matrix *output_logvar)
{
    Linear(model.encoder_layer_0, *x);
    Matrix *output_relu = allocateMatrix( model.encoder_layer_0->output->rows, model.encoder_layer_0->output->columns);
    copyMatrix(*model.encoder_layer_0->output, output_relu);
    reLu_matrix(output_relu);
    Linear(model.encoder_layer_1, *output_relu);
    Linear(model.encoder_layer_2, *output_relu);
    copyMatrix(*model.encoder_layer_1->output, output_mu);
    copyMatrix(*model.encoder_layer_2->output, output_logvar);
    freeMatrix(output_relu);
}
void load_trained_weights(PTNLM *model, char *filename)
{
    
    size_t offset = 0;offset += load_weights(filename, &model->encoder_layer_0->A[0], 784 * 70, offset);
    offset += load_weights(filename, &model->encoder_layer_0->b[0], 70, offset);
    offset += load_weights(filename, &model->encoder_layer_1->A[0], 70 * 2, offset);
    offset += load_weights(filename, &model->encoder_layer_1->b[0], 2, offset);
    offset += load_weights(filename, &model->encoder_layer_2->A[0], 70 * 2, offset);
    offset += load_weights(filename, &model->encoder_layer_2->b[0], 2, offset);
    offset += load_weights(filename, &model->decoder_layer_0->A[0], 2 * 50, offset);
    offset += load_weights(filename, &model->decoder_layer_0->b[0], 50, offset);
    offset += load_weights(filename, &model->decoder_layer_1->A[0], 50 * 784, offset);
    offset += load_weights(filename, &model->decoder_layer_1->b[0], 784, offset);
}
void freeTrainedNonLinear(PTNLM *model)
{
    freeLinear(model->encoder_layer_0);
    freeLinear(model->encoder_layer_1);
    freeLinear(model->encoder_layer_2);
    freeLinear(model->decoder_layer_0);
    freeLinear(model->decoder_layer_1);
free(model);
}
static void print_encoder_layer_0( PTNLM* model )
{
    print_linear(model->encoder_layer_0);
    print_matrix(*model->encoder_layer_0->A, "\nA:");
    print_matrix(*model->encoder_layer_0->b, "b:");
}
static void print_encoder_layer_1( PTNLM* model )
{
    print_linear(model->encoder_layer_1);
    print_matrix(*model->encoder_layer_1->A, "\nA:");
    print_matrix(*model->encoder_layer_1->b, "b:");
}
static void print_encoder_layer_2( PTNLM* model )
{
    print_linear(model->encoder_layer_2);
    print_matrix(*model->encoder_layer_2->A, "\nA:");
    print_matrix(*model->encoder_layer_2->b, "b:");
}
static void print_decoder_layer_0( PTNLM* model )
{
    print_linear(model->decoder_layer_0);
    print_matrix(*model->decoder_layer_0->A, "\nA:");
    print_matrix(*model->decoder_layer_0->b, "b:");
}
static void print_decoder_layer_1( PTNLM* model )
{
    print_linear(model->decoder_layer_1);
    print_matrix(*model->decoder_layer_1->A, "\nA:");
    print_matrix(*model->decoder_layer_1->b, "b:");
}
static double dump_weights_encoder_layer_0( PTNLM* model, char matrix, int r, int c )
{
    if(matrix == 'A') {
        if((r > model->encoder_layer_0->A->rows) || (c > model->encoder_layer_0->A->columns)) {
            printf("Matrix A encoder_layer_0 rows/columns out of bounds!");
            exit(1);
        }
        return model->encoder_layer_0->A->data[r][c];
    }
    else if(matrix == 'b') {
        if((r > model->encoder_layer_0->b->rows) || (c > model->encoder_layer_0->b->columns)) {
            printf("Matrix b encoder_layer_0 rows/columns out of bounds!");
            exit(1);
        }
        return model->encoder_layer_0->b->data[r][c];
    }
    printf("Invalid Matix choice!");
    exit(1);
}

static double dump_weights_encoder_layer_1( PTNLM* model, char matrix, int r, int c )
{
    if(matrix == 'A') {
        if((r > model->encoder_layer_1->A->rows) || (c > model->encoder_layer_1->A->columns)) {
            printf("Matrix A encoder_layer_1 rows/columns out of bounds!");
            exit(1);
        }
        return model->encoder_layer_1->A->data[r][c];
    }
    else if(matrix == 'b') {
        if((r > model->encoder_layer_1->b->rows) || (c > model->encoder_layer_1->b->columns)) {
            printf("Matrix b encoder_layer_1 rows/columns out of bounds!");
            exit(1);
        }
        return model->encoder_layer_1->b->data[r][c];
    }
    printf("Invalid Matix choice!");
    exit(1);
}

static double dump_weights_encoder_layer_2( PTNLM* model, char matrix, int r, int c )
{
    if(matrix == 'A') {
        if((r > model->encoder_layer_2->A->rows) || (c > model->encoder_layer_2->A->columns)) {
            printf("Matrix A encoder_layer_2 rows/columns out of bounds!");
            exit(1);
        }
        return model->encoder_layer_2->A->data[r][c];
    }
    else if(matrix == 'b') {
        if((r > model->encoder_layer_2->b->rows) || (c > model->encoder_layer_2->b->columns)) {
            printf("Matrix b encoder_layer_2 rows/columns out of bounds!");
            exit(1);
        }
        return model->encoder_layer_2->b->data[r][c];
    }
    printf("Invalid Matix choice!");
    exit(1);
}

static double dump_weights_decoder_layer_0( PTNLM* model, char matrix, int r, int c )
{
    if(matrix == 'A') {
        if((r > model->decoder_layer_0->A->rows) || (c > model->decoder_layer_0->A->columns)) {
            printf("Matrix A decoder_layer_0 rows/columns out of bounds!");
            exit(1);
        }
        return model->decoder_layer_0->A->data[r][c];
    }
    else if(matrix == 'b') {
        if((r > model->decoder_layer_0->b->rows) || (c > model->decoder_layer_0->b->columns)) {
            printf("Matrix b decoder_layer_0 rows/columns out of bounds!");
            exit(1);
        }
        return model->decoder_layer_0->b->data[r][c];
    }
    printf("Invalid Matix choice!");
    exit(1);
}

static double dump_weights_decoder_layer_1( PTNLM* model, char matrix, int r, int c )
{
    if(matrix == 'A') {
        if((r > model->decoder_layer_1->A->rows) || (c > model->decoder_layer_1->A->columns)) {
            printf("Matrix A decoder_layer_1 rows/columns out of bounds!");
            exit(1);
        }
        return model->decoder_layer_1->A->data[r][c];
    }
    else if(matrix == 'b') {
        if((r > model->decoder_layer_1->b->rows) || (c > model->decoder_layer_1->b->columns)) {
            printf("Matrix b decoder_layer_1 rows/columns out of bounds!");
            exit(1);
        }
        return model->decoder_layer_1->b->data[r][c];
    }
    printf("Invalid Matix choice!");
    exit(1);
}
static double dump_outputs_encoder_layer_0( PTNLM* model, int r, int c )
{
    if((r > model->encoder_layer_0->output->rows) || (c > model->encoder_layer_0->output->columns)) {
        printf("encoder_layer_0 output rows/columns out of bounds!");
        exit(1);
    }
    if(model->encoder_layer_0->output_init) {
        return model->encoder_layer_0->output->data[r][c];
    }
    printf("No output found!");
    exit(1);
}

static double dump_outputs_encoder_layer_1( PTNLM* model, int r, int c )
{
    if((r > model->encoder_layer_1->output->rows) || (c > model->encoder_layer_1->output->columns)) {
        printf("encoder_layer_1 output rows/columns out of bounds!");
        exit(1);
    }
    if(model->encoder_layer_1->output_init) {
        return model->encoder_layer_1->output->data[r][c];
    }
    printf("No output found!");
    exit(1);
}

static double dump_outputs_encoder_layer_2( PTNLM* model, int r, int c )
{
    if((r > model->encoder_layer_2->output->rows) || (c > model->encoder_layer_2->output->columns)) {
        printf("encoder_layer_2 output rows/columns out of bounds!");
        exit(1);
    }
    if(model->encoder_layer_2->output_init) {
        return model->encoder_layer_2->output->data[r][c];
    }
    printf("No output found!");
    exit(1);
}

static double dump_outputs_decoder_layer_0( PTNLM* model, int r, int c )
{
    if((r > model->decoder_layer_0->output->rows) || (c > model->decoder_layer_0->output->columns)) {
        printf("decoder_layer_0 output rows/columns out of bounds!");
        exit(1);
    }
    if(model->decoder_layer_0->output_init) {
        return model->decoder_layer_0->output->data[r][c];
    }
    printf("No output found!");
    exit(1);
}

static double dump_outputs_decoder_layer_1( PTNLM* model, int r, int c )
{
    if((r > model->decoder_layer_1->output->rows) || (c > model->decoder_layer_1->output->columns)) {
        printf("decoder_layer_1 output rows/columns out of bounds!");
        exit(1);
    }
    if(model->decoder_layer_1->output_init) {
        return model->decoder_layer_1->output->data[r][c];
    }
    printf("No output found!");
    exit(1);
}layer_func_t cfuncs[] = {
    {.name="encoder_layer_0", .print_func=print_encoder_layer_0, .dump_weights_func=dump_weights_encoder_layer_0, .dump_outputs_func=dump_outputs_encoder_layer_0 },
       {.name="encoder_layer_1", .print_func=print_encoder_layer_1, .dump_weights_func=dump_weights_encoder_layer_1, .dump_outputs_func=dump_outputs_encoder_layer_1 },
       {.name="encoder_layer_2", .print_func=print_encoder_layer_2, .dump_weights_func=dump_weights_encoder_layer_2, .dump_outputs_func=dump_outputs_encoder_layer_2 },
       {.name="decoder_layer_0", .print_func=print_decoder_layer_0, .dump_weights_func=dump_weights_decoder_layer_0, .dump_outputs_func=dump_outputs_decoder_layer_0 },
       {.name="decoder_layer_1", .print_func=print_decoder_layer_1, .dump_weights_func=dump_weights_decoder_layer_1, .dump_outputs_func=dump_outputs_decoder_layer_1 },
    {.name=NULL}
};
void print_layer(PTNLM *model, int layer)
{

    printf("%s", cfuncs[layer].name);
    cfuncs[layer].print_func(model);
}
double dump_layer_weights(PTNLM *model, int layer, char matrix, int r, int c)
{

    return cfuncs[layer].dump_weights_func(model, matrix, r, c);
}
double dump_layer_outputs(PTNLM *model, int layer, int r, int c)
{

    return cfuncs[layer].dump_outputs_func(model, r, c);
}
