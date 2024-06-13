import torch
from torch import nn
from torch.nn import functional as F
from torchsummary import summary

class Encoder(nn.Module):
    def __init__(self, input_dim, hidden_dim, latent_dim):
        super(Encoder, self).__init__()
        self.fc1 = nn.Linear(input_dim, hidden_dim)
        self.fc2_mu = nn.Linear(hidden_dim, latent_dim)
        self.fc2_logvar = nn.Linear(hidden_dim, latent_dim)

    def forward(self, x):
        h = F.relu(self.fc1(x))
        mu = self.fc2_mu(h)
        logvar = self.fc2_logvar(h)
        return mu, logvar

class Decoder(nn.Module):
    def __init__(self, latent_dim, hidden_dim, output_dim):
        super(Decoder, self).__init__()
        self.fc1 = nn.Linear(latent_dim, hidden_dim)
        self.fc2 = nn.Linear(hidden_dim, output_dim)

    def forward(self, z):
        h = F.relu(self.fc1(z))
        x_hat = torch.sigmoid(self.fc2(h))
        return x_hat

class VAE(nn.Module):
    def __init__(self, encoder, decoder):
        super(VAE, self).__init__()
        self.encoder = encoder
        self.decoder = decoder

    def reparameterize(self, mu, logvar):
        std = torch.exp(0.5 * logvar)
        eps = torch.randn_like(std)
        return mu + eps * std

    def forward(self, x):
        mu, logvar = self.encoder(x)
        z = self.reparameterize(mu, logvar)
        x_hat = self.decoder(z)
        return x_hat, mu, logvar

def generate_layers_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            print(layer.in_features)
            print(layer.out_features)
            layers.append(f'model->{name}_layer_{i} = InitLinear({layer.in_features}, {layer.out_features});')
            #layers.append(f'void {name}_linear_{i}(float* input, float* output, float* weight, float* bias, int in_features, int out_features);')
            i += 1
    return layers

def generate_layers_free_c_code(module, name):
    layers = []
    i = 0
    for layer in module.children():
        if isinstance(layer, nn.Linear):
            print(layer.in_features)
            print(layer.out_features)
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
                layers.append(f'Linear(model.{name}_layer_{i}, z);')
                layers.append(f'Matrix *output_relu = allocateMatrix( model.{name}_layer_{i}->output->rows, model.{name}_layer_{i}->output->columns);')
                layers.append(f'copyMatrix(*model.{name}_layer_{i}->output, output_relu);')
                layers.append(f'reLu_matrix(output_relu);')
            else:
                layers.append(f'Linear(model.{name}_layer_{i}, model.{name}_layer_{0}->output);')
            i += 1
    return layers

def generate_encode_c_code(module, name, params):
    layers = []
    i = 0
    for layer, dim in zip(module.children(), params):
        if isinstance(layer, nn.Linear):
            if (i == 0): 
                layers.append(f'Linear(model.{name}_layer_{i}, x);')
                layers.append(f'Matrix *output_relu = allocateMatrix( model.{name}_layer_{i}->output->rows, model.{name}_layer_{i}->output->columns);')
                layers.append(f'copyMatrix(*model.{name}_layer_{i}->output, output_relu);')
                layers.append(f'reLu_matrix(output_relu);')
            else:
                layers.append(f'Linear(model.{name}_layer_{i}, output_relu);')
            i += 1
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
            layers.append(f'load_weights("vae_weights", &model->{name}_layer_{i}->A[0], {layer.in_features} * {layer.out_features});')
            layers.append(f'load_weights("vae_weights", &model->{name}_layer_{i}->b[0], {layer.out_features});')
            i += 1
    return layers

def save_weights_to_binary(model, filename):
    with open(filename, 'wb') as f:
        for name, param in model.named_parameters():
            weight = param.detach().numpy()
            weight.tofile(f)

input_dim=784
hidden_dim = 512
hidden_dim_2 = 400
latent_dim=20


## For saving model ##
encoder = Encoder(input_dim, hidden_dim, latent_dim)
decoder = Decoder(latent_dim, hidden_dim_2, input_dim)
model = VAE(encoder, decoder)

torch.save({
    'model_state_dict': model.state_dict(),
    'encoder_params': (input_dim, hidden_dim, latent_dim),
    'decoder_params': (latent_dim, hidden_dim_2, input_dim)
}, 'vae_model.pth')
## Saving model end ##

