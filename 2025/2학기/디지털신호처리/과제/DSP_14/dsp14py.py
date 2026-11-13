import matplotlib.pyplot as plt
plt.rc('font', family='NanumGothic')
plt.rcParams['axes.unicode_minus'] = False

import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import chirp, convolve

fs = 8000
T = 1.0
t = np.linspace(0, T, int(fs*T), endpoint=False)

# 0 ~ fs/2 = 4000 Hz 까지 올라가는 첩 신호
x = chirp(t, f0=0, f1=fs/2, t1=T, method='linear')

# 문2에서 구한 임펄스 응답
h = np.array([1, 1, 0, -1, -1])

# 컨볼루션 (필터링)
y = convolve(x, h, mode='same')

# 그래프
plt.figure(figsize=(10,6))
plt.subplot(2,1,1)
plt.plot(t, x)
plt.title('입력 첩 신호 x[n]')
plt.xlabel('Time [s]')

plt.subplot(2,1,2)
plt.plot(t, y)
plt.title('출력 신호 y[n] = x[n]*h[n]')
plt.xlabel('Time [s]')

plt.tight_layout()
plt.show()
