import numpy as np
from PitchShift.pitch_shift import pitch_shift


def dominant_freq(signal: np.ndarray, sr: int) -> float:
    """Return the dominant frequency of ``signal`` in Hertz."""
    spectrum = np.fft.rfft(signal)
    freqs = np.fft.rfftfreq(len(signal), d=1 / sr)
    return float(freqs[np.argmax(np.abs(spectrum))])


def test_pitch_shift_octave_up() -> None:
    sr = 22050
    duration = 1.0
    t = np.linspace(0, duration, int(sr * duration), endpoint=False)
    freq = 440.0
    signal = np.sin(2 * np.pi * freq * t)
    shifted = pitch_shift(signal, sr, 12.0)
    peak = dominant_freq(shifted, sr)
    assert np.isclose(peak, freq * 2, atol=5.0)


def test_pitch_shift_octave_down() -> None:
    sr = 22050
    duration = 1.0
    t = np.linspace(0, duration, int(sr * duration), endpoint=False)
    freq = 440.0
    signal = np.sin(2 * np.pi * freq * t)
    shifted = pitch_shift(signal, sr, -12.0)
    peak = dominant_freq(shifted, sr)
    assert np.isclose(peak, freq / 2, atol=5.0)
