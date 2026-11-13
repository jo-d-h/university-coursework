import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# 기본 주파수
f = 440

# 샘플링 주파수
fs = 44100

# 시간축
t_sample = np.arange(0, 1, 1/fs)

# 클라리넷/목관악기 스타일: 홀수차 고조파
x1 = np.cos(2*np.pi*f*t_sample)
x3 = (1/3)*np.cos(2*np.pi*3*f*t_sample)
x5 = (1/5)*np.cos(2*np.pi*5*f*t_sample)
x7 = (1/7)*np.cos(2*np.pi*7*f*t_sample)

# 합성
y = x1 + x3 + x5 + x7

# 소리 재생
sd.play(0.5*y, fs)
sd.wait()

# 시간영역 파형
t_plot = 0.005
N = int(fs*t_plot)

t_cont = np.linspace(0, t_plot, 5000)

y_cont = (
    np.cos(2*np.pi*f*t_cont)
    + (1/3)*np.cos(2*np.pi*3*f*t_cont)
    + (1/5)*np.cos(2*np.pi*5*f*t_cont)
    + (1/7)*np.cos(2*np.pi*7*f*t_cont)
)

plt.figure(figsize=(10,4))

plt.plot(t_cont, y_cont, 'k--', label='Continuous-like signal')

plt.stem(t_sample[:N], y[:N],
         linefmt='C0-',
         markerfmt='C0o',
         basefmt=' ',
         label='Sampled data')

plt.title('Clarinet / Woodwind Style Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend(loc='upper right')
plt.grid(True)
plt.show()

# 주파수 스펙트럼
Y = np.fft.fft(y)
freq = np.fft.fftfreq(len(y), 1/fs)

half = len(y)//2
freq_pos = freq[:half]

Y_mag = 2*np.abs(Y[:half]) / len(y)

idx = freq_pos <= 4000

plt.figure(figsize=(10,4))

plt.plot(freq_pos[idx], Y_mag[idx], linewidth=1.5)

plt.title('Spectrum of Clarinet / Woodwind Style')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.xlim([0, 4000])
plt.grid(True)
plt.show()