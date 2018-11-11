import numpy as np
import matplotlib.pyplot as plt

meanf = np.loadtxt("es_result_meanf.csv", delimiter=",")
sigmaN = np.loadtxt("es_result_sigma.csv", delimiter=",")
D = np.loadtxt("es_result_D.csv", delimiter=",")
diagC = np.loadtxt("es_result_diagC.csv", delimiter=",")

plt.figure(figsize=(12, 4))


plt.subplot(141)
meanx = np.arange(0, meanf.size, 1)
plt.semilogy(meanf, "r", label="meanf", linewidth=1)
plt.xlabel("iter");
plt.title("meanf")
plt.xlim(0, meanf.size)

plt.subplot(142)
plt.semilogy(sigmaN, "b", label="sigmaN", linewidth=1)
plt.xlabel("iter");
plt.title("sigma")
plt.xlim(0, meanf.size);

plt.subplot(143)
plt.semilogy(D, label="D", linewidth=0.5)
plt.xlabel("iter");
plt.title("D")
plt.xlim(0, meanf.size)

plt.subplot(144)
plt.semilogy(diagC, label="diagC", linewidth=0.5)
plt.xlabel("iter");
plt.title("diagC")
plt.xlim(0, meanf.size)

plt.show()
