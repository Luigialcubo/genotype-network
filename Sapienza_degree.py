print("EJECUTANDO SCRIPT")
from pathlib import Path
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

BASE_DIR = Path(__file__).resolve().parent
DATA_FILE = BASE_DIR / "BWetal2022_2LCC.edge"
PLOTS_DIR = BASE_DIR / "plots"
RESULTS_DIR = BASE_DIR / "results"

PLOTS_DIR.mkdir(exist_ok=True)
RESULTS_DIR.mkdir(exist_ok=True)

print("Carpeta del script:", BASE_DIR)
print("Archivo de red:", DATA_FILE)
print("Existe archivo:", DATA_FILE.exists())

G = nx.read_edgelist(DATA_FILE, nodetype=int, data=False)

print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())
print("¿Es conexa?", nx.is_connected(G))

grados = [d for _, d in G.degree()]
valores, cuentas = np.unique(grados, return_counts=True)
prob = cuentas / cuentas.sum()

plt.figure()
plt.scatter(valores, prob)
plt.xlabel("Grado k")
plt.ylabel("P(k)")
plt.title("Distribución de grados - H3N2")
plt.yscale("log")
plt.xscale("log")
plt.tight_layout()

ruta_fig = PLOTS_DIR / "figura_2b_distribucion_grado.png"
plt.savefig(ruta_fig, dpi=300)
plt.close()

print("Figura guardada en:", ruta_fig)

ruta_txt = RESULTS_DIR / "distribucion_grado.txt"

with open(ruta_txt, "w", encoding="utf-8") as f:
    f.write("k\tP(k)\n")
    for k, p in zip(valores, prob):
        f.write(f"{k}\t{p}\n")

print("Datos guardados en:", ruta_txt)