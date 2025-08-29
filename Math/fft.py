import os
import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_double, c_size_t, POINTER


_lib = None


def _get_lib():
    global _lib
    if _lib is None:
        lib_path = os.path.join(os.path.dirname(__file__), "libfft.so")
        _lib = CDLL(lib_path)
        _lib.fft.argtypes = [POINTER(c_double), POINTER(c_double), c_size_t]
        _lib.ifft.argtypes = [POINTER(c_double), POINTER(c_double), c_size_t]
    return _lib


def fft(signal):
    """Compute the FFT of *signal* using the C implementation."""
    lib = _get_lib()
    signal = np.asarray(signal, dtype=np.complex128)
    n = signal.shape[0]
    if n & (n - 1) != 0:
        raise ValueError("size of signal must be a power of 2")
    real = np.ascontiguousarray(signal.real, dtype=np.double)
    imag = np.ascontiguousarray(signal.imag, dtype=np.double)
    lib.fft(real.ctypes.data_as(POINTER(c_double)),
            imag.ctypes.data_as(POINTER(c_double)),
            n)
    return real + 1j * imag


def ifft(spectrum):
    """Compute the inverse FFT of *spectrum* using the C implementation."""
    lib = _get_lib()
    spectrum = np.asarray(spectrum, dtype=np.complex128)
    n = spectrum.shape[0]
    real = np.ascontiguousarray(spectrum.real, dtype=np.double)
    imag = np.ascontiguousarray(spectrum.imag, dtype=np.double)
    lib.ifft(real.ctypes.data_as(POINTER(c_double)),
             imag.ctypes.data_as(POINTER(c_double)),
             n)
    return real + 1j * imag


def display_fft(signal, sample_rate):
    """Display the magnitude spectrum of *signal* sampled at *sample_rate*.

    The x-axis is logarithmic and the y-axis shows amplitude in decibels.
    Returns the frequency bins and amplitudes in dB.
    """
    spectrum = fft(signal)
    n = spectrum.size
    freqs = np.fft.fftfreq(n, d=1.0 / sample_rate)[: n // 2]
    magnitude = np.abs(spectrum[: n // 2])
    magnitude_db = 20 * np.log10(np.maximum(magnitude, 1e-12))

    plt.figure()
    plt.plot(freqs, magnitude_db)
    plt.xscale("log")
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("Amplitude [dB]")
    plt.title("FFT Spectrum")
    plt.grid(True, which="both")
    plt.show()

    return freqs, magnitude_db


__all__ = ["fft", "ifft", "display_fft"]
