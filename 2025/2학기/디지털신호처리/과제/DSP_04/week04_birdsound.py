import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import chirp, spectrogram
from scipy.io.wavfile import write
import sounddevice as sd
import os

#설정
fs = 8000                 #샘플링 주파수
tone_duration = 0.1       #지속 시간
silence_duration = 0.1    #휴식 시간
f_start = 3500            #시작 주파수
f_end = 1000              #종료 주파수
n_repeats = 4             #반복

#시간 벡터
t_tone = np.linspace(0, tone_duration, int(fs*tone_duration), endpoint=False)
t_silence = np.linspace(0, silence_duration, int(fs*silence_duration), endpoint=False)

#버스트 신호 생성
tone = chirp(t_tone, f0=f_start, f1=f_end, t1=tone_duration, method='linear')
silence = np.zeros_like(t_silence)
unit_signal = np.concatenate([tone, silence])

# 전체 신호 반복
signal = np.tile(unit_signal, n_repeats)

# 정규화
signal = signal / np.max(np.abs(signal))

#스펙트로그램 
f, t_spec, Sxx = spectrogram(signal, fs, nperseg=128, noverlap=100)
plt.figure(figsize=(8,4))
plt.pcolormesh(t_spec, f, 10*np.log10(Sxx+1e-12), shading='gouraud')
plt.ylabel('Frequency [Hz]')
plt.xlabel('Time [sec]')
plt.title("새소리 스펙트로그램")
plt.ylim([0, 4000])
plt.colorbar(label='dB')
plt.show()

#소리 재생
sd.play(signal, fs)
sd.wait()

#WAV 저장
script_dir = os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else os.getcwd()
file_path = os.path.join(script_dir, "bird_sound.wav")
write(file_path, fs, np.int16(signal*32767))
print(f"{file_path} 파일이 저장되었습니다.")
