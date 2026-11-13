import matplotlib.pyplot as plt
import numpy as np
import numpy.fft as fp
from skimage.io import imread

def plot_image(image, title=''):
    plt.imshow(image)
    plt.axis('off')
    plt.title(title, size=20)

srcImg = np.mean(imread('bricks.jpg'), axis=2)

freqDomain = fp.fft2(srcImg)
freqFilter = fp.fftshift(freqDomain)
freqTmp = np.copy(freqFilter)

(w, h) = freqDomain.shape
hW, hH = int(w/2), int(h/2)

# HPF
freqFilter[hW-20:hW+20, hH-20:hH+20] = 0

# LPF
#freqTmp[hW-20:hW+20, hH-20:hH+20] = 0
#freqFilter -= freqTmp

resImg = np.clip(fp.ifft2(fp.ifftshift(freqFilter)).real, 0, 255)

plt.figure(figsize=(20,15))
plt.gray()

plt.subplot(2,2,1), plot_image(srcImg, 'Original Image')
plt.subplot(2,2,2), plot_image(20*np.log10(0.1 + np.abs(freqDomain)), 'Fourier Spectrum')
plt.subplot(2,2,3), plot_image(20*np.log10(0.1 + np.abs(freqFilter)), 'Filtered Spectrum')
plt.subplot(2,2,4), plot_image(resImg, 'HPF Result')

plt.show()