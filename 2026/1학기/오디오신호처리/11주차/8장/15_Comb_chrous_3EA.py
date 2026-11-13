import numpy as np
import sounddevice as sd
from scipy.io import wavfile

# =========================================
# WAV Load
# =========================================
fs, x = wavfile.read('test_voice.wav')

x = x.astype(np.float32)
x = x / np.max(np.abs(x))

# =========================================
# Chorus Parameters
# =========================================
voices = [
    (30, 3, 0.3),
    (50, 4, 0.5),
    (70, 5, 0.7)
]

mix = 0.4

# =========================================
# Multi Chorus
# =========================================
y = np.copy(x)

for base_ms, depth_ms, rate in voices:

    base_delay = int(base_ms * fs / 1000)
    depth = int(depth_ms * fs / 1000)

    for n in range(len(x)):

        current_delay = (
            base_delay
            +
            int(
                depth
                *
                np.sin(2*np.pi*rate*n/fs)
            )
        )

        if n - current_delay >= 0:
            y[n] += mix * x[n-current_delay]

# normalize
y = y / np.max(np.abs(y))

# =========================================
# Playback
# =========================================
print('Original')
sd.play(x, fs)
sd.wait()

print('Multi Chorus')
sd.play(y, fs)
sd.wait()