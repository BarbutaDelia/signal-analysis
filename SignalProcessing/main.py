import numpy as np
from scipy.signal import hilbert

from scipy.io import wavfile
samplerate, data = wavfile.read('7.wav')

wavFileInfo = open("wafeInfo.txt", "a")
wavFileInfo.write(str(samplerate)+'\n')
wavFileInfo.write(str(data.size)+'\n')
wavFileInfo.close()

#print(samplerate)
#print(data.size)
#print (data)

np.savetxt("waveData.txt", data, fmt="%2.0f")

analytic_signal = hilbert(data) #signal este vectorul aferent semnalului analizat
amplitude_envelope = np.abs(analytic_signal)

print (analytic_signal)
print (amplitude_envelope)

np.savetxt("envelopeData.txt", amplitude_envelope, fmt="%2.0f")
