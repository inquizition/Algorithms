from Bayesian.VAE.Models.model_api import Initiate_c_VAE_model, close_c_VAE_model, c_decoder, c_encoder
from torchvision import datasets
from torchvision.transforms import ToTensor
import matplotlib
import matplotlib.pyplot as plt

test_dataset = datasets.MNIST(
  root = 'data', 
  train = False, 
  transform = ToTensor()
  )

test_images = test_dataset.data/255
test_labels = test_dataset.targets

Initiate_c_VAE_model()

img = test_images[0,:,:]/255
#numimg = img.reshape(28,28)/max(img)

plt.imshow(img, cmap='gray')
plt.show()


close_c_VAE_model()
