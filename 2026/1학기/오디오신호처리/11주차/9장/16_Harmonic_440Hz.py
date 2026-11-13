import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# 신호의 주파수
f = 440

# 샘플링 주파수
fs = 44100

# 시간축
t_sample = np.arange(0, 1, 1/fs)

# 기본파
x1 = np.cos(2*np.pi*f*t_sample)

# 2차 고조파
x2 = 0.5*np.cos(2*np.pi*2*f*t_sample)

# 기본파 + 2차 고조파 합성
y = x1 + x2

# 소리 재생
print("Fundamental + 2nd Harmonic")
sd.play(0.5*y, fs)
sd.wait()

# 그래프 관찰을 위한 시간 축 조절
t_plot = 0.005
N = int(fs*t_plot)

# 비교용 연속 신호 표현용 시간축
t_cont = np.linspace(0, t_plot, 5000)

x1_cont = np.cos(2*np.pi*f*t_cont)
x2_cont = 0.5*np.cos(2*np.pi*2*f*t_cont)
y_cont = x1_cont + x2_cont

# =========================================
# Time-domain waveform
# =========================================
plt.figure(figsize=(10,4))

plt.plot(t_cont, y_cont, 'k--', label='Continuous-like signal')

plt.stem(t_sample[:N], y[:N],
         linefmt='C0-',
         markerfmt='C0o',
         basefmt=' ',
         label='Sampled data')

plt.title('440 Hz + 2nd Harmonic')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.legend(loc='upper right')
plt.grid(True)
plt.show()

# =========================================
# Frequency spectrum
# =========================================
Y = np.fft.fft(y)
freq = np.fft.fftfreq(len(y), 1/fs)

# 양의 주파수만 사용
half = len(y)//2
freq_pos = freq[:half]
Y_mag = np.abs(Y[:half]) / half

plt.figure(figsize=(10,4))

plt.plot(freq_pos, Y_mag, linewidth=1.5)

plt.title('Spectrum of 440 Hz + 2nd Harmonic')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')

plt.xlim([0, 3000])
plt.grid(True)

plt.show()