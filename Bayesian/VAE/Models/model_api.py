from cffi import FFI
import os
import numpy as np
import sys

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

def Initiate_c_VAE_model():
    weights_file_path = ffi.new("char[]", b"Bayesian/VAE/Models/vae_weights.bin")
    init_preTrained_model(weights_file_path)

def close_c_VAE_model():
    close_preTrained_model()

def c_decoder(encoded_data):
    decoded_data = np.zeros(28,28) 

    decoded_data = decoded_data.astype(np.double)
    decoded_data = ffi.from_buffer(decoded_data)
    decoded_data = ffi.cast("double*", decoded_data)

    encoded_data = encoded_data.astype(np.double)
    encoded_data = ffi.from_buffer(encoded_data)
    encoded_data = ffi.cast("double*", encoded_data)

    decode_data(encoded_data, decoded_data)

    return decoded_data

def c_encoder(data):
    z = np.zeros(1,2)
    logvar = np.zeros(1,2)

    z = z.astype(np.double)
    z = ffi.from_buffer(z)
    z = ffi.cast("double*", z)

    logvar = logvar.astype(np.double)
    logvar = ffi.from_buffer(logvar)
    logvar = ffi.cast("double*", logvar)

    data = data.astype(np.double)
    data = ffi.from_buffer(data)
    data = ffi.cast("double*", data)

    encode_data(data, z, logvar)

    return z, logvar 
