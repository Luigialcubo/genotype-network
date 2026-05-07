import os
import networkx as nx
from infomap import Infomap
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent
DATA_FILE = BASE_DIR / "BWetal2022_2LCC.edge"
RESULTS_DIR = BASE_DIR / "results"/"H3N2_txt"
RESULTS_DIR.mkdir(exist_ok=True)

print("Carpeta del script:", BASE_DIR)
print("Archivo de red:", DATA_FILE)
print("Existe archivo:", DATA_FILE.exists())

G = nx.read_edgelist(DATA_FILE, nodetype=int, data=False)

print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())
print("¿Es conexa?", nx.is_connected(G))

# Comunidades con Infomap
im = Infomap()

for u, v in G.edges():
    im.add_link(int(u), int(v))

im.run()

comunidades_dict = {}

for node in im.tree:
    if node.is_leaf:
        comunidades_dict[node.node_id] = node.module_id

modulos = {}

for nodo, comunidad in comunidades_dict.items():
    modulos.setdefault(comunidad, set()).add(nodo)

comunidades = list(modulos.values())

print("Número de comunidades:", len(comunidades))

# Métricas estructurales
L = nx.average_shortest_path_length(G)
C = nx.average_clustering(G)
T = nx.transitivity(G)
r = nx.degree_assortativity_coefficient(G)
Q = nx.algorithms.community.quality.modularity(G, comunidades)

print("Longitud media de caminos L:", L)
print("Clustering medio C:", C)
print("Transitividad T:", T)
print("Assortatividad r:", r)
print("Modularidad Q:", Q)

ruta_txt = RESULTS_DIR / "metricas.txt"

with open(ruta_txt, "w", encoding="utf-8") as f:
    f.write(f"L = {L}\n")
    f.write(f"C = {C}\n")
    f.write(f"T = {T}\n")
    f.write(f"r = {r}\n")
    f.write(f"Q = {Q}\n")

print("Resultados guardados en:", ruta_txt)