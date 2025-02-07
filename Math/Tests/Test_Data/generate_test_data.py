import struct
import numpy as np
from os import listdir
from os.path import isfile, join
import glob

def create_random_matrix_data(rows, columns, name_prefix):
    mu, sigma = 1, 0.1 # mean and standard deviation
    A = np.random.normal(mu, sigma, rows*columns)
    A = np.array([i for i in A])
    # A = np.array([float(round(i, 0)) for i in A])
    A.sort()
    A = A.reshape([rows, columns])

    write_data_to_file(A, name_prefix)
    return A

def write_data_to_file(data, name_prefix):
    rows = np.shape(data)[0]
    columns = np.shape(data)[1]
    file_name = f'Math/Tests/Test_Data/{name_prefix}_{rows}_{columns}.bin'

    flattened_data = data.flatten()
    with open(file_name, 'wb') as f:
        for e in flattened_data:
            f.write(struct.pack('f', e)) 

def create_test_matrices():
    A = create_random_matrix_data(2, 2, 'A')
    B = create_random_matrix_data(2, 2, 'B')

    A_DOT_B = np.dot(A, B)

    print(A)
    print(B)
    print(A_DOT_B)

    write_data_to_file(A_DOT_B, "A_DOT_B")

def print_test_matrices():
    test_list = glob.glob('Math/Tests/Test_Data/*.bin')
    print(test_list)
    
    for test in test_list:
        with open(test, 'rb') as f:
                data = f.read() 

        #print(struct.unpack("f" * ((len(data) -24) // 4), data[20:-4]))
        print(struct.unpack("f"*n, data))
    

def main():
    #create_test_matrices() 
    print_test_matrices()



main()
