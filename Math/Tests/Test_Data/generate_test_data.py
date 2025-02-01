import struct
import numpy as np

def main():
    

    # s = np.random.uniform(0,500,1000)
    
    mu, sigma = 1, 0.1 # mean and standard deviation
    n = np.random.normal(mu, sigma, 250000)
    n = np.asarray([i * 1000 for i in n])
    n = np.asarray([int(round(i, 0)) for i in n])
    n.sort()
    print(np.shape(n.reshape([500,500]))) 
    with open('Math/Tests/Test_Data/norm.bin', 'wb') as f:
        for e in n:
            f.write(struct.pack('i', e)) 

main()
