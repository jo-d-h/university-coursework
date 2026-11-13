import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# 기본 주파수
f = 440

# 샘플링 주파수
fs = 44100

# 시간축
t_sample = np.arange(0, 1, 1/fs)

# 톱니파 스타일: 모든 고조파 포함
num_harmonics = 10

y = np.zeros_like(t_sample)

for k in range(1, num_harmonics + 1):
    y += (1/k) * np.cos(2*np.pi*k*f*t_sample)

# 소리 재생
sd.play(0.3*y, fs)
sd.wait()

# 시간영역 파형
t_plot = 0.005
N = int(fs*t_plot)

plt.figure(figsize=(10,4))
plt.plot(t_sample[:N], y[:N], linewidth=1.5)
plt.title('Sawtooth Style Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show()

# 주파수 스펙트럼
Y = np.fft.fft(y)
freq = np.fft.fftfreq(len(y), 1/fs)

half = len(y)//2
freq_pos = freq[:half]
Y_mag = 2*np.abs(Y[:half]) / len(y)

idx = freq_pos <= 6000

plt.figure(figsize=(10,4))
plt.plot(freq_pos[idx], Y_mag[idx], linewidth=1.5)
plt.title('Spectrum of Sawtooth Style Signal')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.xlim([0, 6000])
plt.grid(True)
plt.show()