
"""
detect_communities_fuerzo.py
Carga cada componente de la red de influenza A (H3N2) desde la carpeta
'componentes_conexas', ejecuta Infomap forzando el número de módulos
según el artículo y guarda la partición en 'results/fig4'.
"""

import networkx as nx
import infomap
import os

def get_communities(graph, target_modules):
    """
    Ejecuta Infomap (two-level) forzando el número deseado de comunidades.
    Retorna diccionario {nodo: id_comunidad}.
    """
    # Si target_modules es 0 o None, no se fuerza el número (resolución por defecto)
    if target_modules and target_modules > 0:
        cmd = f"--two-level --silent --preferred-number-of-modules {int(target_modules)}"
    else:
        cmd = "--two-level --silent"
    
    im = infomap.Infomap(cmd)
    for u, v in graph.edges():
        im.addLink(u, v)
    im.run()
    
    partition = {}
    for node in im.tree:
        if node.isLeaf:
            partition[node.node_id] = node.module_id
    return partition

def leer_red(archivo):
    """Lee red con formato: primera línea = N, luego aristas i j."""
    G = nx.Graph()
    with open(archivo, 'r') as f:
        lineas = f.read().strip().splitlines()
    n = int(lineas[0])                     # número total de nodos
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

# Crear carpeta de salida si no existe
os.makedirs(carpeta_salida, exist_ok=True)

# Número de comunidades objetivo según el artículo
target_communities = {
    1: 7,
    2: 6,
    7: 4   # El artículo no da cifra exacta, se asume similar (5-7)
}

# Procesa cada componente
for comp_id in [1, 2, 7]:
    archivo_red = os.path.join(carpeta_redes, f"componente_{comp_id}.txt")
    archivo_salida = os.path.join(carpeta_salida, f"particion_comp{comp_id}.txt")
    
    print(f"Cargando {archivo_red}...")
    G = leer_red(archivo_red)
    print(f"  Nodos: {G.number_of_nodes()}, Aristas: {G.number_of_edges()}")
    
    target = target_communities[comp_id]
    print(f"  Forzando a {target} comunidades...")
    particion = get_communities(G, target)
    
    num_comunidades = len(set(particion.values()))
    print(f"  Comunidades obtenidas: {num_comunidades}")
    
    # Guardar partición
    with open(archivo_salida, 'w') as f:
        for nodo, comunidad in particion.items():
            f.write(f"{nodo} {comunidad}\n")
    print(f"  Partición guardada en {archivo_salida}\n")

print("¡Detección de comunidades completada!")