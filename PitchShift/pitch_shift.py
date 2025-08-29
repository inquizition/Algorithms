"""Utility for pitch shifting audio signals using a phase vocoder.

This module provides a :func:`pitch_shift` function that shifts the pitch of an
input audio signal by a given number of semitone steps while preserving the
original duration.  The implementation is based on the short-time Fourier
transform (STFT) with phase adjustment to maintain spectral continuity between
frames.  Overlap-add synthesis with a Hann window is used to reconstruct the
output signal.
"""

from __future__ import annotations

from typing import Tuple

import numpy as np
from scipy.signal import stft, istft, resample


def _phase_vocoder(
    stft_matrix: np.ndarray, rate: float, hop: int
) -> np.ndarray:
    """Time-stretch an STFT matrix using a phase vocoder.

    Parameters
    ----------
    stft_matrix:
        Complex STFT matrix with shape (n_frequencies, n_frames).
    rate:
        Rate by which to stretch the time axis.  Values greater than 1 speed up
        the signal (shorter output), while values less than 1 slow it down.
    hop:
        Hop size used in the original STFT.

    Returns
    -------
    np.ndarray
        The time-stretched STFT matrix.
    """

    n_bins, n_frames = stft_matrix.shape
    time_steps = np.arange(0, n_frames, rate, dtype=float)

    # Phase advance for each bin
    omega = 2 * np.pi * hop * np.arange(n_bins) / (2 * (n_bins - 1))

    # Output buffer
    stretched = np.zeros((n_bins, len(time_steps)), dtype=np.complex128)

    # Initialize phases
    phase = np.angle(stft_matrix[:, 0])
    stretched[:, 0] = np.abs(stft_matrix[:, 0]) * np.exp(1j * phase)

    for i, step in enumerate(time_steps[1:], start=1):
        idx = int(np.floor(step))
        frac = step - idx

        if idx + 1 >= n_frames:
            break

        # Linear magnitude interpolation
        mag = (1 - frac) * np.abs(stft_matrix[:, idx]) + frac * np.abs(
            stft_matrix[:, idx + 1]
        )

        # Phase difference
        delta = np.angle(stft_matrix[:, idx + 1]) - np.angle(stft_matrix[:, idx]) - omega
        delta = (delta + np.pi) % (2 * np.pi) - np.pi

        phase += omega + delta
        stretched[:, i] = mag * np.exp(1j * phase)

    return stretched[:, : i + 1]


def pitch_shift(signal: np.ndarray, sr: int, n_steps: float) -> np.ndarray:
    """Shift the pitch of ``signal`` by ``n_steps`` semitones.

    Parameters
    ----------
    signal:
        Input audio signal.
    sr:
        Sampling rate of ``signal`` in Hertz.
    n_steps:
        Number of semitones to shift. Positive values raise the pitch and
        negative values lower it.

    Returns
    -------
    np.ndarray
        The pitch-shifted signal with the same length and dtype as the input.
    """

    if signal.ndim != 1:
        raise ValueError("`signal` must be a mono (1D) array")

    # Analysis parameters
    n_fft = 2048
    hop = n_fft // 4
    window = np.hanning(n_fft)

    # STFT
    _, _, Zxx = stft(signal, fs=sr, window=window, nperseg=n_fft, noverlap=n_fft - hop)

    # Time-stretch using phase vocoder
    rate = 2 ** (-n_steps / 12.0)
    Zxx_stretch = _phase_vocoder(Zxx, rate, hop)

    # Inverse STFT with overlap-add
    _, y_stretch = istft(Zxx_stretch, fs=sr, window=window, nperseg=n_fft, noverlap=n_fft - hop)

    # Resample to match original length
    y = resample(y_stretch, len(signal))

    return y.astype(signal.dtype)

