from scipy.io import wavfile
import matplotlib.pyplot as plt
import numpy as np
import sounddevice as sd

# WAV 파일 읽기
fs, x = wavfile.read('original_20223156.wav')

# 정수형 → float 변환
x = x.astype(np.float32)

# 시간축 생성
t = np.arange(len(x)) / fs

# 정규화
x = x / np.max(np.abs(x))

# Flanger 신호 생성
base_delay_ms = 20      # 기본 지연 시간
depth_ms = 5            # 지연 변화 폭
rate = 0.3             # 지연 변화 속도(Hz)
mix = 0.6              # 지연 신호 섞는 비율

# 지연에 맞는 샘플 개수
base_delay = int(base_delay_ms * fs / 1000)

# 깊이에 맞는 샘플 개수
depth = int(depth_ms * fs / 1000)

# 출력 신호
y = np.zeros_like(x)

for n in range(len(x)):
    # 시간에 따라 변하는 delay
    current_delay = base_delay + int(depth * np.sin(2*np.pi*rate*t[n]))

    if n - current_delay >= 0:
        y[n] = x[n] + mix * x[n - current_delay]
    else:
        y[n] = x[n]

# 음량 정규화
y = y / np.max(np.abs(y))

#파일 저장
wavfile.write('chorus_20223156.wav', fs, (y * 32767).astype(np.int16))

"""
# 원음 재생
print("Original Sound")
sd.play(x, fs)
sd.wait()

# 코러스 적용 소리 재생
print("Chorus Sound")
sd.play(y, fs)
sd.wait()
"""

# 그래프 일부 표시
N = int(fs * 0.1)  # 100 ms

plt.figure(figsize=(18, 3))

plt.subplot(1,2,1)
plt.plot(t[:N], x[:N])
plt.title('Original Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.subplot(1,2,2)
plt.plot(t[:N], y[:N])
plt.title('Chorus Output')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.tight_layout()
plt.show()