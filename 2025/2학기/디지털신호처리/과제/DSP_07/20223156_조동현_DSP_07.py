import numpy as np
import matplotlib.pyplot as plt

# === 새 창으로 그래프 띄우는 설정 ===
plt.ion()  # interactive mode ON
plt.close('all')  # 이전 figure 창 닫기

# 정규화 주파수 및 스펙트럼 정보
freqs = np.array([0, 1/4, -1/4, 1/8, -1/8])
amps = np.array([2, 1.5, 1.5, 2.5, 2.5])
phases = np.array([0, -np.pi/16, np.pi/16, 3*np.pi/8, -3*np.pi/8])

# 플롯 생성
plt.figure(figsize=(8, 4))
plt.stem(freqs, amps, basefmt=" ")

plt.title("Discrete-Time Spectrum of x[n]")
plt.xlabel("Normalized Frequency  (f̂ = f / f_si)")
plt.ylabel("Amplitude")
plt.grid(True, linestyle="--", alpha=0.6)

# 각 주파수 성분에 진폭, 위상 표시
for f, a, p in zip(freqs, amps, phases):
    plt.text(f, a + 0.2, f"A={a}, ϕ={p:.2f}", ha='center', fontsize=9)

plt.tight_layout()
plt.show(block=True)  # block=True로 별도 창에 표시 후 닫을 때까지 대기