# Load the model with metadata
checkpoint = torch.load('vae_model.pth')
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

encoder_layers = generate_layers_c_code(encoder_2, "encoder", encoder_params)
encode_code = generate_encode_c_code(encoder_2, "encoder", encoder_params)

free_encoder_layers = generate_layers_free_c_code(encoder_2, "encoder");
free_decoder_layers = generate_layers_free_c_code(decoder_2, "decoder");

decoder_layers = generate_layers_c_code(decoder_2, "decoder", decoder_params)
decode_code = generate_decode_c_code(decoder_2, "decoder", decoder_params)

model_enc_struct_h_code = generate_NLM_h_code(encoder_2, "encoder")
model_dec_struct_h_code = generate_NLM_h_code(decoder_2, "decoder")

load_enc_weights_code = generate_weight_load_c_code(encoder_2, "encoder", encoder_params)
load_dec_weights_code = generate_weight_load_c_code(decoder_2, "decoder", decoder_params)

save_weights_to_binary(model_2, 'Bayesian/VAE/Models/vae_weights.bin')

with open('Bayesian/VAE/Models/model.c', 'w') as f:
    f.write('#include \"model.h\"\n')
    f.write('static void load_weights(const char *filename, double *weights, size_t size)\n{\n')
    f.write('    FILE *f = fopen(filename, "rb");\n')
    f.write('    if(!f){\n')
    f.write('        fprintf(stderr, "Error opening weights file\\n");\n')
    f.write('        exit(1);\n')
    f.write('    }\n')
    f.write('    fread(weights, sizeof(double), size, f);\n')
    f.write('}')
    f.write('\n')

    f.write(f'PTNLM *InitTrainedNLM_Model(void)\n')
    f.write('{\n')
    f.write('    PTNLM *model;\n')
    f.write('    model = (PTNLM*) malloc(sizeof(PTNLM));\n')
    f.write('    ')
    f.write('\n    '.join(encoder_layers + decoder_layers))
    f.write('\n    return model;\n');
    f.write('}')
    f.write('\n')

    f.write('void decode(PTNLM model, Matrix *z)\n')
    f.write('{\n')
    f.write('    ')
    f.write('\n    '.join(decode_code))
    f.write('\n}')
    f.write('\n')
    f.write('void encode(PTNLM model, Matrix *x)\n')
    f.write('{\n')
    f.write('    ')
    f.write('\n    '.join(encode_code))
    f.write('\n}\n')

    f.write('void load_trained_weights(PTNLM *model)\n{\n')
    f.write('    ')
    f.write('\n    '.join(load_enc_weights_code + load_dec_weights_code))
    f.write('\n}\n')

    f.write('void freeTrainedNonLinear(PTNLM *model)\n{\n')
    f.write('    ')
    f.write('\n    '.join(free_encoder_layers + free_encoder_layers))
    f.write('\nfree(model);')
    f.write('\n}\n')


with open('Bayesian/VAE/Models/model.h', 'w') as f:
    f.write('#ifndef MODEL_H\n#define MODEL_H\n\n')
    f.write('#include "Matrix.h"\n#include "LinearModel.h"\n#include <stdbool.h>\n#include <stdlib.h>\n\n')
    f.write(f'#define INPUT_DIM {enc_in_dim}\n')
    f.write(f'#define ENCODER_HIDDEN_DIM {enc_hid_dim}\n')
    f.write(f'#define DECODER_HIDDEN_DIM {dec_hid_dim}\n')
    f.write(f'#define LATENT_DIM {enc_lat_dim}\n\n')

    f.write('\n')
    f.write('typedef struct\n{\n')
    f.write('    bool output_init;\n')
    f.write('    ')
    f.write('\n    '.join(model_enc_struct_h_code + model_dec_struct_h_code))
    f.write('\n    Matrix* logsigma2;\n')
    f.write('}PTNLM;\n')
    f.write('\n')

    f.write('void decode(PTNLM model, Matrix *z);\n')
    f.write('\n')
    f.write('void encode(PTNLM model, Matrix *x);\n')
    f.write('\n')

    f.write(f'PTNLM *InitTrainedNLM_Model(void);\n')
    f.write('\n')
    f.write('void load_trained_weights(PTNLM *model);')
    f.write('\n')
    f.write('void freeTrainedNonLinear(PTNLM *model);\n')
    f.write('\n')

    f.write('\n\n#endif // MODEL_H\n')



print("Header file generated: model.h")
