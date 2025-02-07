import numpy as np
import os
import sys
import torch
import struct
from torch import nn
from torch.nn import functional as F
from torchsummary import summary
# Now you can import modules from the parent directory
from VAE import Encoder, Decoder, VAE

parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(parent_dir)
print(parent_dir)

number_of_encoder_layers = 0
number_of_decoder_layers = 0

# Load the model with metadata
checkpoint = torch.load('Bayesian/VAE/Models/vae_model.pth')
encoder_params = checkpoint['encoder_params']
decoder_params = checkpoint['decoder_params']

encoder_2 = Encoder(*encoder_params)
decoder_2 = Decoder(*decoder_params)
model_2 = VAE(encoder_2, decoder_2)
model_2.load_state_dict(checkpoint['model_state_dict'])

enc_in_dim = encoder_params[0]
enc_hid_dim = encoder_params[1]
enc_lat_dim = encoder_params[2]
dec_hid_dim = decoder_params[1]

def generate_layers_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            layers.append(f'model->{name}_layer_{i} = InitLinear({layer.in_features}, {layer.out_features});')
            #layers.append(f'void {name}_linear_{i}(float* input, float* output, float* weight, float* bias, int in_features, int out_features);')
            i += 1
    return layers

def generate_layers_free_c_code(module, name):
    layers = []
    i = 0
    for layer in module.children():
        if isinstance(layer, nn.Linear):
            layers.append(f'freeLinear(model->{name}_layer_{i});')
            #layers.append(f'void {name}_linear_{i}(float* input, float* output, float* weight, float* bias, int in_features, int out_features);')
            i += 1

    return layers

def generate_decode_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            if (i == 0):
                layers.append(f'Linear(model.{name}_layer_{i}, *z);')
                layers.append(f'Matrix *output_relu = allocateMatrix( model.{name}_layer_{i}->output->rows, model.{name}_layer_{i}->output->columns);')
                layers.append(f'copyMatrix(*model.{name}_layer_{i}->output, output_relu);')
                layers.append(f'reLu_matrix(output_relu);')
            else:
                layers.append(f'Linear(model.{name}_layer_{i}, *output_relu);')
            i += 1
    layers.append(f'sigmoid_matrix(model.{name}_layer_{i-1}->output);')
    layers.append(f'copyMatrix(*model.{name}_layer_{i-1}->output, output_mu);')
    layers.append(f'freeMatrix(output_relu);')
    return layers

def generate_encode_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            if (i == 0): 
                layers.append(f'Linear(model.{name}_layer_{i}, *x);')
                layers.append(f'Matrix *output_relu = allocateMatrix( model.{name}_layer_{i}->output->rows, model.{name}_layer_{i}->output->columns);')
                layers.append(f'copyMatrix(*model.{name}_layer_{i}->output, output_relu);')
                layers.append(f'reLu_matrix(output_relu);')
            else:
                layers.append(f'Linear(model.{name}_layer_{i}, *output_relu);')
            i += 1

    layers.append(f'copyMatrix(*model.{name}_layer_{i-2}->output, output_mu);')
    layers.append(f'copyMatrix(*model.{name}_layer_{i-1}->output, output_logvar);')
    layers.append(f'freeMatrix(output_relu);')
    return layers

def generate_NLM_h_code(module, name):
    layers = []
    i = 0
    for layer in module.children():
        if isinstance(layer, nn.Linear):
            layers.append(f'LM* {name}_layer_{i};')
            i += 1
    return layers

def generate_weight_load_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            layers.append(f'offset += load_weights(filename, &model->{name}_layer_{i}->A[0], {layer.in_features} * {layer.out_features}, offset);')
            layers.append(f'offset += load_weights(filename, &model->{name}_layer_{i}->b[0], {layer.out_features}, offset);')
            i += 1
    return layers

def generate_func_iterator_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            layers.append(f'    {{.name="{name}_layer_{i}", .print_func=print_{name}_layer_{i}, .dump_weights_func=dump_weights_{name}_layer_{i}, .dump_outputs_func=dump_outputs_{name}_layer_{i} }},\n')
            i +=1

    return layers

def generate_print_model_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            layers.append(f'static void print_{name}_layer_{i}( PTNLM* model )')
            layers.append(f'{{')
            layers.append(f'    print_linear(model->{name}_layer_{i});')
            layers.append(f'    print_matrix(*model->{name}_layer_{i}->A, "\\nA:");')
            layers.append(f'    print_matrix(*model->{name}_layer_{i}->b, "b:");')
            layers.append(f'}}')
            i +=1
    return layers

