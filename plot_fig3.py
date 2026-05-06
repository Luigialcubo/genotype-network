"""
plot_fig3.py
Reproduce la Figura 3 del artículo en tres archivos independientes:
  fig3a.png → Topología de las tres redes sintéticas.
  fig3b.png → Prevalencia I(t) con inmunidad duradera (gamma = 0).
  fig3c.png → Prevalencia I(t) con inmunidad temporal (alpha = gamma = 0.03).

Utiliza los datos generados por main_sims_general.c y las redes originales.
"""

import numpy as np
import matplotlib.pyplot as plt
import networkx as nx
import os

# ============================================================
# 0. CONFIGURACIÓN DE RUTAS
# ============================================================
# Redes
RED_LATTICE = "results/redes_txt/reticula_10x10.txt"
RED_HOMOG   = "results/redes_txt/red_homogenea.txt"
RED_HETEROG = "results/redes_txt/red_heterogenea.txt"

# Simulaciones gamma = 0
SIM_LATTICE_G0 = "results/fig3/sims_reticula_gamma_0.txt"
SIM_HOM_G0     = "results/fig3/sims_hom_gamma_0.txt"
SIM_HETER_G0   = "results/fig3/sims_het_gamma_0.txt"

# Simulaciones gamma = 0.03
SIM_LATTICE_G003 = "results/fig3/sims_reticula_gamma_003.txt"
SIM_HOM_G003     = "results/fig3/sims_hom_gamma_003.txt"
SIM_HETER_G003   = "results/fig3/sims_het_gamma_003.txt"

# Carpeta de salida
OUT_DIR = "plots/fig3"

# ============================================================
# 1. FUNCIONES AUXILIARES
# ============================================================
def cargar_red(archivo):
    """
    Lee una red desde un archivo con formato:
        primera línea: N (número de nodos)
        líneas siguientes: i j (aristas)
    Retorna un grafo de NetworkX.
    """
    with open(archivo, 'r') as f:
        lineas = f.read().strip().splitlines()
    n = int(lineas[0])
    G = nx.Graph()
    G.add_nodes_from(range(n))
    for linea in lineas[1:]:
        if not linea.strip():
            continue
        u, v = map(int, linea.split())
        G.add_edge(u, v)
    return G

def cargar_serie(archivo):
    """
    Carga un archivo de dos columnas (tiempo, I(t)) sin cabecera.
    Devuelve arrays t, I.
    """
    datos = np.loadtxt(archivo)
    return datos[:, 0], datos[:, 1]

# ============================================================
# 2. CARGA DE DATOS (común para todos los paneles)
# ============================================================
G_lattice = cargar_red(RED_LATTICE)
G_hom     = cargar_red(RED_HOMOG)
G_heter   = cargar_red(RED_HETEROG)

#t_lat_g0, I_lat_g0 = cargar_serie(SIM_LATTICE_G0)
#t_hom_g0, I_hom_g0 = cargar_serie(SIM_HOM_G0)
#t_het_g0, I_het_g0 = cargar_serie(SIM_HETER_G0)

#t_lat_g03, I_lat_g03 = cargar_serie(SIM_LATTICE_G003)
#t_hom_g03, I_hom_g03 = cargar_serie(SIM_HOM_G003)
#t_het_g03, I_het_g03 = cargar_serie(SIM_HETER_G003)

# Crear carpeta de salida si no existe
os.makedirs(OUT_DIR, exist_ok=True)

# Colores
COLOR_LATTICE = 'turquoise'
COLOR_HOMOG   = 'blue'
COLOR_HETEROG = 'orange'

# ============================================================
# 3. PANEL (a): TOPOLOGÍA DE LAS REDES
# ============================================================
print("Generando Figura 3(a)...")
fig_a, axes_a = plt.subplots(1, 3, figsize=(14, 4))

