"""
Ejecutar: py detector_comunidades_comp2.py
Lee la red componente_2.txt y la partición particion_comp2.txt (2 comunidades),
y para cada comunidad aplica Infomap forzando 3 módulos en su subgrafo.
Genera una nueva partición con 6 comunidades, ordenada por comunidad,
y la guarda en results/fig4/particion_comp2_6.txt.
"""

import networkx as nx
import infomap
import os

# ============================================================
# 0. Configuración
# ============================================================
ARCHIVO_RED       = "componentes_conexas/componente_2.txt"
ARCHIVO_PARTICION = "results/fig4/particion_comp2.txt"
ARCHIVO_SALIDA    = "results/fig4/particion_comp2_6.txt"

# ============================================================
# 1. Lectura de la red
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
        u, v = map(int, linea.split()[:2])
        G.add_edge(u, v)
    return G

G = leer_red(ARCHIVO_RED)
print(f"Red cargada: {G.number_of_nodes()} nodos, {G.number_of_edges()} aristas")

# ============================================================
# 2. Lectura de la partición original (2 comunidades)
# ============================================================
particion_original = {}
with open(ARCHIVO_PARTICION, 'r') as f:
    for linea in f:
        if not linea.strip():
            continue
        nodo, com = map(int, linea.split()[:2])
        particion_original[nodo] = com

comunidades_orig = set(particion_original.values())
print(f"Comunidades originales: {comunidades_orig}")

# Agrupar nodos por comunidad original
nodos_por_com = {c: [] for c in comunidades_orig}
for nodo, com in particion_original.items():
    nodos_por_com[com].append(nodo)

# ============================================================
# 3. Para cada comunidad original, subdividir en 3 módulos
# ============================================================
nueva_particion = {}   # nodo -> nuevo id (de 1 a 6)
siguiente_id = 1

for com_orig, nodos in nodos_por_com.items():
    print(f"\nProcesando comunidad {com_orig} ({len(nodos)} nodos)...")
    # Crear subgrafo inducido
    subG = G.subgraph(nodos).copy()
    print(f"  Subgrafo: {subG.number_of_nodes()} nodos, {subG.number_of_edges()} aristas")
    
    # Ejecutar Infomap forzando 3 módulos
    cmd = "--two-level --silent --preferred-number-of-modules 3"
    im = infomap.Infomap(cmd)
    for u, v in subG.edges():
        im.addLink(u, v)
    im.run()
    
    # Obtener partición del subgrafo
    sub_particion = {}
    for node in im.tree:
        if node.isLeaf:
            sub_particion[node.node_id] = node.module_id
    
    # Verificar cuántos módulos se obtuvieron
    modulos_sub = set(sub_particion.values())
    print(f"  Módulos en el subgrafo: {len(modulos_sub)} (IDs: {modulos_sub})")
    
    # Asignar nuevos IDs consecutivos a cada sub-módulo
    mapeo_modulos = {}
    for mod in sorted(modulos_sub):
        mapeo_modulos[mod] = siguiente_id
        siguiente_id += 1
    
    # Guardar en la nueva partición global
    for nodo in nodos:
        if nodo in sub_particion:
            nueva_particion[nodo] = mapeo_modulos[sub_particion[nodo]]
        else:
            # Nodo aislado (sin aristas), asignarlo al primer módulo del grupo
            nueva_particion[nodo] = mapeo_modulos[list(modulos_sub)[0]]

print(f"\nNueva partición generada: {siguiente_id - 1} comunidades")

# ============================================================
# 4. Guardar la nueva partición ordenada por comunidades
# ============================================================
# Agrupar nodos por comunidad nueva
nodos_por_nueva_com = {c: [] for c in range(1, siguiente_id)}
for nodo, com in nueva_particion.items():
    nodos_por_nueva_com[com].append(nodo)

os.makedirs(os.path.dirname(ARCHIVO_SALIDA), exist_ok=True)
with open(ARCHIVO_SALIDA, 'w') as f:
    for com in range(1, siguiente_id):
        # Ordenar numéricamente los nodos dentro de cada comunidad
        for nodo in sorted(nodos_por_nueva_com[com]):
            f.write(f"{nodo} {com}\n")

print(f"Particion guardada en {ARCHIVO_SALIDA}")