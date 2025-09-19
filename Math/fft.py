"""Python helpers for working with the FFT C implementation."""

from __future__ import annotations

import ctypes
import math
import sys
from pathlib import Path
from typing import Iterable, Optional, Sequence

try:  # NumPy is optional at import time; the module works without it.
    import numpy as np
except ImportError:  # pragma: no cover - optional dependency
    np = None  # type: ignore

from typing import TYPE_CHECKING

if TYPE_CHECKING:  # pragma: no cover - type-checking only imports
    import numpy as np  # type: ignore[no-redef]


_FRACTIONAL_BITS = 15
_SCALE = 1 << _FRACTIONAL_BITS
_MAX_Q = (1 << 31) - 1
_MIN_Q = -(1 << 31)
_MAX_VAL = _MAX_Q / _SCALE
_MIN_VAL = _MIN_Q / _SCALE

_LIB_HANDLE: Optional[ctypes.CDLL] = None


def _candidate_library_names() -> Iterable[str]:
    """Return possible shared library names for the current platform."""

    if sys.platform.startswith("win"):
        yield "fft.dll"
    elif sys.platform == "darwin":
        yield "libfft.dylib"
    else:
        yield "libfft.so"


def _candidate_directories(build_dir: Optional[Path]) -> Iterable[Path]:
    """Return directories that may contain the compiled shared library."""

    module_dir = Path(__file__).resolve().parent
    if build_dir is not None:
        yield build_dir
        yield build_dir / "lib"
        yield build_dir / "Math"
        yield build_dir / "Math" / "lib"

    yield module_dir
    yield module_dir / "build"

    repo_root = module_dir.parent
    yield repo_root / "build" / "Math"
    yield repo_root / "build" / "lib"
    yield repo_root / "build"
    yield repo_root / "lib"


def _find_library_path(build_directory: Optional[str] = None) -> Path:
    """Locate the compiled FFT shared library."""

    build_dir = Path(build_directory).resolve() if build_directory else None
    for directory in _candidate_directories(build_dir):
        for name in _candidate_library_names():
            candidate = directory / name
            if candidate.exists():
                return candidate

    search_locations = [str(p) for p in _candidate_directories(build_dir)]
    raise FileNotFoundError(
        "Could not locate the FFT shared library. "
        "Ensure the project is built with CMake and, if using a custom "
        "build directory, pass it via the 'build_directory' argument. "
        f"Checked: {search_locations}"
    )


def load_library(build_directory: Optional[str] = None) -> ctypes.CDLL:
    """Load and memoise the FFT shared library."""

    global _LIB_HANDLE
    if _LIB_HANDLE is None:
        library_path = _find_library_path(build_directory)
        _LIB_HANDLE = ctypes.CDLL(str(library_path))
        _LIB_HANDLE.fft.argtypes = [
            ctypes.POINTER(ctypes.c_int32),
            ctypes.POINTER(ctypes.c_int32),
            ctypes.c_size_t,
        ]
        _LIB_HANDLE.fft.restype = None
        _LIB_HANDLE.ifft.argtypes = [
            ctypes.POINTER(ctypes.c_int32),
            ctypes.POINTER(ctypes.c_int32),
            ctypes.c_size_t,
        ]
        _LIB_HANDLE.ifft.restype = None
    return _LIB_HANDLE


def _to_q_format_numpy(values: "np.ndarray") -> "np.ndarray":
    clipped = np.clip(values.astype(np.float64, copy=False), _MIN_VAL, _MAX_VAL)
    scaled = np.rint(clipped * _SCALE)
    saturated = np.clip(scaled, _MIN_Q, _MAX_Q)
    return np.ascontiguousarray(saturated.astype(np.int32))


def _from_q_format_numpy(values: "np.ndarray") -> "np.ndarray":
    return values.astype(np.float64, copy=False) / _SCALE


def _to_q_format_ctypes(values: Sequence[float]) -> ctypes.Array[ctypes.c_int32]:
    ints: list[int] = []
    for value in values:
        clipped = float(max(min(value, _MAX_VAL), _MIN_VAL))
        scaled = clipped * _SCALE
        if scaled >= 0:
            rounded = math.floor(scaled + 0.5)
        else:
            rounded = math.ceil(scaled - 0.5)
        saturated = int(max(min(rounded, _MAX_Q), _MIN_Q))
        ints.append(saturated)

    array_type = ctypes.c_int32 * len(ints)
    return array_type(*ints)


def _from_q_format_ctypes(values: Sequence[int]) -> list[float]:
    return [float(v) / _SCALE for v in values]


def _validate_length(length: int) -> None:
    if length == 0:
        raise ValueError("Signal must contain at least one sample.")
    if length & (length - 1) != 0:
        raise ValueError("Length of signal must be a power of two.")


def fft(
    signal: Sequence[complex] | Sequence[float] | "np.ndarray",
    *,
    build_directory: Optional[str] = None,
) -> "np.ndarray" | list[complex]:
    """Compute the FFT of *signal* using the C implementation."""

    if np is not None and isinstance(signal, np.ndarray):
        np_signal = np.ascontiguousarray(signal, dtype=np.complex128)
        if np_signal.ndim != 1:
            raise ValueError("FFT input must be one-dimensional.")

        length = np_signal.size
        if length == 0:
            return np.empty(0, dtype=np.complex128)
        _validate_length(length)

        real_buffer = _to_q_format_numpy(np_signal.real)
        imag_buffer = _to_q_format_numpy(np_signal.imag)

        lib = load_library(build_directory)
        lib.fft(
            real_buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
            imag_buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
            ctypes.c_size_t(length),
        )

        real = _from_q_format_numpy(real_buffer)
        imag = _from_q_format_numpy(imag_buffer)
        return real + 1j * imag

    values = [complex(sample) for sample in signal]
    length = len(values)
    if length == 0:
        return []
    _validate_length(length)

    real_buffer = _to_q_format_ctypes([value.real for value in values])
    imag_buffer = _to_q_format_ctypes([value.imag for value in values])

    lib = load_library(build_directory)
    lib.fft(real_buffer, imag_buffer, ctypes.c_size_t(length))

    real = _from_q_format_ctypes(real_buffer)
    imag = _from_q_format_ctypes(imag_buffer)
    return [complex(r, i) for r, i in zip(real, imag)]


