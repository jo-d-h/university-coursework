import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd
from scipy.signal import freqz

# 신호의 주파수
f = 440

# 샘플링 주파수
fs = 44100

# 샘플링된 신호
t_sample = np.arange(0, 1, 1/fs)
x_sample = np.cos(2*np.pi*f*t_sample)

# 지연된 신호
D = 10
x_delay = np.zeros_like(x_sample)
x_delay[D:] = x_sample[:-D]

# 콤 필터
y = x_sample - x_delay

# 정현파 소리 재생
print('x[n] Sound')
sd.play(x_sample, fs)
sd.wait()

# 지연된 신호 소리 재생
print('x[n-10] Sound')
sd.play(x_delay, fs)
sd.wait()

# 콤 필터 이후 소리
print('y[n] sound')
sd.play(y, fs)
sd.wait()

# 그래프 관찰을 위한 시간 축 조절 0~0.002초 구간
t_plot = 0.002

N = int(fs*t_plot)

plt.figure(figsize=(10,8))

# 원신호 그래프
plt.subplot(3,1,1)
plt.stem(t_sample[:N], x_sample[:N],
         linefmt='C0-',
         markerfmt='C0o',
         basefmt=' ',
         label='Sampled data')

plt.title('440 Hz sampled at 44100 Hz')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.legend(loc='upper right')
plt.grid(True)

# 지연 신호
plt.subplot(3, 1, 2)
plt.stem(t_sample[:N], x_delay[:N],
         linefmt='C1-',
         markerfmt='C1o',
         basefmt=' ',
         label='Delayed x[n-10]')
plt.title('Delayed Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend(loc='upper right')
plt.grid(True)

# Comb filter 출력
plt.subplot(3, 1, 3)
plt.stem(t_sample[:N], y[:N],
         linefmt='C2-',
         markerfmt='C2o',
         basefmt=' ',
         label='y[n] = x[n] - x[n-10]')
plt.title('Comb Filter Output')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend(loc='upper right')
plt.grid(True)

plt.tight_layout()
plt.show()

# 차분 방정식의 입력항의 계수
b = np.zeros(D + 1)
b[0] = 1
b[D] = -1

# 차분 방정식의 출력항의 계수
a = [1]

# 주파수 응답 표현
f_axis, H = freqz(b, a, worN=4096, fs=fs)

# 주파수 응답 진폭 스펙트럼 표현
plt.figure(figsize=(10,4))

plt.plot(f_axis, np.angle(H), linewidth=2)

plt.title('Magnitude Response of Comb Filter')
plt.xlabel('Frequency (Hz)')
plt.ylabel('|H(f)|')

plt.grid(True)
plt.xlim([0, fs/2])

plt.show()