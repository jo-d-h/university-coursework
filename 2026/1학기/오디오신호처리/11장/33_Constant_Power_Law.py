import numpy as np
import sounddevice as sd
from scipy.io import wavfile

# WAV 파일 읽기
fs, x = wavfile.read('test_voice.wav')

# float 변환
x = x.astype(np.float32)

# 스테레오 파일이면 모노로 변환
if x.ndim == 2:
    x = np.mean(x, axis=1)

# 정규화
x = x / np.max(np.abs(x))

# ==============================
# Pan Position
# p = 0   : Left
# p = 0.5 : Center
# p = 1   : Right
# ==============================
p = 0.5

theta = p * np.pi / 2

gL_const = np.cos(theta)
gR_const = np.sin(theta)

left_const = gL_const * x
right_const = gR_const * x

stereo_const = np.column_stack((left_const, right_const))

# ==============================
# Playback
# ==============================
print('Original Mono')
sd.play(x, fs)
sd.wait()

print('Constant Power Panning')
sd.play(stereo_const, fs)
sd.wait()