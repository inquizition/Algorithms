import os
import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_int32, c_size_t, POINTER


FRACTIONAL_BITS = 15
_SCALE = 1 << FRACTIONAL_BITS
_MAX_Q = np.iinfo(np.int32).max
_MIN_Q = np.iinfo(np.int32).min
_MAX_VAL = _MAX_Q / _SCALE
_MIN_VAL = _MIN_Q / _SCALE


_lib = None


def _get_lib():
    global _lib
    if _lib is None:
        lib_path = os.path.join(os.path.dirname(__file__), "libfft.so")
        _lib = CDLL(lib_path)
        _lib.fft.argtypes = [POINTER(c_int32), POINTER(c_int32), c_size_t]
        _lib.ifft.argtypes = [POINTER(c_int32), POINTER(c_int32), c_size_t]
    return _lib


def _to_q_format(values):
    values = np.asarray(values, dtype=np.float64)
    values = np.clip(values, _MIN_VAL, _MAX_VAL)
    scaled = values * _SCALE
    scaled = np.where(scaled >= 0.0, scaled + 0.5, scaled - 0.5)
    scaled = np.clip(scaled, _MIN_Q, _MAX_Q)
    return scaled.astype(np.int32)


def _from_q_format(values):
    values = np.asarray(values, dtype=np.int32)
    return values.astype(np.float64) / _SCALE


def fft(signal):
    """Compute the FFT of *signal* using the C implementation."""
    lib = _get_lib()
    signal = np.asarray(signal, dtype=np.complex128)
    n = signal.shape[0]
    if n & (n - 1) != 0:
        raise ValueError("size of signal must be a power of 2")
    real = np.ascontiguousarray(_to_q_format(signal.real))
    imag = np.ascontiguousarray(_to_q_format(signal.imag))
    lib.fft(real.ctypes.data_as(POINTER(c_int32)),
            imag.ctypes.data_as(POINTER(c_int32)),
            n)
    real_f = _from_q_format(real)
    imag_f = _from_q_format(imag)
    return real_f + 1j * imag_f


def ifft(spectrum):
    """Compute the inverse FFT of *spectrum* using the C implementation."""
    lib = _get_lib()
    spectrum = np.asarray(spectrum, dtype=np.complex128)
    n = spectrum.shape[0]
    real = np.ascontiguousarray(_to_q_format(spectrum.real))
    imag = np.ascontiguousarray(_to_q_format(spectrum.imag))
    lib.ifft(real.ctypes.data_as(POINTER(c_int32)),
             imag.ctypes.data_as(POINTER(c_int32)),
             n)
    real_f = _from_q_format(real)
    imag_f = _from_q_format(imag)
    return real_f + 1j * imag_f


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
