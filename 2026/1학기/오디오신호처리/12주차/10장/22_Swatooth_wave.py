import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# 기본 주파수
f0 = 440

# 샘플링 주파수
fs = 44100

dur = 1

t = np.arange(0, dur, 1/fs)

# 고조파 수
num_harmonics = 20

# 톱니파 스타일 생성
x = np.zeros_like(t)

for k in range(1, num_harmonics + 1):

    x += (1/k) * np.sin(2*np.pi*k*f0*t)

# 정규화
x = x / np.max(np.abs(x))

# 소리 재생
print('Sawtooth Wave Sound')

sd.play(x, fs)
sd.wait()

# 시간영역 파형
t_plot = 0.005
N_plot = int(fs * t_plot)

plt.figure(figsize=(10,4))

plt.plot(t[:N_plot], x[:N_plot],
         'b', linewidth=1.5)

plt.title('Sawtooth Wave - Time Domain')

plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.grid(True)

plt.show()

# 주파수 스펙트럼 생성
X = np.fft.fft(x)

freq = np.fft.fftfreq(len(x), 1/fs)

half = len(x)//2

mag = 2*np.abs(X[:half]) / len(x)

plt.figure(figsize=(10,4))

plt.plot(freq[:half], mag,
         linewidth=1.5)

plt.title('Sawtooth Wave - Frequency Domain')

plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')

plt.xlim([0, 10000])

plt.grid(True)

plt.show()