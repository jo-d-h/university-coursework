import numpy as np
import matplotlib.pyplot as plt

# ---------------------------------------------------------
# 1) 원본 이미지 로드
# ---------------------------------------------------------
img = plt.imread(r"C:\Programming\디지털신호처리\과제\DSP_12\ma.jpg")

# 그레이스케일이 아닌 RGB이면 → 한 채널만 사용
if img.ndim == 3:
    img = img[:, :, 0]

# 이미지 크기 확인
print("image shape:", img.shape)  # (height, width)

# ---------------------------------------------------------
# 2) 특정 행 선택 (40번째 행)
#    Python은 0부터 시작하므로 실제로는 row 39
# ---------------------------------------------------------
row_idx = 39
x = img[row_idx, :].astype(float)

# 값이 0~1인 경우 255배 해주면 눈에 더 잘 보임
if x.max() <= 1.0:
    x = x * 255

# ---------------------------------------------------------
# 3) 11점 이동 평균 필터 적용
# ---------------------------------------------------------
kernel = np.ones(11) / 11
y = np.convolve(x, kernel, mode="same")

# ---------------------------------------------------------
# 4) (제출 1) 입력 vs 출력
# ---------------------------------------------------------
plt.figure(figsize=(12,4))
plt.plot(x, 'b', label='Input signal (x)')
plt.plot(y, 'r', label='Filtered output (y)')
plt.title("Input vs 11-Point Moving Average Output")
plt.xlabel("Sample index")
plt.ylabel("Intensity")
plt.legend()
plt.grid()
plt.savefig("output_step1.png", dpi=300)   # 제출파일 1

# ---------------------------------------------------------
# 5) (제출 2) 5 sample delay 보정
# ---------------------------------------------------------
y_adj = np.roll(y, -5)  # 앞으로 5 이동

plt.figure(figsize=(12,4))
plt.plot(x, 'b', label='Input signal (x)')
plt.plot(y_adj, 'r', label='Output shifted -5 samples')
plt.title("Input vs Time-Adjusted Output")
plt.xlabel("Sample index")
plt.ylabel("Intensity")
plt.legend()
plt.grid()
plt.savefig("output_step2.png", dpi=300)   # 제출파일 2

plt.show()
