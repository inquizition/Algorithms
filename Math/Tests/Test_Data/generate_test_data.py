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
            f.write(struct.pack('d', e)) 

def create_test_matrices():
    A_2_2 = create_random_matrix_data(2, 2, 'A')
    B_2_2 = create_random_matrix_data(2, 2, 'B')
    A_2_4 = create_random_matrix_data(2, 4, 'A')

    A_500_500 = create_random_matrix_data(500, 500, 'A')
    B_500_500 = create_random_matrix_data(500, 500, 'B')

    A_5000_5000 = create_random_matrix_data(5000, 5000, 'A')
    B_5000_5000 = create_random_matrix_data(5000, 5000, 'B')

    A_DOT_B = np.dot(A_2_2, B_2_2)
    A_ADD_B = A_2_2 + B_2_2
    A_DOT_B_500_500 = np.dot(A_500_500, B_500_500)
    A_DOT_B_5000_5000 = np.dot(A_5000_5000, B_5000_5000)

    # print(A)
    # print(B)
    # print(A_DOT_B)
    # print(A_ADD_B)

    write_data_to_file(A_DOT_B, "A_DOT_B")
    write_data_to_file(A_ADD_B, "A_ADD_B")
    write_data_to_file(A_DOT_B_500_500, "A_DOT_B")
    write_data_to_file(A_DOT_B_5000_5000, "A_DOT_B")


def print_test_matrices():
    test_list = glob.glob('Math/Tests/Test_Data/*.bin')
    print(test_list)
    
    for test in test_list:
        with open(test, 'rb') as f:
                data = f.read() 
        print(struct.unpack("d" * (len(data) // 8), data))

def main():
    create_test_matrices() 
    #print_test_matrices()

# A_DOT_B_2_2.bin  
# (1.7335287448731826, 1.8903259182999945, 1.8766938307757912, 2.0459984404127223)
# B_2_2.bin
# (0.8082234285741664, 0.8984323832110909, 1.0329155542789747, 1.1093411793381136)
# A_2_2.bin
# (0.9383293091589001, 0.9440742851179628, 1.0012930578664205, 1.0334100576930476)




main()
