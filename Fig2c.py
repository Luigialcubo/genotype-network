from pathlib import Path
import matplotlib.pyplot as plt

BASE_DIR = Path(__file__).resolve().parent
RESULTS_DIR = BASE_DIR / "results"
PLOTS_DIR = BASE_DIR / "plots"
PLOTS_DIR.mkdir(exist_ok=True)

archivo_metricas = RESULTS_DIR / "metricas.txt"

metricas = {}

with open(archivo_metricas, "r", encoding="utf-8") as f:
    for linea in f:
        nombre, valor = linea.strip().split("=")
        metricas[nombre.strip()] = float(valor.strip())

orden = ["L", "Q", "C", "T", "r"]
valores = [metricas[x] for x in orden]

plt.figure()
plt.scatter(orden, valores)
plt.ylabel("Valor")
plt.title("Propiedades estructurales - H3N2")
plt.tight_layout()

salida = PLOTS_DIR / "figura_2c_metricas_H3N2.png"
plt.savefig(salida, dpi=300)
plt.close()

print("Figura 2.c guardada en:", salida)