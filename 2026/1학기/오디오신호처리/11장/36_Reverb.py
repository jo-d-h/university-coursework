import numpy as np
import sounddevice as sd
from scipy.io import wavfile

# WAV 파일 읽기
fs, x = wavfile.read('test_voice.wav')

# float 변환
x = x.astype(np.float32)

# 스테레오면 모노 변환
if x.ndim == 2:
    x = np.mean(x, axis=1)

# 정규화
x = x / np.max(np.abs(x))

# ==========================================
# Simple Multi Delay Reverb
# ==========================================

y = np.copy(x)

delays_ms = [20, 35, 50, 65]
decays = [0.5, 0.4, 0.3, 0.2]

for delay_ms, decay in zip(delays_ms, decays):

    D = int(delay_ms * fs / 1000)

    temp = np.zeros_like(x)

    temp[D:] = decay * x[:-D]

    y += temp

# 출력 정규화
y = y / np.max(np.abs(y))

# ==========================================
# Playback
# ==========================================

print('Original')
sd.play(x, fs)
sd.wait()

print('Simple Reverb')
sd.play(y, fs)
sd.wait()