def ifft(
    spectrum: Sequence[complex] | Sequence[float] | "np.ndarray",
    *,
    build_directory: Optional[str] = None,
) -> "np.ndarray" | list[complex]:
    """Compute the inverse FFT of *spectrum* using the C implementation."""

    if np is not None and isinstance(spectrum, np.ndarray):
        np_spectrum = np.ascontiguousarray(spectrum, dtype=np.complex128)
        if np_spectrum.ndim != 1:
            raise ValueError("FFT input must be one-dimensional.")

        length = np_spectrum.size
        if length == 0:
            return np.empty(0, dtype=np.complex128)
        _validate_length(length)

        real_buffer = _to_q_format_numpy(np_spectrum.real)
        imag_buffer = _to_q_format_numpy(np_spectrum.imag)

        lib = load_library(build_directory)
        lib.ifft(
            real_buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
            imag_buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
            ctypes.c_size_t(length),
        )

        real = _from_q_format_numpy(real_buffer)
        imag = _from_q_format_numpy(imag_buffer)
        return real + 1j * imag

    values = [complex(sample) for sample in spectrum]
    length = len(values)
    if length == 0:
        return []
    _validate_length(length)

    real_buffer = _to_q_format_ctypes([value.real for value in values])
    imag_buffer = _to_q_format_ctypes([value.imag for value in values])

    lib = load_library(build_directory)
    lib.ifft(real_buffer, imag_buffer, ctypes.c_size_t(length))

    real = _from_q_format_ctypes(real_buffer)
    imag = _from_q_format_ctypes(imag_buffer)
    return [complex(r, i) for r, i in zip(real, imag)]


def plot_fft(
    signal: Sequence[complex] | Sequence[float] | "np.ndarray",
    sample_rate: float,
    *,
    build_directory: Optional[str] = None,
) -> tuple["np.ndarray", "np.ndarray"] | tuple[list[float], list[float]]:
    """Display the magnitude spectrum of *signal* sampled at *sample_rate*."""

    import matplotlib.pyplot as plt

    spectrum = fft(signal, build_directory=build_directory)

    if np is not None:
        spectrum_array = np.asarray(spectrum, dtype=np.complex128)
        spectrum_array = np.ascontiguousarray(spectrum_array)
        length = spectrum_array.size
        if length == 0:
            raise ValueError("Signal must contain at least one sample.")

        half = max(length // 2, 1)
        freqs = np.fft.fftfreq(length, d=1.0 / sample_rate)[:half]
        magnitude = np.abs(spectrum_array[:half])
        magnitude_db = 20.0 * np.log10(np.maximum(magnitude, 1e-12))

        freq_values = freqs
        magnitude_values = magnitude_db
    else:
        values = [complex(x) for x in spectrum]
        length = len(values)
        if length == 0:
            raise ValueError("Signal must contain at least one sample.")

        half = max(length // 2, 1)
        freq_values = [sample_rate * k / length for k in range(half)]
        magnitude = [abs(values[k]) for k in range(half)]
        magnitude_values = [
            20.0 * math.log10(m if m > 1e-12 else 1e-12) for m in magnitude
        ]

    plt.figure(figsize=(10, 6))
    plt.plot(freq_values, magnitude_values, label="Magnitude")
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("Amplitude [dB]")
    plt.title("FFT Spectrum")
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.show()

    return freq_values, magnitude_values


def display_fft(
    signal: Sequence[complex] | Sequence[float] | "np.ndarray",
    sample_rate: float,
    *,
    build_directory: Optional[str] = None,
):
    """Backward-compatible alias for :func:`plot_fft`."""

    return plot_fft(signal, sample_rate, build_directory=build_directory)


def demo(build_directory: Optional[str] = None) -> None:
    """Showcase the FFT magnitude spectrum with a Matplotlib plot."""

    import random

    if np is not None:
        duration = 1.0
        sample_rate = 1024
        time = np.linspace(0.0, duration, int(duration * sample_rate), endpoint=False)
        signal = 0.7 * np.sin(2 * np.pi * 50 * time)
        signal += 0.3 * np.sin(2 * np.pi * 120 * time)
        rng = np.random.default_rng(seed=2024)
        noisy_signal = signal + 0.2 * rng.standard_normal(size=time.shape)
    else:
        duration = 1.0
        sample_rate = 1024
        length = int(duration * sample_rate)
        time = [i / sample_rate for i in range(length)]
        signal = [
            0.7 * math.sin(2 * math.pi * 50 * t) +
            0.3 * math.sin(2 * math.pi * 120 * t)
            for t in time
        ]
        random.seed(2024)
        noisy_signal = [value + 0.2 * random.gauss(0.0, 1.0) for value in signal]

    plot_fft(noisy_signal, sample_rate, build_directory=build_directory)


__all__ = ["fft", "ifft", "plot_fft", "display_fft", "demo", "load_library"]
