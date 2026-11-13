import numpy as np
import matplotlib.pyplot as plt

# ---------------------------------------------------------
# 1) 이미지 로드 (OpenCV 필요 없음)
# ---------------------------------------------------------
img = plt.imread(r"C:\Users\jo_dd\Downloads\imag_1d_ext_n_filtered.png")


# 그레이스케일이 아닌 RGB이면 → R/G/B 중 한 채널만 사용
if img.ndim == 3:
    img = img[:, :, 0]

# 특정 행 선택
row_idx = 40
x = img[row_idx, :].astype(float)

# ---------------------------------------------------------
# 2) 11점 이동 평균 필터
# ---------------------------------------------------------
kernel = np.ones(11) / 11
y = np.convolve(x, kernel, mode="same")

# ---------------------------------------------------------
# (제출 1) 입력 vs 필터 출력
# ---------------------------------------------------------
plt.figure(figsize=(12,4))
plt.plot(x, 'b', label='Input signal (x)')
plt.plot(y, 'r', label='Filtered output (y)')
plt.title("Input vs 11-point Moving Average Output")
plt.legend()
plt.grid()
plt.savefig("20223156_input_vs_output.png", dpi=300)

# ---------------------------------------------------------
# (제출 2) 5-sample delay 보정
# ---------------------------------------------------------
y_adj = np.roll(y, -5)
y_adj[-5:] = y_adj[-6]  # 끝부분 보정(Optional)

plt.figure(figsize=(12,4))
plt.plot(x, 'b', label='Input signal (x)')
plt.plot(y_adj, 'r', label='Output shifted -5 samples')
plt.title("Input vs Time-Adjusted Output")
plt.legend()
plt.grid()
plt.savefig("20223156_input_vs_output_adjusted.png", dpi=300)

plt.show()
