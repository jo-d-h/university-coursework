import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# 원 신호
f = 440
fs = 44100
dur = 3

t = np.arange(0, dur, 1/fs)
x = np.cos(2*np.pi*f*t)

# Flanger 신호 생성
base_delay_ms = 2       # 기본 지연 시간
depth_ms = 2            # 지연 변화 폭
rate = 0.5              # 지연 변화 속도(Hz)
mix = 0.7               # 지연 신호 섞는 비율

# 2ms 지연에 맞는 샘플 개수
base_delay = int(base_delay_ms * fs / 1000)

# 2ms 깊이에 맞는 샘플 개수
depth = int(depth_ms * fs / 1000)

# 출력 신호
y = np.zeros_like(x)

for n in range(len(x)):
    # 시간에 따라 변하는 delay
    current_delay = base_delay + int(depth * (1 + np.sin(2*np.pi*rate*t[n])) / 2)

    if n - current_delay >= 0:
        y[n] = x[n] + mix * x[n - current_delay]
    else:
        y[n] = x[n]

# 음량 정규화
y = y / np.max(np.abs(y))

# 원음 재생
print("Original Sound")
sd.play(x, fs)
sd.wait()

# 플랜저 적용 소리 재생
print("Flanger Sound")
sd.play(y, fs)
sd.wait()

# 그래프 일부 표시
N = int(fs * 0.02)  # 20 ms

plt.figure(figsize=(10, 6))

plt.subplot(2,1,1)
plt.plot(t[:N], x[:N])
plt.title('Original Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.subplot(2,1,2)
plt.plot(t[:N], y[:N])
plt.title('Flanger Output')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.tight_layout()
plt.show()