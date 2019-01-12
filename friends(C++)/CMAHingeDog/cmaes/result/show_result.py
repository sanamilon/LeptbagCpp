import numpy as np
import matplotlib.pyplot as plt

meanf = np.loadtxt("es_result_meanf.csv", delimiter=",")
sigma = np.loadtxt("es_result_sigma.csv", delimiter=",")
D = np.loadtxt("es_result_D.csv", delimiter=",")
diagC = np.loadtxt("es_result_diagC.csv", delimiter=",")

plt.figure(figsize=(12, 4))
plt.subplot(131)
plt.semilogy(meanf, "r", label="meanf")
plt.semilogy(sigma, "b", label="sigma")
plt.title("meanf and sigma")
plt.xlim(0, meanf.size);
plt.legend()

plt.subplot(132)
plt.semilogy(D, label="D")
plt.title("D")
plt.xlim(0, meanf.size)

plt.subplot(133)
plt.semilogy(diagC, label="diagC")
plt.title("diagC")
plt.xlim(0, meanf.size)

plt.show()
