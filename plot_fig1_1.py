import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import art3d
from matplotlib.colors import LinearSegmentedColormap

##Plot de la figura 1.1.a
sis = np.loadtxt("results/fig1_1/datos_sis.txt", skiprows=1)
sir = np.loadtxt("results/fig1_1/datos_sir.txt", skiprows=1)
sirs = np.loadtxt("results/fig1_1/datos_sirs.txt", skiprows=1)

plt.figure(figsize=(8,5))

I_sis_star  = sis[-1,1]
I_sir_star  = sir[-1,1]
I_sirs_star = sirs[-1,1]


# texto
plt.text(
    sis[-1,0]-30,
    I_sis_star-0.04,
    rf"$I^*={I_sis_star:.3f}$",
    fontsize=11
)

plt.text(
    sir[-1,0]-30,
    I_sir_star+0.015,
    rf"$I^*={I_sir_star:.3f}$",
    fontsize=11
)

plt.text(
    sirs[-1,0]-30,
    I_sirs_star+0.015,
    rf"$I^*={I_sirs_star:.3f}$",
    fontsize=11
)

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

## Plot de la figura 1.1.b
data = np.loadtxt("results/fig1_1/datos_simsb.txt", skiprows=1)

t = data[:, 0]
rho = data[:, 1:]   # columnas de cepas

n = rho.shape[1]

fig = plt.figure(figsize=(11, 10))
ax = fig.add_subplot(111, projection="3d")

# degradado
cmap = LinearSegmentedColormap.from_list(
    "mi_gradiente",
    ["orange", "red", "purple", "blue", "green"],
    N=n
)

for i in range(n):
    ax.plot(
        t,
        np.full_like(t, i),
        rho[:, i],
        lw=2.5,
        color=cmap(i)
    )

ax.set_xlabel(r"$t$", fontsize=18)
ax.set_ylabel(r"$i$", fontsize=18)
ax.set_zlabel(r"$\rho_i^I$", fontsize=18)

ax.set_yticks([0, 10, 19])
ax.set_yticklabels(["0", "10", "20"])

ax.set_xlim(0, 1000)
ax.set_ylim(0, n - 1)
ax.set_zlim(0, 0.6)

ax.view_init(elev=20, azim=-65)

plt.title("Dinámica temporal del modelo SIMS para cada cepa sin considerar inmunidad cruzada", fontsize=18)

plt.tight_layout()
plt.savefig("plots/fig1_1/fig1_1_b.png", dpi=300, bbox_inches="tight")


## Plot de la figura 1.1.c
data = np.loadtxt("results/fig1_1/datos_simsc.txt", skiprows=1)

t = data[:, 0]
rho = data[:, 1:]   # columnas de cepas

n = rho.shape[1]

fig = plt.figure(figsize=(11, 10))
ax = fig.add_subplot(111, projection="3d")

# degradado
cmap = LinearSegmentedColormap.from_list(
    "mi_gradiente",
    ["orange", "red", "purple", "blue", "green"],
    N=n
)

for i in range(n):
    ax.plot(
        t,
        np.full_like(t, i),
        rho[:, i],
        lw=2.5,
        color=cmap(i)
    )

ax.set_xlabel(r"$t$", fontsize=18)
ax.set_ylabel(r"$i$", fontsize=18)
ax.set_zlabel(r"$\rho_i^I$", fontsize=18)

ax.set_yticks([0, 10, 19])
ax.set_yticklabels(["0", "10", "20"])

ax.set_xlim(0, 1000)
ax.set_ylim(0, n - 1)
ax.set_zlim(0, 0.6)

ax.view_init(elev=20, azim=-65)

plt.title("Dinámica temporal del modelo SIMS para cada cepa sin considerar inmunidad cruzada", fontsize=18)

plt.tight_layout()
plt.savefig("plots/fig1_1/fig1_1_c.png", dpi=300, bbox_inches="tight")