# Lattice
pos = nx.spring_layout(G_lattice, seed=42)
nx.draw_networkx_nodes(G_lattice, pos, node_size=20, node_color=COLOR_LATTICE, ax=axes_a[0])
nx.draw_networkx_edges(G_lattice, pos, width=0.5, ax=axes_a[0])
axes_a[0].set_title('Red lattice')
axes_a[0].axis('off')

# Homogéneas
pos = nx.spring_layout(G_hom, seed=42)
nx.draw_networkx_nodes(G_hom, pos, node_size=20, node_color=COLOR_HOMOG, ax=axes_a[1])
nx.draw_networkx_edges(G_hom, pos, width=0.5, ax=axes_a[1])
axes_a[1].set_title('Estrellas homogéneas')
axes_a[1].axis('off')

# Heterogéneas
pos = nx.spring_layout(G_heter, seed=42)
nx.draw_networkx_nodes(G_heter, pos, node_size=20, node_color=COLOR_HETEROG, ax=axes_a[2])
nx.draw_networkx_edges(G_heter, pos, width=0.5, ax=axes_a[2])
axes_a[2].set_title('Estrellas heterogéneas')
axes_a[2].axis('off')

plt.suptitle('(a) Redes sintéticas de genotipos', fontsize=14)
plt.tight_layout()
plt.savefig(os.path.join(OUT_DIR, 'fig3a.png'), dpi=300)
plt.close(fig_a)
print(" -> Guardada en", os.path.join(OUT_DIR, 'fig3a.png'))
"""
# ============================================================
# 4. PANEL (b): INMUNIDAD DURADERA (gamma = 0)
# ============================================================
print("Generando Figura 3(b)...")
fig_b, ax_b = plt.subplots(figsize=(10, 5))

ax_b.plot(t_lat_g0, I_lat_g0, color=COLOR_LATTICE, lw=2, label='Lattice')
ax_b.plot(t_hom_g0, I_hom_g0, color=COLOR_HOMOG,   lw=2, label='Estrellas homog.')
ax_b.plot(t_het_g0, I_het_g0, color=COLOR_HETEROG, lw=2, label='Estrellas heterog.')

ax_b.set_xlabel('Tiempo (días)')
ax_b.set_ylabel('Prevalencia $I(t)$')
ax_b.set_title('(b) Inmunidad duradera ($\\gamma=0$)')
ax_b.legend(frameon=False)
ax_b.grid(alpha=0.3)

plt.tight_layout()
plt.savefig(os.path.join(OUT_DIR, 'fig3b.png'), dpi=300)
plt.close(fig_b)
print(" -> Guardada en", os.path.join(OUT_DIR, 'fig3b.png'))

# ============================================================
# 5. PANEL (c): INMUNIDAD TEMPORAL (gamma = 0.03)
# ============================================================
print("Generando Figura 3(c)...")
fig_c, ax_c = plt.subplots(figsize=(10, 5))

ax_c.plot(t_lat_g03, I_lat_g03, color=COLOR_LATTICE, lw=2, label='Lattice')
ax_c.plot(t_hom_g03, I_hom_g03, color=COLOR_HOMOG,   lw=2, label='Estrellas homog.')
ax_c.plot(t_het_g03, I_het_g03, color=COLOR_HETEROG, lw=2, label='Estrellas heterog.')

ax_c.set_xlabel('Tiempo (días)')
ax_c.set_ylabel('Prevalencia $I(t)$')
ax_c.set_title('(c) Inmunidad temporal ($\\alpha=\\gamma=0.03$)')
ax_c.legend(frameon=False)
ax_c.grid(alpha=0.3)

plt.tight_layout()
plt.savefig(os.path.join(OUT_DIR, 'fig3c.png'), dpi=300)
plt.close(fig_c)
print(" -> Guardada en", os.path.join(OUT_DIR, 'fig3c.png'))

print("\n¡Las tres figuras se han generado correctamente!")
"""