import numpy as np
import matplotlib.pyplot as plt

sis = np.loadtxt("results/fig1_1/datos_sis.txt", skiprows=1)
sir = np.loadtxt("results/fig1_1/datos_sir.txt", skiprows=1)
sirs = np.loadtxt("results/fig1_1/datos_sirs.txt", skiprows=1)

plt.figure(figsize=(8,5))

plt.plot(sis[:,0], sis[:,1], lw=3, label="SIS")
plt.plot(sir[:,0], sir[:,1], lw=3, label="SIR")
plt.plot(sirs[:,0], sirs[:,1], lw=3, label="SIRS")

plt.xlabel("t")
plt.ylabel("I(t)")
plt.xlim(0, 200)
plt.ylim(0, 0.7) 
plt.title("Dinámica temporal del modelo SIMS para una sola cepa", fontsize=14)
plt.legend()
plt.tight_layout()
plt.savefig(
    "plots/fig1_1/fig1_1_a.png",
    dpi=300,
    bbox_inches="tight"
)