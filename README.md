## Algorithms

A showcase of algorithms can be found here: [a relative link](main.ipynb)

# FFT

An implementation of the Cooley–Tukey fast Fourier transform is provided in
[`Math/fft.c`](Math/fft.c) with a small Python wrapper in
[`Math/fft.py`](Math/fft.py).  Compile the C source to a shared library before
use:

```
gcc -fPIC -shared Math/fft.c -o Math/libfft.so -lm
```

The Python module then exposes `fft`, `ifft` and `display_fft` functions for
working with NumPy arrays.

```
import numpy as np
from Math.fft import display_fft

sample_rate = 48_000
t = np.linspace(0, 1, sample_rate, endpoint=False)
signal = np.sin(2 * np.pi * 440 * t)
display_fft(signal, sample_rate)
```

# Build
Build the project with "cmake --build ." in build folder
then do "cmake .." in build folder.

# Tests
Run tests with "make test" in the build folder.

# Running
Run the main executable with "build/main" from root folder.
