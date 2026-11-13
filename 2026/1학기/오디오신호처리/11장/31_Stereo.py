import numpy as np
import sounddevice as sd

fs = 44100
dur = 1

t = np.arange(0, dur, 1/fs)

left = np.sin(2*np.pi*440*t)
right = np.sin(2*np.pi*880*t)

# 스테레오 신호 생성
stereo = np.column_stack((left, right))

sd.play(stereo, fs)
sd.wait()