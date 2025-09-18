"""Python helpers for working with the Wiener filter C implementation."""

from __future__ import annotations

import ctypes
import sys
from pathlib import Path
from typing import Iterable, Optional, Sequence

try:  # NumPy is optional at import time; the module degrades gracefully otherwise.
    import numpy as np
except ImportError:  # pragma: no cover - optional dependency
    np = None  # type: ignore

from typing import TYPE_CHECKING

if TYPE_CHECKING:  # pragma: no cover - type-checking only imports
    import numpy as np  # type: ignore[no-redef]

_LIB_HANDLE: Optional[ctypes.CDLL] = None


def _candidate_library_names() -> Iterable[str]:
    """Return possible shared library names for the current platform."""
    if sys.platform.startswith("win"):
        yield "wiener_filter.dll"
    elif sys.platform == "darwin":
        yield "libwiener_filter.dylib"
    else:
        yield "libwiener_filter.so"


def _candidate_directories(build_dir: Optional[Path]) -> Iterable[Path]:
    """Return directories that may contain the compiled shared library."""
    repo_root = Path(__file__).resolve().parent.parent
    if build_dir is not None:
        yield build_dir
        yield build_dir / "lib"
    yield repo_root / "build" / "lib"
    yield repo_root / "build"
    yield repo_root / "lib"


def _find_library_path(build_directory: Optional[str] = None) -> Path:
    """Locate the compiled Wiener filter shared library."""
    build_dir = Path(build_directory).resolve() if build_directory else None
    for directory in _candidate_directories(build_dir):
        for name in _candidate_library_names():
            candidate = directory / name
            if candidate.exists():
                return candidate
    search_locations = [str(p) for p in _candidate_directories(build_dir)]
    raise FileNotFoundError(
        "Could not locate the Wiener filter shared library. "
        "Ensure the project is built with CMake and, if using a custom "
        "build directory, pass it via the 'build_directory' argument. "
        f"Checked: {search_locations}"
    )


def load_library(build_directory: Optional[str] = None) -> ctypes.CDLL:
    """Load and memoise the Wiener filter shared library."""
    global _LIB_HANDLE
    if _LIB_HANDLE is None:
        library_path = _find_library_path(build_directory)
        _LIB_HANDLE = ctypes.CDLL(str(library_path))
        _LIB_HANDLE.wiener_filter.argtypes = [
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.c_size_t,
            ctypes.c_size_t,
            ctypes.c_double,
        ]
        _LIB_HANDLE.wiener_filter.restype = None
    return _LIB_HANDLE


def apply_wiener_filter(
    signal: Sequence[float] | "np.ndarray",
    window_radius: int,
    noise_variance: float,
    *,
    build_directory: Optional[str] = None,
) -> "np.ndarray" | list[float]:
    """Filter a one-dimensional signal using the C implementation.

    Parameters
    ----------
    signal:
        Input 1-D signal. It is converted to a contiguous ``float64`` array.
    window_radius:
        Radius of the sliding window. A value of ``1`` corresponds to a
        three-sample window.
    noise_variance:
        Estimated variance of the additive noise in the signal.
    build_directory:
        Optional path to the CMake build directory. If omitted, standard
        locations such as ``./build`` are searched.
    """

    if np is not None and isinstance(signal, np.ndarray):
        np_signal = np.ascontiguousarray(np.squeeze(signal).astype(np.float64))
        if np_signal.ndim != 1:
            raise ValueError("The Wiener filter operates on one-dimensional signals.")

        result = np.empty_like(np_signal)

        lib = load_library(build_directory)
        lib.wiener_filter(
            np_signal.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
            result.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
            ctypes.c_size_t(len(np_signal)),
            ctypes.c_size_t(window_radius),
            ctypes.c_double(noise_variance),
        )
        return result

    values = [float(x) for x in signal]
    if not values:
        return []

    length = len(values)
    InputArray = ctypes.c_double * length
    OutputArray = ctypes.c_double * length
    input_buffer = InputArray(*values)
    output_buffer = OutputArray()

    lib = load_library(build_directory)
    lib.wiener_filter(
        input_buffer,
        output_buffer,
        ctypes.c_size_t(length),
        ctypes.c_size_t(window_radius),
        ctypes.c_double(noise_variance),
    )

    if np is not None:
        return np.ctypeslib.as_array(output_buffer).astype(np.float64, copy=True)
    return [output_buffer[i] for i in range(length)]


def demo(build_directory: Optional[str] = None) -> None:
    """Showcase the Wiener filter on a noisy sine wave with Matplotlib plots."""
    import math
    import random

    import matplotlib.pyplot as plt

    if np is not None:
        time = np.linspace(0, 1, 500)
        clean_signal = np.sin(2 * np.pi * 5 * time)
        rng = np.random.default_rng(seed=1234)
        noisy_signal = clean_signal + rng.normal(scale=0.4, size=time.shape)
    else:
        steps = 500
        time = [i / (steps - 1) for i in range(steps)]
        clean_signal = [math.sin(2 * math.pi * 5 * t) for t in time]
        random.seed(1234)
        noisy_signal = [value + random.gauss(0.0, 0.4) for value in clean_signal]

    filtered = apply_wiener_filter(
        noisy_signal,
        window_radius=3,
        noise_variance=0.16,
        build_directory=build_directory,
    )

    plt.figure(figsize=(10, 6))
    plt.plot(time, clean_signal, label="Original", linewidth=2)
    plt.plot(time, noisy_signal, label="Noisy", alpha=0.6)
    plt.plot(time, filtered, label="Filtered", linewidth=2)
    plt.xlabel("Time [s]")
    plt.ylabel("Amplitude")
    plt.title("Wiener Filter Demo")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    demo()
