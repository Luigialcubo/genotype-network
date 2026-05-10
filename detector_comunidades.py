"""
Ejecutar: py detector_comunidades.py
Carga cada componente de la red de influenza A (H3N2) desde la carpeta
'componentes_conexas', ejecuta Infomap forzando el número de módulos
según el artículo y guarda la partición en 'results/fig4'.
"""

import networkx as nx
import infomap
import os

def get_communities(graph, target_modules):
    """
    Ejecuta Infomap en modo de dos niveles forzando el número deseado de módulos.
    Retorna diccionario {nodo: id_comunidad}.
    """
    # --two-level: partición plana (un solo nivel)
    # --preferred-number-of-modules N: Infomap intenta obtener exactamente N módulos
    # --silent: no mostrar mensajes de progreso
    cmd = f"--two-level --silent --preferred-number-of-modules {int(target_modules)}"
    im = infomap.Infomap(cmd)

    for u, v in graph.edges():
        im.addLink(u, v)

    im.run()

    partition = {}
    for node in im.tree:
        if node.isLeaf:                     # solo los nodos originales
            partition[node.node_id] = node.module_id
    return partition

def leer_red(archivo):
    """Lee la red ya reindexada (nodos 0..N-1)."""
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

# Carpetas
carpeta_redes = "componentes_conexas"
carpeta_salida = "results/fig4"
os.makedirs(carpeta_salida, exist_ok=True)

# Número de comunidades objetivo según el artículo
target_communities = {
    2: 2,
}

for comp_id, target in target_communities.items():
    archivo_red = os.path.join(carpeta_redes, f"componente_{comp_id}.txt")
    archivo_salida = os.path.join(carpeta_salida, f"particion_comp{comp_id}.txt")
    
    print(f"Cargando {archivo_red}...")
    G = leer_red(archivo_red)
    print(f"  Nodos: {G.number_of_nodes()}, Aristas: {G.number_of_edges()}")
    print(f"  Forzando a {target} comunidades...")
    
    particion = get_communities(G, target)
    num_comunidades = len(set(particion.values()))
    print(f"  Comunidades obtenidas: {num_comunidades}")
    
    with open(archivo_salida, 'w') as f:
        for nodo, comunidad in particion.items():
            f.write(f"{nodo} {comunidad}\n")
    print(f"  Partición guardada en {archivo_salida}\n")

print("¡Detección de comunidades completada!")