import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib.colors as colors

data = np.loadtxt("results/fig1_2/datos_figura1_2a.txt", skiprows=1)

R0 = data[:,0]
I = data[:,1:]

etas = np.array([1e-4,1e-3,1e-2,1e-1,1e0,1e1,1e2,1e3,1e4])

fig, ax = plt.subplots(figsize=(7,4))

norm = colors.LogNorm(vmin=1e-4, vmax=1e4)
cmap = cm.get_cmap("coolwarm")

for k, eta in enumerate(etas):
    ax.plot(
        R0,
        I[:,k],
        lw=2.5,
        color=cmap(norm(eta))
    )

ax.set_xlim(0,8)
ax.set_ylim(0,1)

ax.set_xlabel(r"$R_0$", fontsize=24)
ax.set_ylabel(r"$I^\star$", fontsize=24, rotation=0, labelpad=20)

# barra lateral
sm = cm.ScalarMappable(norm=norm, cmap=cmap)
cbar = plt.colorbar(sm, ax=ax)
cbar.set_label(r"$\bar{\eta}$", fontsize=22)

plt.tight_layout()
plt.savefig("plots/figura_1_2_a.png", dpi=300)
