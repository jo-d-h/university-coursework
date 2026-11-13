import tensorflow as tf
import numpy as np

# XOR 학습 데이터 (4,2)
X = np.array([[0,0],
              [0,1],
              [1,0],
              [1,1]])
# 정답
y = np.array([[0],
              [1],
              [1],
              [0]])

# 모델 정의
model = tf.keras.models.Sequential()
model.add(tf.keras.layers.Dense(units=8, input_dim=2, activation='relu'))
model.add(tf.keras.layers.Dense(units=1, activation='sigmoid'))

# 학습 설정
sgd = tf.keras.optimizers.SGD(learning_rate=0.1)
model.compile(loss='mean_squared_error', optimizer=sgd)

# 학습
model.fit(X, y, batch_size=1, epochs=10000, verbose=0)

# 학습 결과 출력
print("=== Training Output (4개 한번에 예측) ===")
print(model.predict(X))

# 가중치 저장
model.save_weights("xor_weights.h5")
print("가중치 저장 완료 : xor_weights.h5")
