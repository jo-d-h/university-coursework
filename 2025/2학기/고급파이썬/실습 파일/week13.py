#16_1 PPT p.15 손실함수
import numpy as np

# 중요!
def MSE(y, H):
    return np.sum((y-H)**2) / y.shape[0]

x = np.array([1.,2.,3.])
y = np.array([1.,2.,3.])

W, b = 0, 5
H = W * x + b
e = MSE(y, H)
print(e)

W, b = 1/2, 0
H = W * x + b
e = MSE(y, H)
print(e)

W, b = 1.1, 0.5
H = W * x + b
e = MSE(y, H)
print(e)

W, b = 1, 0
H = W * x + b
e = MSE(y, H)
print(e)