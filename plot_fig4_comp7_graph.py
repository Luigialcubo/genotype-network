"""
Ejecutar: py plot_fig4_comp7_graph.py
Carga la red de la componente 7 y su partición en comunidades,
y dibuja el grafo con los nodos coloreados según la comunidad.
Guarda la figura en plots/fig4/fig4_comp7_graph.png
"""

import networkx as nx
import matplotlib.pyplot as plt
import os
import numpy as np

# ============================================================
# 1. RUTAS DE ARCHIVOS
# ============================================================
ARCHIVO_RED       = "componentes_conexas/componente_7.txt"
ARCHIVO_PARTICION = "results/fig4/particion_comp7.txt"
CARPETA_SALIDA    = "plots/fig4"
os.makedirs(CARPETA_SALIDA, exist_ok=True)

# ============================================================
# 2. FUNCIONES DE LECTURA
# ============================================================
def leer_red(archivo):
    """Lee la red con formato: primera línea N, luego aristas i j."""
    G = nx.Graph()
    with open(archivo, 'r') as f:
        lineas = f.read().strip().splitlines()
    n = int(lineas[0])
    G.add_nodes_from(range(n))
    for linea in lineas[1:]:
        if not linea.strip():
            continue
        # Las aristas pueden estar separadas por tabulador o espacio
        u, v = map(int, linea.split()[:2])
        G.add_edge(u, v)
    return G

def leer_particion(archivo):
    """Lee la partición: cada línea tiene nodo y comunidad.
    Devuelve diccionario {nodo: id_comunidad}."""
    particion = {}
    with open(archivo, 'r') as f:
        for linea in f:
            if not linea.strip():
                continue
            nodo, com = map(int, linea.split()[:2])
            particion[nodo] = com
    return particion

# ============================================================
# 3. CARGA DE DATOS
# ============================================================
print("Cargando red...")
G = leer_red(ARCHIVO_RED)
print(f"  Nodos: {G.number_of_nodes()}, Aristas: {G.number_of_edges()}")

print("Cargando partición...")
particion = leer_particion(ARCHIVO_PARTICION)
comunidades_unicas = sorted(set(particion.values()))
K = len(comunidades_unicas)
print(f"  Comunidades detectadas: {K} (IDs: {comunidades_unicas})")

# ============================================================
# 4. COLORES (paleta personalizada como en el ejemplo)
# ============================================================
# Usamos la misma lista de colores que aparecía en el código de ejemplo
colores_base = ['#789e97', '#2d6280', '#202642', '#c93c2c', '#d76840', '#e4a166']

# Si hay más comunidades que colores, interpolamos
def interpolar_colores(colors, num):
    """Crea una lista de num colores interpolando entre los dados."""
    from matplotlib.colors import LinearSegmentedColormap
    cmap = LinearSegmentedColormap.from_list("custom", colors, N=num)
    return [cmap(i / (num - 1)) for i in range(num)]

colores_comunidad = interpolar_colores(colores_base, K)
# Mapeo de cada id de comunidad a su color
color_map = {com: colores_comunidad[i] for i, com in enumerate(comunidades_unicas)}

# Reordenar colores para que coincida con el orden visual del artículo (opcional)
# En el código de ejemplo reasignaban colores manualmente; aquí mantendremos el orden natural.

# Color de cada nodo según su comunidad
node_colors = [color_map[particion[n]] for n in G.nodes()]

# ============================================================
# 5. DIBUJO DEL GRAFO
# ============================================================
fig, ax = plt.subplots(figsize=(10, 8))

# Layout con semilla fija para reproducibilidad
pos = nx.spring_layout(G, seed=42)

nx.draw(
    G,
    pos,
    ax=ax,
    node_color=node_colors,
    node_size=30,
    edge_color='gray',
    width=0.3,
    with_labels=False
)
'''
# Mostrar el número de cada nodo en el grafo
nx.draw_networkx_labels(
    G,
    pos,
    labels={n: str(n) for n in G.nodes()},
    font_size=6,
    font_color='black'
)
'''
ax.set_title("Componente 7 – Comunidades detectadas")
ax.axis('off')

# Leyenda con los colores de las comunidades
from matplotlib.patches import Patch
leyenda = [Patch(facecolor=color_map[com], label=f'Comunidad {com}') for com in comunidades_unicas]
ax.legend(handles=leyenda, loc='upper right', fontsize=8)

plt.tight_layout()
plt.savefig(os.path.join(CARPETA_SALIDA, 'fig4_comp7_graph.png'), dpi=300)
plt.show()
print(f"Figura guardada en {CARPETA_SALIDA}/fig4_comp7_graph.png")