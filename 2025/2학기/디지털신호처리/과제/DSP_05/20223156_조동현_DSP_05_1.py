import numpy as np
import matplotlib.pyplot as plt

# --- 설정: 주기 ---
T = 0.04
omega0 = 2 * np.pi / T

# --- 원래 삼각파 정의 (0→1→0) ---
def triangle(t):
    tmod = np.mod(t, T)
    half = T / 2
    return np.where(tmod <= half, (2.0 / T) * tmod, (2.0 / T) * (T - tmod))

# --- 퓨리에 급수 부분합 ---
def fourier_partial(t, k_max):
    a0 = 1/2.0
    s = a0 * np.ones_like(t)
    for k in range(1, k_max + 1):
        if k % 2 == 0:   # 짝수항은 0
            continue
        ak = -4.0 / (np.pi**2 * k**2)   # 홀수항 계수
        s += ak * np.cos(omega0 * k * t)
    return s

# --- 시간축 (2주기) ---
t = np.linspace(0, 2*T, 2000)
orig = triangle(t)
approx_k3 = fourier_partial(t, 3)
approx_k11 = fourier_partial(t, 11)

# --- 전체 그래프: 원래 파형, 근사, 오차 ---
plt.figure(figsize=(12,5))

# 원래 파형
plt.plot(t, orig, 'k', linewidth=2, label='Original triangle')

# 퓨리에 근사
plt.plot(t, approx_k3, 'r--', label='Fourier approx (k≤3)')
plt.plot(t, approx_k11, 'b--', label='Fourier approx (k≤11)')

# 오차 파형
plt.plot(t, orig - approx_k3, 'r:', label='Error (k≤3)')
plt.plot(t, orig - approx_k11, 'b:', label='Error (k≤11)')

plt.title("Triangle wave, Fourier approximations, and Errors")
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")
plt.legend()
plt.grid(True)
plt.show()

# --- 최대 오차 계산 ---
err3 = np.max(np.abs(orig - approx_k3))
err11 = np.max(np.abs(orig - approx_k11))
print(f"Max error (k≤3): {err3:.6f}")
print(f"Max error (k≤11): {err11:.6f}")
