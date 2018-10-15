import numpy as np
import matplotlib.pyplot as plt

meanf = np.loadtxt("es_result_meanf.csv", delimiter=",")
sigmaN = np.loadtxt("es_result_sigmaN.csv", delimiter=",")
D = np.loadtxt("es_result_D.csv", delimiter=",")
diagC = np.loadtxt("es_result_diagC.csv", delimiter=",")

plt.figure(figsize=(16, 4))

plt.subplot(141)
meanx = np.arange(0, meanf.size, 1)
#plt.plot(meanx, meanf, "r", label="meanf")
plt.semilogy(-1.0*meanf, "r", label="semilogy(-meanf)")
plt.title("meanf")
plt.xlim(0, meanf.size)

plt.subplot(142)
plt.semilogy(sigmaN, "b", label="sigmaN")
plt.title("sigmaN")
plt.xlim(0, meanf.size);

plt.subplot(143)
plt.semilogy(D, label="D")
plt.title("D")
plt.xlim(0, meanf.size)

plt.subplot(144)
plt.semilogy(diagC, label="diagC")
plt.title("diagC")
plt.xlim(0, meanf.size)

plt.show()
