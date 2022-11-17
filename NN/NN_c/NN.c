#include "NN.h"
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include <time.h>

void d_relu(struct Matrix z, struct Matrix *dz);
void relu(struct Matrix z, struct Matrix *a);
struct Linear *InitLayer(int input_features, int output_features);
void linear( struct Linear *linear_model, struct Matrix x);
void output_error(struct Matrix z, struct Matrix y, struct Matrix error);

void testLinear();

void relu(struct Matrix z, struct Matrix *a)
{
    int r;
    int c;
    for(r = 0; r < z.rows; r++)
    {
        for(c = 0; c < z.columns; c++)
        {
            if(z.data[r][c] <= 0) {
                a->data[r][c] = 0.0;
            }
            else
            {
                a->data[r][c] = z.data[r][c];
            }
        }
    }
}

void d_relu(struct Matrix z, struct Matrix *dz)
{
    int r;
    int c;
    for(r = 0; r < z.rows; r++)
    {
        for(c = 0; c < z.columns; c++)
        {
            if(z.data[r][c] > 0) {
                dz->data[r][c] = 1.0;
            } else {
                dz->data[r][c] = 0.0;
            }
        }
    }
}

// void backwardpass(struct Linear layer, struct Matrix y)
// {
//     a = softmax(layer.output)
//     dz = d_relu(layer.output)
// }

void output_error(struct Matrix z, struct Matrix y, struct Matrix error)
{

    struct Matrix *a = allocateMatrix(z.rows,z.columns);
    struct Matrix *diff = allocateMatrix(z.rows,z.columns);
    struct Matrix *dz = allocateMatrix(z.rows,z.columns);

    relu(z, a);
    d_relu(z, dz);
    matrixSubtract(*a, y, diff);

    hadamard_prod(*diff, *dz, &error);

    print_matrix(*a);
    print_matrix(*dz);
    print_matrix(*diff);

    freeMatrix(a);
    freeMatrix(diff);
    freeMatrix(dz);
}

double MSE_loss(struct Matrix pred, struct Matrix y)
{
    double mse = 0.0;
    int r;
    int c;
    for(r = 0; r < y.rows; r++)
    {
        for(c = 0; c < y.columns; c++)
        {
            mse += pow( (y.data[r][c] - y.data[r][c]), 2);
        }
    }

    return mse/(y.columns*y.rows);
}

void linear( struct Linear *linear_model, struct Matrix x)
{
    transpose(&linear_model->weight);
    matMult(x, *linear_model->weight, linear_model->output);
    matrixAdd(linear_model->output, *linear_model->bias);
}

struct Linear *InitLayer(int input_features, int output_features)
{
    struct Linear *linear_model = (struct Linear*)malloc(sizeof(struct Linear));
    linear_model->weight = allocateMatrix(output_features, input_features);
    linear_model->bias = allocateMatrix(1, output_features);
    linear_model->output = allocateMatrix(1, output_features);

    linear_model->output->rows = 1;
    linear_model->output->columns = linear_model->output_features;

    InitRandomMatrix(linear_model->weight);
    InitRandomMatrix(linear_model->bias);

    return linear_model;
}

void freeLayer(struct Linear *layer)
{
    freeMatrix(layer->weight);
    freeMatrix(layer->bias);
    freeMatrix(layer->output);
    free(layer);
}

void print_layer(struct Linear linear_model)
{
    printf("Weights: \n");
    print_matrix(*linear_model.weight);
    printf("Bias: \n");
    print_matrix(*linear_model.bias);
}

void testLinear()
{
    struct Matrix *x = allocateMatrix(1,3);
    double data[1][3] = {{1,2,3}};
    fillMatrix(x, *data);
    print_matrix(*x);

    struct Linear *lin_layer = InitLayer(3,2);
    ones(lin_layer->weight);
    ones(lin_layer->bias);

    linear(lin_layer, *x);
    print_matrix(*lin_layer->output);

    struct Matrix *answer = allocateMatrix(1,2);
    double data_2[1][2] = {{7,7}};
    fillMatrix(answer, *data_2);

    if( !cmpMatrix(*lin_layer->output, *answer))
    {
        printf("linearTestFailed!");
    }

    freeLayer(lin_layer);
    freeMatrix(x);
    freeMatrix(answer);
}

void output_error_test()
{
    struct Matrix *x = allocateMatrix(1,3);
    double data[1][3] = {{1,2,3}};
    fillMatrix(x, *data);
    print_matrix(*x);

    struct Linear *lin_layer = InitLayer(3,2);
    ones(lin_layer->weight);
    ones(lin_layer->bias);

    linear(lin_layer, *x);

    struct Matrix *answer = allocateMatrix(1,2);
    double data_2[1][2] = {{7,7}};
    fillMatrix(answer, *data_2);

    print_matrix(*lin_layer->output);

    struct Matrix *erro = allocateMatrix(answer->rows,answer->columns);
    output_error(*lin_layer->output, *answer, *erro);
    print_matrix(*erro);

    freeLayer(lin_layer);
    freeMatrix(x);
    freeMatrix(answer);
    freeMatrix(erro);
}

int main( void )
{
    printf("Start \n");
    srand(time(NULL));

    printf("Linear test\n");
    testLinear();

    printf("Output Error test\n");
    output_error_test();

    // struct Matrix *x = allocateMatrix(1,3);
    // double data[1][3] = {{1,2,3}};
    // fillMatrix(x, *data);
    // print_matrix(*x);

    // struct Linear *lin_layer_1 = InitLayer(3,3);
    // struct Linear *lin_layer_2 = InitLayer(3,2);

    // print_layer(*lin_layer_1);
    // print_layer(*lin_layer_2);

    // linear(lin_layer_1, *x);
    // print_matrix(*lin_layer_1->output);
    // linear(lin_layer_2, *lin_layer_1->output);
    // print_matrix(*lin_layer_2->output);

    // freeLayer(lin_layer_1);
    // freeLayer(lin_layer_2);
    // freeMatrix(x);

    return 1;
}