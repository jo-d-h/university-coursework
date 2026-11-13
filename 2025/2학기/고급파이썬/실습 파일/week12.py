#14장_2 PPT p.28(MSE 오차 계산하기)
import numpy as np
def MSE(target, y):
    n = y.shape[0] #데이터 갯수
    ret_mse = np.sum((y - target) ** 2) / n
    #ret_mse = np.sum(np.square(ypred - y) / n)
    return ret_mse

y = np.array([0, 1, 0, 0, 0]) #정답
ypred = np.array([1, 0, 0, 0, 0]) #예측값 1

mse = MSE(ypred, y)
print(mse)

ypred = np.array([0, 1, 0, 0, 0]) #예측값 2
mse = MSE(ypred, y)
print(mse)