def generate_dump_model_weights_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            layers.append(f'\nstatic double dump_weights_{name}_layer_{i}( PTNLM* model, char matrix, int r, int c )')
            layers.append(f'{{')
            layers.append(f'    if(matrix == \'A\') {{')
            layers.append(f'        if((r > model->{name}_layer_{i}->A->rows) || (c > model->{name}_layer_{i}->A->columns)) {{')
            layers.append(f'            printf("Matrix A {name}_layer_{i} rows/columns out of bounds!");')
            layers.append(f'            exit(1);')
            layers.append(f'        }}')
            layers.append(f'        return model->{name}_layer_{i}->A->data[r][c];')
            layers.append(f'    }}')
            layers.append(f'    else if(matrix == \'b\') {{')
            layers.append(f'        if((r > model->{name}_layer_{i}->b->rows) || (c > model->{name}_layer_{i}->b->columns)) {{')
            layers.append(f'            printf("Matrix b {name}_layer_{i} rows/columns out of bounds!");')
            layers.append(f'            exit(1);')
            layers.append(f'        }}')
            layers.append(f'        return model->{name}_layer_{i}->b->data[r][c];')
            layers.append(f'    }}')
            layers.append(f'    printf("Invalid Matix choice!");')
            layers.append(f'    exit(1);')
            layers.append(f'}}')
            i +=1
    return layers

def generate_dump_model_outputs_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            layers.append(f'\nstatic double dump_outputs_{name}_layer_{i}( PTNLM* model, int r, int c )')
            layers.append(f'{{')
            layers.append(f'    if((r > model->{name}_layer_{i}->output->rows) || (c > model->{name}_layer_{i}->output->columns)) {{')
            layers.append(f'        printf("{name}_layer_{i} output rows/columns out of bounds!");')
            layers.append(f'        exit(1);')
            layers.append(f'    }}')
            layers.append(f'    if(model->{name}_layer_{i}->output_init) {{')
            layers.append(f'        return model->{name}_layer_{i}->output->data[r][c];')
            layers.append(f'    }}')
            layers.append(f'    printf("No output found!");')
            layers.append(f'    exit(1);')
            layers.append(f'}}')
            i +=1
    return layers


encoder_layers = generate_layers_c_code(encoder_2, "encoder", encoder_params)
encode_code = generate_encode_c_code(encoder_2, "encoder", encoder_params)

free_encoder_layers = generate_layers_free_c_code(encoder_2, "encoder");
free_decoder_layers = generate_layers_free_c_code(decoder_2, "decoder");

decoder_layers = generate_layers_c_code(decoder_2, "decoder", decoder_params)
decode_code = generate_decode_c_code(decoder_2, "decoder", decoder_params)

number_of_encoder_layers = len(encoder_layers)
number_of_decoder_layers = len(decoder_layers)

model_enc_struct_h_code = generate_NLM_h_code(encoder_2, "encoder")
model_dec_struct_h_code = generate_NLM_h_code(decoder_2, "decoder")

load_enc_weights_code = generate_weight_load_c_code(encoder_2, "encoder", encoder_params)
load_dec_weights_code = generate_weight_load_c_code(decoder_2, "decoder", decoder_params)

model_enc_print_code = generate_print_model_c_code(encoder_2, "encoder", encoder_params)
model_dec_print_code = generate_print_model_c_code(decoder_2, "decoder", decoder_params)

function_iterator_enc = generate_func_iterator_c_code(encoder_2, "encoder", encoder_params)
function_iterator_dec = generate_func_iterator_c_code(decoder_2, "decoder", decoder_params)

model_enc_dump_code = generate_dump_model_weights_c_code(encoder_2, "encoder", encoder_params)
model_dec_dump_code = generate_dump_model_weights_c_code(decoder_2, "decoder", decoder_params)

function_dump_output_enc_code = generate_dump_model_outputs_c_code(encoder_2, "encoder", encoder_params)
function_dump_output_dec_code = generate_dump_model_outputs_c_code(decoder_2, "decoder", decoder_params)

