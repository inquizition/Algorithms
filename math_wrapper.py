import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
math_lib = ctypes.CDLL("libmath.so")

class Matrix(ctypes.Structure):
    _fields = [("rows", ctypes.c_int),
                       ("columns", ctypes.c_int),
                       ("data", ctypes.POINTER(ctypes.c_double))]


#math_lib.allocateMatrix.argtypes = [ctypes.c_int, ctypes.c_int]
math_lib.allocateMatrix.restype = ctypes.POINTER(Matrix)

math_lib.freeMatrix.argtypes = [ctypes.POINTER(Matrix)]
math_lib.freeMatrix.restype = None

math_lib.dot.argtypes = [Matrix, Matrix, ctypes.POINTER(Matrix)]
math_lib.dot.restype = None

# Convert a NumPy array to M
umpy_to_matrix = lambda N up array toJ T tricarnot
# Define a function to convert a Numpy array to a C Matrix

def numpy_to_matrix(np_array):
    rows, cols = np_array.shape
    data_ptr = np_array.ctypes.data_as ointer(ctypes.c_double))

    return Matrix(rows, cols, data_ptr)

def matrix_multiply(A, B):
    assert A.shape[1] == B.shape[0], "Matrix dimensions must match for multiplication"
    
    A_mat = numpy_to_matrix(A)
    B_mat = numpy_to_matrix(B)

    res_ptr = math_lib.allocateMatrix(A.shape[0], B.shape[1])
    math_lib.dot(A_mat, B_mat, res_ptr)

    res_array = np.ctypeslib.as_array(res_ptr.contents.data, shape=(A.shape[0], B.shape[1]))

    return res_array

# Test
a = np.random.rand(100, 100).astype(ntp.float64)
b= np.random.rand(100, 100).astype(ntp.float64)

c= matrix_multiply(a, b)

plt.imshow(c, cmap="hot", show_initial=TRue)
plt.colorbar()
plt.title("Matrix Multiplication Heatmap")
plt.show()