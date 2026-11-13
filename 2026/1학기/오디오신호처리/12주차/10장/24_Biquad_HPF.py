import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd
from scipy import signal

# 기본 주파수
f0 = 440

# 샘플링 주파수
fs = 44100

dur = 2

t = np.arange(0, dur, 1/fs)

# 톱니파 생성
num_harmonics = 20

x = np.zeros_like(t)

for k in range(1, num_harmonics + 1):

    x += (1/k) * np.sin(2*np.pi*k*f0*t)

# 입력신호 크기 정규화
#x = x / np.max(np.abs(x))

# 바이쿼드 HPF 생성
fc = 1500

b, a = signal.iirfilter(
    N=2,
    Wn=fc,
    btype='high',
    ftype='butter',
    fs=fs
)

# 입력 신호 필터 통과
y = signal.lfilter(b, a, x)

# 출력 신호 크기 정규화
#y = y / np.max(np.abs(y))

# 소리 재생
print('Original Sawtooth')

sd.play(x, fs)
sd.wait()

print('HPF Applied')

sd.play(y, fs)
sd.wait()

# 시간 영역 파형
t_plot = 0.005
N_plot = int(fs * t_plot)

plt.figure(figsize=(10,4))

plt.plot(t[:N_plot], x[:N_plot],
         label='Original')

plt.plot(t[:N_plot], y[:N_plot],
         label='HPF Output')

plt.title('Time Domain')

plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.legend()

plt.grid(True)

plt.show()


# 주파수 스펙트럼 생성
X = np.fft.fft(x)
Y = np.fft.fft(y)

freq = np.fft.fftfreq(len(x), 1/fs)

half = len(x)//2

magX = 2*np.abs(X[:half]) / len(x)
magY = 2*np.abs(Y[:half]) / len(y)

plt.figure(figsize=(10,4))

plt.plot(freq[:half], magX,
         label='Original')

plt.plot(freq[:half], magY,
         label='HPF Output')

plt.title('Frequency Domain')

plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')

plt.xlim([0, 10000])

plt.legend()

plt.grid(True)

plt.show()