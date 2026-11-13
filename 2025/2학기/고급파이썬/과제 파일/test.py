import tensorflow as tf
import numpy as np

# XOR 데이터
X_test = np.array([[0,0],
                   [0,1],
                   [1,0],
                   [1,1]])

y_label = [0, 1, 1, 0]   # 정답 라벨

# 학습과 동일한 모델 구조
model = tf.keras.models.Sequential()
model.add(tf.keras.layers.Dense(units=8, input_dim=2, activation='relu'))
model.add(tf.keras.layers.Dense(units=1, activation='sigmoid'))

# 가중치 로드
model.load_weights("xor_weights.h5")

# 컴파일 (MSE는 그대로)
sgd = tf.keras.optimizers.SGD(learning_rate=0.1)
model.compile(loss='mean_squared_error', optimizer=sgd)

# 테스트 : 1개씩 입력 후 출력
print("=== Test Output (1개씩 예측) ===")
for i in range(4):
    x = X_test[i].reshape(1,2)
    pred = model.predict(x)[0][0]
    print(f"{X_test[i]} input : label({y_label[i]}) -> {pred}")
