import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# =========================================
# Parameters
# =========================================
f = 440
fs = 44100

# 시간축
t_sample = np.arange(0, 1, 1/fs)

# =========================================
# Fundamental + Harmonics
# =========================================
x1 = np.cos(2*np.pi*f*t_sample)

x2 = (1/4)*np.cos(2*np.pi*2*f*t_sample)

x3 = (1/9)*np.cos(2*np.pi*3*f*t_sample)

x4 = (1/16)*np.cos(2*np.pi*4*f*t_sample)

# 전자 오르간 스타일 합성
y = x1 + x2 + x3 + x4

# =========================================
# Sound Playback
# =========================================
print("Electronic Organ Style")

# 재생 시 clipping 방지를 위해 볼륨만 감소
sd.play(0.5*y, fs)
sd.wait()

# =========================================
# Time-domain waveform
# =========================================
t_plot = 0.005
N = int(fs*t_plot)

# 연속 신호 표현용
t_cont = np.linspace(0, t_plot, 5000)

x1_cont = np.cos(2*np.pi*f*t_cont)

x2_cont = (1/4)*np.cos(2*np.pi*2*f*t_cont)

x3_cont = (1/9)*np.cos(2*np.pi*3*f*t_cont)

x4_cont = (1/16)*np.cos(2*np.pi*4*f*t_cont)

y_cont = x1_cont + x2_cont + x3_cont + x4_cont

plt.figure(figsize=(10,4))

plt.plot(t_cont, y_cont,
         'k--',
         label='Continuous-like signal')

plt.stem(t_sample[:N], y[:N],
         linefmt='C0-',
         markerfmt='C0o',
         basefmt=' ',
         label='Sampled data')

plt.title('Electronic Organ Style Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.legend(loc='upper right')
plt.grid(True)

plt.show()

# =========================================
# Frequency Spectrum
# =========================================
Y = np.fft.fft(y)

freq = np.fft.fftfreq(len(y), 1/fs)

half = len(y)//2

freq_pos = freq[:half]

# 실제 진폭에 가깝게 표현
Y_mag = 2*np.abs(Y[:half]) / len(y)

plt.figure(figsize=(10,4))

plt.plot(freq_pos,
         Y_mag,
         linewidth=1.5)

plt.title('Spectrum of Electronic Organ Style')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')

plt.xlim([0, 2500])

plt.grid(True)

plt.show()