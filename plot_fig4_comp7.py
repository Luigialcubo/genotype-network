"""
Ejecutar: py plot_fig4_comp7.py
Genera la gráfica de la componente 7 de la red de Influenza A (Figura 4 del artículo):
  - Arriba: prevalencia absoluta de cada comunidad (área apilada).
  - Abajo: prevalencia relativa por comunidad (área apilada, normalizada a 1).

Lee los datos de results/fig4/sims_comp7.txt.
Guarda la figura en plots/fig4/fig4_comp7.png
"""

import numpy as np
import matplotlib.pyplot as plt
import os

# ============================================================
# 1. CONFIGURACIÓN DE RUTAS
# ============================================================
ARCHIVO_DATOS = "results/fig4/sims_comp7.txt"
CARPETA_SALIDA = "plots/fig4"
os.makedirs(CARPETA_SALIDA, exist_ok=True)

# ============================================================
# 2. CARGA DE DATOS
# ============================================================
datos = np.loadtxt(ARCHIVO_DATOS)
t = datos[:, 0]                  # columna de tiempo
I_comunidades = datos[:, 2:]     # 4 columnas de comunidades (ya no hay I_total separada)
K = I_comunidades.shape[1]       # número de comunidades (4)

# Prevalencia total como suma de las comunidades
I_total = np.sum(I_comunidades, axis=1)

# Prevalencias relativas (evitar división por cero)
I_relativa = np.zeros_like(I_comunidades)
mascara = I_total > 1e-12
for i in range(K):
    I_relativa[mascara, i] = I_comunidades[mascara, i] / I_total[mascara]

# ============================================================
# 3. COLORES
# ============================================================
colores = plt.cm.tab10(np.linspace(0, 1, max(K, 10)))[:K]

# ============================================================
# 4. CREACIÓN DE LA FIGURA
# ============================================================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8), sharex=True)

# ---------- PANEL SUPERIOR: Prevalencia absoluta (área apilada) ----------
ax1.stackplot(t, *I_comunidades.T, colors=colores, alpha=0.8)
ax1.set_ylabel('I(t) abs')
ax1.set_title('Componente 7')
ax1.set_xlim(0, 800.0)
ax1.grid(alpha=0.3)

# ---------- PANEL INFERIOR: Prevalencia relativa (área apilada normalizada) ----------
ax2.stackplot(t, *I_relativa.T, colors=colores, alpha=0.8)
ax2.set_xlabel('t (días)')
ax2.set_ylabel('I(t) rel')
ax2.set_xlim(0, 800.0)
ax2.set_ylim(0, 1.0)
ax2.grid(alpha=0.3)

# Leyenda común
from matplotlib.patches import Patch
leyenda = [Patch(facecolor=colores[i], label=f'Comunidad {i+1}') for i in range(K)]
fig.legend(handles=leyenda, loc='upper right', bbox_to_anchor=(0.98, 0.95))

plt.tight_layout()
plt.savefig(os.path.join(CARPETA_SALIDA, 'fig4_comp7.png'), dpi=300)
plt.show()
print(f"Figura guardada en {CARPETA_SALIDA}/fig4_comp7.png")