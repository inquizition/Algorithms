from cffi import FFI
import os
import numpy as np
import sys
import torch

parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(parent_dir)
print(parent_dir)

# Now you can import modules from the parent directory
from VAE import Encoder, Decoder, VAE

ffi = FFI()

# Define the C function signatures from your library
ffi.cdef("""
void Init_preTrained_VAE_model(char * weights_file_path);
void close_preTrained_VAE_model(void);
void decode_data(double *encoded_data, double *decoded_data);
void encode_data(double *data, double *encoded_data, double *logvar_data);
void get_model_weights(int layer, char ab, double *weights, int rows, int columns);
""")

# Load the shared library
lib_name = "libModelFunctions.so" if os.name == "posix" else "ModelFunctions.dll"
C = ffi.dlopen(os.path.join(os.path.dirname(__file__), "../../../build/Bayesian/VAE/Models", lib_name))

# Access the C functions
init_preTrained_model = C.Init_preTrained_VAE_model
close_preTrained_model = C.close_preTrained_VAE_model
decode_data = C.decode_data
encode_data = C.encode_data

checkpoint = torch.load('Bayesian/VAE/Models/vae_model.pth')
encoder_params = checkpoint['encoder_params']
in_dim = encoder_params[0]
lat_dim = encoder_params[2]

def Initiate_c_VAE_model():
    weights_file_path = ffi.new("char[]", b"Bayesian/VAE/Models/vae_weights.bin")
    init_preTrained_model(weights_file_path)

def close_c_VAE_model():
    close_preTrained_model()

def c_decoder(encoded_data):
    # Ensure encoded_data is a numpy array
    encoded_data = np.asarray(encoded_data, dtype=np.double)
    encoded_data_size = encoded_data.size

    # Prepare decoded_data array
    decoded_data_size = in_dim  # Example size, adjust as needed
    decoded_data = np.zeros(decoded_data_size, dtype=np.double)
    original_shape = decoded_data.shape

    # Prepare the encoded data buffer
    encoded_data_ffi = ffi.from_buffer(encoded_data)
    encoded_data_cast = ffi.cast("double*", encoded_data_ffi)

    # Prepare the decoded data buffer
    decoded_data_ffi = ffi.from_buffer(decoded_data)
    decoded_data_cast = ffi.cast("double*", decoded_data_ffi)

    # Call the C function
    C.decode_data(encoded_data_cast, decoded_data_cast)

    # Convert the decoded data back to a numpy array
    decoded_data_numpy = np.copy(decoded_data).reshape(original_shape)

    return decoded_data_numpy

def c_encoder(in_data):
    z = np.zeros([1, lat_dim], dtype=np.double)
    logvar = np.zeros([1, lat_dim], dtype=np.double)

    # Prepare the encoded data buffer for z
    z_ffi = ffi.from_buffer(z)
    z_cast = ffi.cast("double*", z_ffi)

    # Prepare the encoded data buffer for logvar
    logvar_ffi = ffi.from_buffer(logvar)
    logvar_cast = ffi.cast("double*", logvar_ffi)

    # Prepare the input data buffer
    in_data = np.asarray(in_data, dtype=np.double)
    data_ffi = ffi.from_buffer(in_data)
    data_cast = ffi.cast("double*", data_ffi)

    # Call the C function
    C.encode_data(data_cast, z_cast, logvar_cast)

    # Convert the z data back to a numpy array
    z_numpy = np.copy(z)

    # Convert the logvar data back to a numpy array
    logvar_numpy = np.copy(logvar)

    return z_numpy, logvar_numpy
