from scipy.io import wavfile
import matplotlib.pyplot as plt
import numpy as np
import sounddevice as sd

# WAV 파일 읽기
fs, x = wavfile.read('test_voice.wav')

# 정수형 → float 변환
x = x.astype(np.float32)

# 정규화
x = x / np.max(np.abs(x))

# 재생
print('Original Voice')
sd.play(x, fs)
sd.wait()

# 시간축 생성
t = np.arange(len(x)) / fs

# 앞부분만 그래프 표시
N = int(fs * 0.02)   # 20ms

plt.figure(figsize=(10,4))

plt.plot(t[:N], x[:N])

plt.title('Voice Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.grid(True)

plt.show()