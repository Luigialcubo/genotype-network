
"""
Carga cada componente de la red de influenza A (H3N2) desde la carpeta
'componentes_conexas', ejecuta Infomap y guarda la partición en archivos
dentro de la carpeta 'componentes_conexas_particion'.
"""

import networkx as nx
import infomap
import os

def get_communities(graph):
    """Ejecuta Infomap (two-level) y devuelve un diccionario {nodo: id_comunidad}."""
    im = infomap.Infomap("--two-level --silent")
    for u, v in graph.edges():
        im.addLink(u, v)
    im.run()
    partition = {}
    for node in im.tree:
        if node.isLeaf:
            partition[node.node_id] = node.module_id
    return partition

def leer_red(archivo):
    """Lee una red desde un archivo con formato de lista de aristas: u v."""
    G = nx.Graph()
    with open(archivo, 'r') as f:
        for linea in f:
            linea = linea.strip()
            if not linea:
                continue
            u, v = map(int, linea.split()[:2])
            G.add_edge(u, v)
    return G

# Carpetas
carpeta_redes = "componentes_conexas"
carpeta_salida = "componentes_conexas_particion"

# Crear carpeta de salida si no existe
os.makedirs(carpeta_salida, exist_ok=True)

# Procesa cada componente
for comp_id in [1, 2, 7]:
    archivo_red = os.path.join(carpeta_redes, f"componente_{comp_id}.txt")
    archivo_salida = os.path.join(carpeta_salida, f"particion_comp{comp_id}.txt")
    
    print(f"Cargando {archivo_red}...")
    G = leer_red(archivo_red)
    print(f"  Nodos: {G.number_of_nodes()}, Aristas: {G.number_of_edges()}")
    print(f"  Detectando comunidades...")
    
    particion = get_communities(G)
    
    num_comunidades = len(set(particion.values()))
    print(f"  Comunidades encontradas: {num_comunidades}")
    
    # Guardar partición
    with open(archivo_salida, 'w') as f:
        for nodo, comunidad in particion.items():
            f.write(f"{nodo} {comunidad}\n")
    print(f"  Partición guardada en {archivo_salida}\n")

print("¡Detección de comunidades completada!")