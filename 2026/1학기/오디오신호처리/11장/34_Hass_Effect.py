import numpy as np
import sounddevice as sd
from scipy.io import wavfile

# WAV 파일 읽기
fs, x = wavfile.read('test_voice.wav')

# float 변환
x = x.astype(np.float32)

# 스테레오면 모노로 변환
if x.ndim == 2:
    x = np.mean(x, axis=1)

# 정규화
x = x / np.max(np.abs(x))

# ==========================================
# Haas Effect
# ==========================================

delay_ms = 10

# ms → sample
D = int(delay_ms * fs / 1000)

# Left : original
left = x

# Right : delayed
right = np.zeros_like(x)

right[D:] = x[:-D]

# Stereo signal
stereo = np.column_stack((left, right))

# ==========================================
# Playback
# ==========================================

print('Haas Effect')

sd.play(stereo, fs)
sd.wait()