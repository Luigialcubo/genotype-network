from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np
import os

BASE_DIR = Path(__file__).resolve().parent
RESULTS_DIR = BASE_DIR / "results"/"H3N2_txt"
PLOTS_DIR = BASE_DIR / "plots"/"H3N2"
PLOTS_DIR.mkdir(exist_ok=True)

archivo_metricas = RESULTS_DIR / "metricas.txt"

metricas = {}

with open(archivo_metricas, "r", encoding="utf-8") as f:
    for linea in f:
        nombre, valor = linea.strip().split("=")
        metricas[nombre.strip()] = float(valor.strip())

orden = ["L", "r", "C", "T", "Q"]
valores = [metricas[x] for x in orden]


valores = [metricas[x] for x in orden]
nombres = ["Longitud media","Asortatividad","Clustering medio","Transitividad","Modularidad"]
y = np.arange(len(valores))[::-1]

rangos = {
    "Longitud media": (3, 7),
    "Asortatividad": (-0.5, 0.0),
    "Clustering medio": (0.0, 0.15),
    "Transitividad": (0.00, 0.06),
    "Modularidad": (0.0, 0.9),
}

fig, ax = plt.subplots(figsize=(9, 5))

for yi, nombre, valor in zip(y, nombres, valores):

    xmin, xmax = rangos[nombre]

    # posición normalizada del punto
    x_norm = (valor - xmin) / (xmax - xmin)

    # línea visual siempre de 0 a 1
    ax.hlines(yi, 0, 1, color="lightgray", lw=2, zorder=1)

    punto = ax.scatter(
        x_norm, yi,
        s=120,
        color="#1f6f8b",
        edgecolor="white",
        linewidth=1.5,
        zorder=3
    )

    # valores inicial y final reales de cada métrica
    ax.text(0, yi - 0.22, f"{xmin:g}", ha="center", va="top", fontsize=10, color="gray")
    ax.text(1, yi - 0.22, f"{xmax:g}", ha="center", va="top", fontsize=10, color="gray")

ax.set_yticks(y)
ax.set_yticklabels(nombres, fontsize=13)

ax.set_title("Propiedades H3N2", fontsize=18, fontweight="bold")

ax.set_xlim(-0.08, 1.08)
ax.set_xticks([])

for spine in ax.spines.values():
    spine.set_visible(False)

ax.tick_params(axis="y", length=0)

plt.tight_layout()

os.makedirs(PLOTS_DIR, exist_ok=True)
fig.savefig(PLOTS_DIR/"figura_2c_metricas_H3N2.png", dpi=300, bbox_inches="tight")
