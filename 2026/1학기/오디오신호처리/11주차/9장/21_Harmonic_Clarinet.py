import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# =========================================
# Parameters
# =========================================
f = 440
fs = 44100
dur = 2

t = np.arange(0, dur, 1/fs)

# =========================================
# Clarinet-like harmonic structure
# 홀수차 고조파 중심
# =========================================
x = (
    np.cos(2*np.pi*f*t)
    + (1/3)*np.cos(2*np.pi*3*f*t)
    + (1/5)*np.cos(2*np.pi*5*f*t)
    + (1/7)*np.cos(2*np.pi*7*f*t)
)

# 약한 breath/noise 성분
noise = 0.03 * np.random.randn(len(t))
x = x + noise

# 공명감: 특정 고조파를 약간 강조한 느낌
x = (
    x
    + 0.15*np.cos(2*np.pi*3*f*t)
    + 0.08*np.cos(2*np.pi*5*f*t)
)

# 음량 정리
x = x / np.max(np.abs(x))

# =========================================
# ADSR Envelope
# 클라리넷은 피아노보다 attack이 비교적 완만한 편
# =========================================
attack = 0.15
decay = 0.2
sustain_level = 0.75
release = 0.4

attack_n = int(attack * fs)
decay_n = int(decay * fs)
release_n = int(release * fs)
sustain_n = len(x) - attack_n - decay_n - release_n

env_attack = np.linspace(0, 1, attack_n)
env_decay = np.linspace(1, sustain_level, decay_n)
env_sustain = np.ones(sustain_n) * sustain_level
env_release = np.linspace(sustain_level, 0, release_n)

env = np.concatenate([
    env_attack,
    env_decay,
    env_sustain,
    env_release
])

env = env[:len(x)]

# ADSR 적용
y = x * env

# =========================================
# Sound Playback
# =========================================
print("Clarinet-like Sound")
sd.play(0.8*y, fs)
sd.wait()

# =========================================
# Plot ADSR Envelope
# =========================================
plt.figure(figsize=(10,4))
plt.plot(t, env, linewidth=2)
plt.title('ADSR Envelope for Clarinet-like Sound')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show()

# =========================================
# Time-domain waveform
# =========================================
t_plot = 0.02
N = int(fs*t_plot)

plt.figure(figsize=(10,4))
plt.plot(t[:N], y[:N], linewidth=1.5)
plt.title('Clarinet-like Signal with ADSR')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show()

# =========================================
# Frequency Spectrum
# =========================================
Y = np.fft.fft(y)
freq = np.fft.fftfreq(len(y), 1/fs)

half = len(y)//2
freq_pos = freq[:half]
Y_mag = 2*np.abs(Y[:half]) / len(y)

idx = freq_pos <= 5000

plt.figure(figsize=(10,4))
plt.plot(freq_pos[idx], Y_mag[idx], linewidth=1.5)
plt.title('Spectrum of Clarinet-like Sound')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.grid(True)
plt.show()