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
decoder_layers = generate_layers_c_code(decoder_2, "decoder", decoder_params)

with open('Bayesian/VAE/model.c', 'w') as f:
    f.write('#ifndef MODEL_H\n#define MODEL_H\n\n')
    f.write(f'#define INPUT_DIM {enc_in_dim}\n')
    f.write(f'#define ENCODER_HIDDEN_DIM {enc_hid_dim}\n')
    f.write(f'#define DECODER_HIDDEN_DIM {dec_hid_dim}\n')
    f.write(f'#define LATENT_DIM {enc_lat_dim}\n\n')
    f.write(f'NLM *InitNLM_Model(void)\n')
    f.write('{\n')
    f.write('    NLM *model;\n')
    f.write('    model = (NLM*) malloc(sizeof(NLM));')
    f.write('\n    '.join(encoder_layers + decoder_layers))
    f.write('\n    return model;\n');
    f.write('}')
    f.write('\n\n#endif // MODEL_H\n')

print("Header file generated: model.h")
