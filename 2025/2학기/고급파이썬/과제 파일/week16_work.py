import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import SGD

#XOR 데이터
X = np.array([[0, 0],
              [0, 1],
              [1, 0],
              [1, 1]], dtype=np.float32)

y = np.array([[0],
              [1],
              [1],
              [0]], dtype=np.float32)

#학습 모델
def build_model():
    model = Sequential()
    model.add(Dense(8, activation='relu', input_dim=2))
    model.add(Dense(1, activation='sigmoid'))
    return model

model = build_model()

sgd = SGD(learning_rate=0.1)
model.compile(loss="mean_squared_error", optimizer=sgd)

#학습
model.fit(X, y, batch_size=1, epochs=10000, verbose=0)

print("학습 결과")
print(model.predict(X, verbose=0))

#가중치
weight_file = "xor_weights.h5"
model.save_weights(weight_file)
print("가중치 저장 완료")

#새 모델
new_model = build_model()
new_model.load_weights(weight_file)
new_model.compile(loss="mean_squared_error", optimizer=sgd)

#예측
print("결과")
labels = [0, 1, 1, 0]

for x, label in zip(X, labels):
    x_input = x.reshape(1, 2)
    pred = new_model.predict(x_input, verbose=0)[0, 0]
    print(f"{x.astype(int)} input : label({label}) -> {pred}")