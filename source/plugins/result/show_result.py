import numpy as np
import matplotlib.pyplot as plt

topf = np.loadtxt("es_result_topf.csv", delimiter=",")
meanf = np.loadtxt("es_result_meanf.csv", delimiter=",")
sigmaN = np.loadtxt("es_result_sigmaN.csv", delimiter=",")
D = np.loadtxt("es_result_D.csv", delimiter=",")
diagC = np.loadtxt("es_result_diagC.csv", delimiter=",")

plt.figure(figsize=(9, 6))


plt.subplot(231)
topx = np.arange(0, topf.size, 1)
plt.plot(topx, topf, "r", label="topf", linewidth=0.5)
plt.title("topf")
plt.xlim(0, topf.size)

plt.subplot(232)
meanx = np.arange(0, meanf.size, 1)
plt.plot(meanx, meanf, "r", label="meanf", linewidth=0.5)
plt.title("meanf")
plt.xlim(0, meanf.size)

plt.subplot(233)
meanx = np.arange(0, meanf.size, 1)
plt.semilogy(-1.0*meanf, "r", label="semilogy(-meanf)", linewidth=0.5)
plt.title("semilogy(-meanf)")
plt.xlim(0, meanf.size)

plt.subplot(234)
plt.semilogy(sigmaN, "b", label="sigmaN", linewidth=1)
plt.title("sigmaN")
plt.xlim(0, meanf.size);

plt.subplot(235)
plt.semilogy(D, label="D", linewidth=0.5)
plt.title("D")
plt.xlim(0, meanf.size)

plt.subplot(236)
plt.semilogy(diagC, label="diagC", linewidth=0.5)
plt.title("diagC")
plt.xlim(0, meanf.size)

plt.show()