with open('Bayesian/VAE/Models/model.c', 'w') as f:
    f.write('#include \"model.h\"\n')
    f.write('static size_t load_weights(char *filename, Matrix *m, size_t size, size_t offset)\n{\n')
    f.write('    double *weights = (double *)malloc(size * sizeof(double));\n')
    f.write('    FILE *f = fopen(filename, "rb");\n')
    f.write('    if (!f) {\n')
    f.write('        fprintf(stderr, "Error opening weights file\\n");\n')
    f.write('        exit(1);\n')
    f.write('    }\n')
    f.write('    fseek(f, offset * sizeof(double), SEEK_SET);\n')
    f.write('    size_t read_count = fread(weights, sizeof(double), size, f);\n')
    f.write('    fclose(f);\n\n')
    f.write('    int rows = m->rows;\n')
    f.write('    int columns = m->columns;\n\n')
    f.write('    for (int i = 0; i < rows; i++) {\n')
    f.write('        for (int j = 0; j < columns; j++) {\n')
    f.write('            m->data[i][j] = weights[i * columns + j];\n')
    f.write('        }\n')
    f.write('    }\n\n')
    f.write('    free(weights);\n')
    f.write('    return read_count;\n')
    f.write('}\n')

    f.write(f'PTNLM *InitTrainedNLM_Model(void)\n')
    f.write('{\n')
    f.write('    PTNLM *model;\n')
    f.write('    model = (PTNLM*) malloc(sizeof(PTNLM));\n')
    f.write('    ')
    f.write('\n    '.join(encoder_layers + decoder_layers))
    f.write('\n    return model;\n');
    f.write('}')
    f.write('\n')

    f.write('void decode(PTNLM model, Matrix *z, Matrix *output_mu)\n')
    f.write('{\n')
    f.write('    ')
    f.write('\n    '.join(decode_code))
    f.write('\n}')
    f.write('\n')
    f.write('void encode(PTNLM model, Matrix *x, Matrix *output_mu, Matrix *output_logvar)\n')
    f.write('{\n')
    f.write('    ')
    f.write('\n    '.join(encode_code))
    f.write('\n}\n')

    f.write('void load_trained_weights(PTNLM *model, char *filename)\n{\n')
    f.write('    ')
    f.write('\n    size_t offset = 0;')
    f.write('\n    '.join(load_enc_weights_code + load_dec_weights_code))
    f.write('\n}\n')

    f.write('void freeTrainedNonLinear(PTNLM *model)\n{\n')
    f.write('    ')
    f.write('\n    '.join(free_encoder_layers + free_decoder_layers))
    f.write('\nfree(model);')
    f.write('\n}\n')

    f.write('\n'.join(model_enc_print_code + model_dec_print_code))
    f.write('\n'.join(model_enc_dump_code + model_dec_dump_code))
    f.write('\n'.join(function_dump_output_enc_code + function_dump_output_dec_code))

    f.write('layer_func_t cfuncs[] = {\n');
    f.write('   '.join(function_iterator_enc + function_iterator_dec));
    f.write(f'    {{.name=NULL}}\n')
    f.write('};\n');

    f.write('void print_layer(PTNLM *model, int layer)\n{\n')
    f.write('\n    printf("%s", cfuncs[layer].name);')
    f.write('\n    cfuncs[layer].print_func(model);')
    f.write('\n}\n')

    f.write('double dump_layer_weights(PTNLM *model, int layer, char matrix, int r, int c)\n{\n')
    f.write('\n    return cfuncs[layer].dump_weights_func(model, matrix, r, c);')
    f.write('\n}\n')
    
    f.write('double dump_layer_outputs(PTNLM *model, int layer, int r, int c)\n{\n')
    f.write('\n    return cfuncs[layer].dump_outputs_func(model, r, c);')
    f.write('\n}\n')

with open('Bayesian/VAE/Models/model.h', 'w') as f:
    f.write('#ifndef MODEL_H\n#define MODEL_H\n\n')
    f.write('#include "Matrix.h"\n#include "LinearModel.h"\n#include <stdbool.h>\n#include <stdlib.h>\n\n')
    f.write(f'#define INPUT_DIM {enc_in_dim}\n')
    f.write(f'#define ENCODER_HIDDEN_DIM {enc_hid_dim}\n')
    f.write(f'#define DECODER_HIDDEN_DIM {dec_hid_dim}\n')
    f.write(f'#define LATENT_DIM {enc_lat_dim}\n')
    f.write(f'#define ENCODER_LAYERS {number_of_encoder_layers}\n')
    f.write(f'#define DECODER_LAYERS {number_of_decoder_layers}\n\n')

    f.write('\n')
    f.write('typedef struct\n{\n')
    f.write('    bool output_init;\n')
    f.write('    ')
    f.write('\n    '.join(model_enc_struct_h_code + model_dec_struct_h_code))
    f.write('\n    Matrix* logsigma2;\n')
    f.write('}PTNLM;\n')
    f.write('\n')

    f.write('\n')
    f.write('typedef struct\n{\n')
    f.write('    char *name;\n')
    f.write('    void (*print_func)(PTNLM *);\n')
    f.write('    double (*dump_weights_func)(PTNLM *, char matrix, int r, int c);\n')
    f.write('    double (*dump_outputs_func)(PTNLM *, int r, int c);\n')
    f.write('}layer_func_t;\n')
    f.write('\n')

    f.write('void decode(PTNLM model, Matrix *z, Matrix *output);\n')
    f.write('\n')
    f.write('void encode(PTNLM model, Matrix *x, Matrix *output_mu, Matrix *output_logvar);\n')
    f.write('\n')

    f.write('void print_layer(PTNLM *model, int layer);\n')
    f.write('double dump_layer_weights(PTNLM *model, int layer, char matrix, int r, int c);\n')
    f.write('double dump_layer_outputs(PTNLM *model, int layer, int r, int c);\n')

    f.write(f'PTNLM *InitTrainedNLM_Model(void);\n')
    f.write('\n')
    f.write('void load_trained_weights(PTNLM *model, char *filename);\n')
    f.write('\n')
    f.write('void freeTrainedNonLinear(PTNLM *model);\n')
    f.write('\n')

    f.write('\n\n#endif // MODEL_H\n')



print("Header file generated: model.h")
