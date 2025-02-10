import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
math_lib = ctypes.CDLL("libmath.so")

class Matrix(ctypes.Structure):
    _fields = [("rows", ctypes.c_int),
                       ("columns", ctypes.c_int),
                       ("data", ctypes.POINTER(ctypes.c_double))]


# Convert a NumPy array to C Matrix
def numpy_to_matrix(np_array):
    rows, cols = np_array.shape
    data_ptr = np_array.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    return Matrix(rows, cols, data_ptr)

# Matrix multiplication using the C library
def matrix_multiply(A, B):
    assert A.shape[1] == B.shape[0], "Matrix dimensions must match for multiplication"
    A_mat = numpy_to_matrix(A)
    B_mat = numpy_to_matrix(B)

    res_ptr = math_lib.allocateMatrix(A.shape[0], B.shape[1])
    math_lib.dot(A_mat, B_mat, res_ptr)

    res_array = np.ctypeslib.as_array(res_ptr.contents.data, shape=(A.shape[0], B.shape[1]))
    # Free the allocated matrix
    math_lib.freeMatrix(res_ptr)
    return res_array

# Test
a= np.random.rand(100, 100).astype(np.float64)
b= np.random.rand(100, 100).astype(ntp.float64)

get_res = matrix_multiply(a, b)

plt.imshow(get_res, cmap="hot", interpolation="nearest")
plt.colorbar()
plt.title("Matrix Multiplication Heatmap")
plt.show()