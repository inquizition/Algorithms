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
void run_trained_model(double *image);
void run_print_model(char *weights_file);
""")

# Load the shared library
lib_name = "libModelFunctions.so" if os.name == "posix" else "ModelFunctions.dll"
C = ffi.dlopen(os.path.join(os.path.dirname(__file__), "../../../build/Bayesian/VAE/Models", lib_name))

# Access the C functions
run_trained_model = C.run_trained_model
run_print_model = C.run_print_model

# Load the model with metadata
checkpoint = torch.load('vae_model.pth')
encoder_params = checkpoint['encoder_params']
decoder_params = checkpoint['decoder_params']
encoder = Encoder(*encoder_params)
decoder = Decoder(*decoder_params)
model = VAE(encoder, decoder)
model.load_state_dict(checkpoint['model_state_dict'])


if __name__ == "__main__":
 # Read the binary image file
    bin_file_path = "data/img.bin"
    with open(bin_file_path, "rb") as f:
        data = f.read(28 * 28 * 4)  # Read 784 int elements (each int is 4 bytes)

    # Convert the binary data to a numpy array of integers
    img = np.frombuffer(data, dtype=np.int32).reshape(28, 28)

    # Normalize the image data
    norm_img = img / 255.0

    norm_img = norm_img.astype(np.double)

    # Use ffi.from_buffer to get a C-compatible pointer to the NumPy array data
    input_c_array = ffi.from_buffer(norm_img)

    # Explicitly cast the pointer to the correct type
    input_c_array = ffi.cast("double*", input_c_array)

    weights_file_path = ffi.new("char[]", b"Bayesian/VAE/Models/vae_weights.bin")

    run_print_model(weights_file_path)

    # Call the function with the C array and its length
    run_trained_model(input_c_array)

    # Convert the NumPy array to a PyTorch tensor
    norm_img_tensor = torch.from_numpy(norm_img.reshape(-1,784)).float()

    print(encoder.forward(norm_img_tensor))
