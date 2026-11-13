import numpy as np
import sounddevice as sd
from scipy.io import wavfile

# WAV 파일 읽기
fs, x = wavfile.read('test_voice.wav')

# float 변환
x = x.astype(np.float32)

# 스테레오면 그대로 사용, 모노면 임시 스테레오 생성
if x.ndim == 1:
    # 모노 음성을 약간의 Haas delay로 스테레오화
    delay_ms = 15
    D = int(delay_ms * fs / 1000)

    left = x
    right = np.zeros_like(x)
    right[D:] = 0.3 * x[:-D]

else:
    left = x[:, 0]
    right = x[:, 1]

# 정규화
max_val = max(np.max(np.abs(left)), np.max(np.abs(right)))
left = left / max_val
right = right / max_val

# ==============================
# Mid-Side Encoding
# ==============================
M = (left + right) / 2
S = (left - right) / 2

# ==============================
# Stereo Width Control
# alpha = 0   : Mono
# alpha = 1   : Original width
# alpha > 1   : Wider stereo
# ==============================
alpha = 1.8

L_new = M + alpha * S
R_new = M - alpha * S

stereo_width = np.column_stack((L_new, R_new))

# 출력 정규화
stereo_width = stereo_width / np.max(np.abs(stereo_width))

# ==============================
# Playback
# ==============================
print('Original Stereo')
original_stereo = np.column_stack((left, right))
sd.play(original_stereo, fs)
sd.wait()

print('Mid-Side Width Applied')
sd.play(stereo_width, fs)
sd.wait()