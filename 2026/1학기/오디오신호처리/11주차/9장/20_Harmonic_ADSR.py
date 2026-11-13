import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# =========================================
# Parameters
# =========================================
f = 440
fs = 44100
dur = 2

# 시간축
t = np.arange(0, dur, 1/fs)

# =========================================
# Pure tone
# =========================================
x = np.cos(2*np.pi*f*t)

# =========================================
# ADSR Parameters
# =========================================
attack = 0.1
decay = 0.2
sustain_level = 0.6
release = 0.5

# 샘플 수 계산
attack_n = int(attack * fs)
decay_n = int(decay * fs)
release_n = int(release * fs)

sustain_n = len(x) - attack_n - decay_n - release_n

# =========================================
# ADSR Envelope
# =========================================

# Attack
env_attack = np.linspace(0, 1, attack_n)

# Decay
env_decay = np.linspace(1, sustain_level, decay_n)

# Sustain
env_sustain = np.ones(sustain_n) * sustain_level

# Release
env_release = np.linspace(sustain_level, 0, release_n)

# Envelope 결합
env = np.concatenate([
    env_attack,
    env_decay,
    env_sustain,
    env_release
])

# =========================================
# ADSR 적용
# =========================================
y = x * env

# =========================================
# Sound Playback
# =========================================
print("Original Pure Tone")
sd.play(0.5*x, fs)
sd.wait()

print("ADSR Applied")
sd.play(0.5*y, fs)
sd.wait()

# =========================================
# ADSR Envelope Plot
# =========================================
plt.figure(figsize=(10,4))

plt.plot(t, env, linewidth=2)

plt.title('ADSR Envelope')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.grid(True)

plt.show()

# =========================================
# Time-domain comparison
# =========================================
t_plot = 0.02
N = int(fs*t_plot)

plt.figure(figsize=(10,4))

plt.plot(t[:N], x[:N],
         'k--',
         label='Original signal')

plt.plot(t[:N], y[:N],
         linewidth=1.5,
         label='ADSR applied')

plt.title('Time-domain Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.legend(loc='upper right')

plt.grid(True)

plt.show()