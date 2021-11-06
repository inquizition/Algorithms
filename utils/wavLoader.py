import numpy as np
import wave

def everyOther (v, offset=0):
    return [v[i] for i in range(offset, len(v), 2)]

def wavLoad (fname):
    wav = wave.open(fname, "r")
    (nchannels, sampwidth, framerate, nframes, comptype, compname) = wav.getparams()
    frames = wav.readframes(nframes*nchannels)
    out = np.struct.unpack_from("%dh" % nframes*nchannels, frames)

    if nchannels == 2:
        left = np.array(list (everyOther(out, 0)))
        right = np.array(list (everyOther(out,1)))
    else:
        left = np.array(out)
        right = left
