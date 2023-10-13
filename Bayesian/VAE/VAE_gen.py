import numpy as np
import torch
import struct
from torchvision import datasets
from torchvision.transforms import ToTensor

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

data = np.asarray(train_dataset.data[0].numpy())

with open('data/img.bin', 'wb') as f:
    for c in (data):
        for r in c:
            f.write(struct.pack('i', r)) 