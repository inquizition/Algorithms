import numpy as np
import torch
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