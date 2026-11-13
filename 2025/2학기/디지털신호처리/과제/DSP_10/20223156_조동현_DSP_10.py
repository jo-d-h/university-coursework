import numpy as np
import matplotlib.pyplot as plt

# 입력 신호 x[n]
x = np.arange(1, 11)
# 임펄스 응답 h[n]
h = np.array([5, 4, 3, 2, 1])

# 길이 계산
m = len(x)
n = len(h)
y = np.zeros(m + n - 1, dtype=int)

for i in range(m):
    for j in range(n):
        y[i + j] += int(x[i] * h[j])  # 선형 컨볼루션 누산

print("y[n] =", list(y))

plt.figure(figsize=(8, 6))

# x[n]
plt.subplot(3, 1, 1)
plt.stem(range(len(x)), x, basefmt=" ")
plt.title("Input sequence x[n]")
plt.xlabel("n")
plt.ylabel("x[n]")
plt.grid(True)

# h[n]
plt.subplot(3, 1, 2)
plt.stem(range(len(h)), h, basefmt=" ")
plt.title("Impulse response h[n]")
plt.xlabel("n")
plt.ylabel("h[n]")
plt.grid(True)

# y[n]
plt.subplot(3, 1, 3)
plt.stem(range(len(y)), y, basefmt=" ")
plt.title("Output y[n] = x[n] * h[n] (linear convolution)")
plt.xlabel("n")
plt.ylabel("y[n]")
plt.grid(True)

plt.tight_layout()
plt.show()
