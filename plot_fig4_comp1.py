"""
Ejecutar: py plot_fig4_comp1.py
Genera la gráfica de la componente 1 de la red de Influenza A (Figura 4 del artículo):
  - Arriba: prevalencia absoluta de cada comunidad (área apilada).
  - Abajo: prevalencia relativa por comunidad (área apilada, normalizada a 1).
Los colores coinciden con los del grafo de comunidades (plot_fig4_comp1_graph.py).
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

# ============================================================
# 1. CONFIGURACIÓN DE RUTAS
# ============================================================
ARCHIVO_DATOS     = "results/fig4/sims_comp1.txt"
ARCHIVO_PARTICION = "results/fig4/particion_comp1.txt"
CARPETA_SALIDA    = "plots/fig4"
os.makedirs(CARPETA_SALIDA, exist_ok=True)

# ============================================================
# 2. LECTURA DE LA PARTICIÓN Y MAPEO COLUMNAS -> COMUNIDAD ORIGINAL
# ============================================================
# Leemos la red solo para saber N (número de nodos)
with open("componentes_conexas/componente_1.txt", 'r') as f:
    N = int(f.readline().strip())

# Leer partición original (nodo -> id_original)
comm_id = np.full(N, -1, dtype=int)
with open(ARCHIVO_PARTICION, 'r') as f:
    for line in f:
        if not line.strip():
            continue
        nodo, com = map(int, line.split()[:2])
        if 0 <= nodo < N:
            comm_id[nodo] = com

# Reproducir el mapeo del código C: índices compactos según orden de aparición
map_orig_to_compact = {}
compact_index = 0
for i in range(N):
    cid = comm_id[i]
    if cid != -1 and cid not in map_orig_to_compact:
        map_orig_to_compact[cid] = compact_index
        compact_index += 1

K = compact_index                      # número de comunidades (7)
# Mapeo inverso: índice compacto -> comunidad original
compact_to_orig = {v: k for k, v in map_orig_to_compact.items()}

print("Mapeo comunidad original -> columna en archivo:")
for orig, comp in sorted(map_orig_to_compact.items()):
    print(f"  Comunidad {orig} -> columna {comp+1} (índice {comp})")

# ============================================================
# 3. CARGA DE DATOS DE SIMULACIÓN
# ============================================================
df = pd.read_csv(ARCHIVO_DATOS, sep=r'\s+', header=None)
t = df[0].values                      # tiempo
# Datos en el orden de columnas del archivo (0..K-1)
datos_por_compact = df.iloc[:, 1:K+1].values  # matriz (tiempos x K)

# ============================================================
# 4. REORDENAR PARA QUE LAS COLUMNAS VAYAN POR COMUNIDAD ORIGINAL (1..7)
# ============================================================
# Crear matriz (tiempos x 7) con NaN en las comunidades no presentes
num_com_original = max(map_orig_to_compact.keys())  # debería ser 7
datos_por_orig = np.full((len(t), num_com_original), np.nan)
for orig, comp in map_orig_to_compact.items():
    datos_por_orig[:, orig-1] = datos_por_compact[:, comp]  # orig-1 para índice 0

# Quedarnos solo con las comunidades del 1 al 7 (todas)
# y llenar NaN con 0 por si alguna falta (no debería)
datos_por_orig = np.nan_to_num(datos_por_orig)

# ============================================================
# 5. PALETA DE COLORES (idéntica al script del grafo)
# ============================================================
colores_base = ['#789e97', '#2d6280', '#202642', '#6b2a31', '#c93c2c', '#d76840', '#e4a166']
# Las comunidades originales 1..7 en orden numérico
etiquetas = ['Comunidad 1','Comunidad 2','Comunidad 3','Comunidad 4','Comunidad 5','Comunidad 6','Comunidad 7']
colores = colores_base[:7]   # 7 colores

# ============================================================
# 6. CÁLCULO DE PREVALENCIAS RELATIVAS (por fila)
# ============================================================
total = datos_por_orig.sum(axis=1)
relativa = datos_por_orig / total[:, None]   # cada fila suma 1

# ============================================================
# 7. CREACIÓN DE LA FIGURA
# ============================================================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True,
                               gridspec_kw={'height_ratios': [3, 1.5]})

# ---------- PANEL SUPERIOR: prevalencia absoluta ----------
for i in range(7):
    ax1.plot(t, datos_por_orig[:, i], color=colores[i], linewidth=1.5, label=etiquetas[i])
    ax1.fill_between(t, datos_por_orig[:, i], alpha=0.3, color=colores[i])

ax1.set_ylabel('I(t)', fontsize=16)
ax1.set_title('COMPONENTE 1 INFLUENZA A', fontsize=20, fontweight='bold')
ax1.set_xlim(0, 900)
ax1.set_ylim(0, None)   # ajuste automático
ax1.legend(loc='upper right', fontsize='large')
ax1.grid(True, linestyle='--', alpha=0.5)

# ---------- PANEL INFERIOR: prevalencia relativa apilada ----------
ax2.stackplot(t, *[relativa[:, i] for i in range(7)],
              labels=etiquetas, colors=colores, alpha=0.7)
ax2.set_xlabel('t (días)', fontsize=16)
ax2.set_ylabel(r"$I(t)^{rel}$", fontsize=16)
ax2.set_ylim(0, 1)
ax2.set_xlim(0, 900)
ax2.legend(loc='lower left', fontsize='large')
ax2.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout()
plt.savefig(os.path.join(CARPETA_SALIDA, 'fig4_comp1.png'), dpi=300)
print(f'Figura guardada en {os.path.join(CARPETA_SALIDA, "fig4_comp1.png")}')
plt.show()