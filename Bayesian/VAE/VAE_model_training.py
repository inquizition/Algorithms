import numpy as np
import os
import sys
import torch
import struct
import torch
import matplotlib.pyplot as plt
import numpy as np
import torch.optim as optim
from torch import nn
from torch.nn import functional as F
from torchsummary import summary
from torchvision import datasets
from torchvision.transforms import ToTensor
    

parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(parent_dir)
print(parent_dir)

# Now you can import modules from the parent directory
from VAE import Encoder, Decoder, VAE

def load_data():
   
    # load MNIST training and test data sets
    train_dataset = datasets.MNIST(
      root = 'data',
      train = True,                         
      transform = ToTensor(), 
      download = True,            
      )
    test_dataset = datasets.MNIST(
      root = 'data', 
      train = False, 
      transform = ToTensor()
      )

    return train_dataset, test_dataset

def sample_decode(model, K):
    # sample z
    z = torch.randn(K,2)
    
    return model.decode(z) 

def kl_divergence_term(Z_mu, Z_logsigma2):
    # compute the average KL divergence to the standard normal
    # distribution, neglecting additive constant terms
    sigma2 = torch.exp(Z_logsigma2)

    term = torch.mean(torch.sum(sigma2 ,dim=1) + torch.sum(torch.pow(Z_mu ,2), dim=1) - torch.sum(Z_logsigma2, dim=1))
    return term

def cost_function(X, model):
    # compute mean and log variance of the normal distribution of
    # the encoding z of input X
    Z_mu, Z_logsigma2 = model.encode(X)

    # compute the average KL divergence to the prior standard normal
    # distribution of z, neglecting constant terms
    # expected log p(x | z) + C
    kl = kl_divergence_term(Z_mu, Z_logsigma2)

    # sample z
    Z = Z_mu + torch.randn(Z_mu.size()) * torch.exp(0.5 * Z_logsigma2)

    # compute the representative decoding of z
    X_decoding = model.decode(Z)

    # compute negative average evidence of the input x, neglecting additive constant terms
    neg_log_evidence = 784 * model.logsigma2 + \
        torch.sum((X_decoding - X).pow(2) * torch.exp(- model.logsigma2), dim=1).mean()

    return neg_log_evidence + kl

def train_model(model, train_dataset, test_dataset):
    # define the data loaders
    train_data = torch.utils.data.DataLoader(train_dataset, batch_size=400, shuffle=True)
    test_data = torch.utils.data.DataLoader(test_dataset, batch_size=400)
    
    # define the optimizer
    optimizer = optim.Adam(model.parameters(), lr=0.01)
    
    # track the training and test loss
    training_loss = []
    test_loss = []
    
    training_epochs = 20 
    # optimize parameters for 20 epochs
    for i in range(training_epochs):
    
        # for each minibatch
        for x,_ in train_data:
            x = x.reshape(400,28*28)
            # evaluate the cost function on the training data set
            loss = cost_function(x, model)
    
            # update the statistics
            training_loss.append(loss.item())
            test_loss.append(float('nan'))
    
            # perform backpropagation
            loss.backward()
    
            # perform a gradient descent step
            optimizer.step()
            
            # reset the gradient information
            optimizer.zero_grad()
    
        # evaluate the model after every epoch
        with torch.no_grad():
    
            # evaluate the cost function on the test data set
            accumulated_loss = 0
            for x, _ in test_data:
                x = x.reshape(400,28*28)
                loss = cost_function(x, model)
                accumulated_loss += loss.item()
                
            # update statistics
            test_loss[-1] = accumulated_loss / len(test_data)
                
        print(f"Epoch {i + 1:2d}: training loss {training_loss[-1]: 9.3f}, "
              f"test loss {test_loss[-1]: 9.3f}")

    return model
            
def save_weights_to_binary(model, filename):
    with open(filename, 'wb') as bin_f:
        for name, param in model.named_parameters():
            if(not name == "logsigma2"):
                print(name)
                weight = param.detach().numpy()
                weight = weight.astype(np.float64);
                weight.tofile(bin_f)

def save_test_files(model, test_filenames):
    for filename in test_filenames:
        with open(filename, 'w') as txt_f:
            for name, param in model.named_parameters():
                if(not name == "logsigma2"):
                    print(name)
                    #txt_f.write(f"{param.row}\n")
                    weight = param.detach().numpy()
                    weight = weight.astype(np.float64);
                    txt_f.write(f"{np.shape(weight)[0]}\n")
                    if(len(np.shape(weight))==2):
                        txt_f.write(f"{np.shape(weight)[1]}\n")
                    else:
                        txt_f.write(f"1\n")
                    for val in weight.flatten():
                        txt_f.write(f"{val}\n")

def save_values_to_binary(tensor, filename):
    with open(filename, 'w') as txt_f:
        for val in tensor:
            res = val.detach().numpy()
            res = res.astype(np.float64);
            for v in res.flatten():
                txt_f.write(f"{v}\n")

def read_image(file_path):
    SIZE = 28 * 28  # 28x28 image
    img = []
    
    with open(file_path, 'rb') as file:
        for _ in range(SIZE):
            # Read 4 bytes at a time (size of an int in C)
            data = file.read(4)
            if not data:
                break
            # Unpack the binary data to an integer
            value = struct.unpack('i', data)[0]
            img.append(value)
    
    return img

def normalize_image(img):
    norm_img = [pixel / 255.0 for pixel in img]
    return norm_img

def save_weights_and_test_files(model):
    save_weights_to_binary(model, 'Bayesian/VAE/Models/vae_weights.bin')
    save_test_files(model, ['Bayesian/VAE/Tests/data/vae_weights.txt'])
    
    img = read_image('data/img.bin')
    norm_img = normalize_image(img)
    # Convert the normalized image list to a NumPy array
    norm_img_np = np.array(norm_img)  # Reshape to 28x28
    
    # Convert the NumPy array to a PyTorch tensor
    norm_img_tensor = torch.tensor(norm_img_np, dtype=torch.float32)
    res = model.encode(norm_img_tensor)
    save_values_to_binary(res[0:2], 'Bayesian/VAE/Tests/data/vae_encode.txt')
    
    save_values_to_binary(model.decode(res[0]), 'Bayesian/VAE/Tests/data/vae_decode.txt')


input_dim =     28*28
hidden_dim =    70
hidden_dim_2 =  50
latent_dim =    2

## For saving model ##
encoder = Encoder(input_dim, hidden_dim, latent_dim)
decoder = Decoder(latent_dim, hidden_dim_2, input_dim)
model = VAE(encoder, decoder)
train_dataset, test_dataset = load_data()
model = train_model(model, train_dataset, test_dataset)

torch.save({
    'model_state_dict': model.state_dict(),
    'encoder_params': (input_dim, hidden_dim, latent_dim),
    'decoder_params': (latent_dim, hidden_dim_2, input_dim)
}, 'vae_model.pth')

save_weights_and_test_files(model)
