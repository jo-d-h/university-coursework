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

gL_linear = 1 - p
gR_linear = p

left_linear = gL_linear * x
right_linear = gR_linear * x

stereo_linear = np.column_stack((left_linear, right_linear))

print('Original Mono')
sd.play(x, fs)
sd.wait()

print('Linear Panning')
sd.play(stereo_linear, fs)
sd.wait()