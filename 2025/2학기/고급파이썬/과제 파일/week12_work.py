import numpy as np
import math

def distance(x1, y1, x2, y2):
    return math.sqrt((x1 - x2)**2 + (y1 - y2)**2)

arr = np.zeros((20, 20), dtype=int)

center_x, center_y = 9, 9

for i in range(20):
    for j in range(20):
        d = distance(i, j, center_x, center_y)
        if 3 < d < 5:
            arr[i, j] = 1

for row in arr:
    print(*row, sep=", ")
