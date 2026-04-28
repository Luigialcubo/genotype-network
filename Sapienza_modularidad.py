# La modularidad mide precisamente si existen esos grupos bien separados. 
# Por eso necesitas Infomap antes de calcular Q.
import networkx as nx
import matplotlib.pyplot as plt
from infomap import infomap as Infmap

archivo = "BWetal2022_2LCC.edge"
G = nx.read_edgelist(
    archivo,
    nodetype=int,
    data=False
)
print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())
print("¿Es conexa?", nx.is_connected(G))
# Cada nodo es un genotipo de H3N2 y Cada enlace indica que dos genotipos difieren 
# por una mutación. 

# Las comunidades detectadas por Infomap representan regiones del espacio genético
# donde el virus puede moverse fácilmente.

# En el contexto del paper, estas comunidades son importantes porque pueden comportarse 
# como grupos de variantes capaces de sostener brotes o cambios de predominancia epidémica

im = Infmap()

for u, v in G.edges():
    im.add_link(int(u), int(v))

im.run()

comunidades = {}

for node in im.tree:
    if node.is_leaf:
        comunidades[node.node_id] = node.module_id

print("Número de comunidades:", len(set(comunidades.values())))

archivo = "BWetal2022_2LCC.edge"

G = nx.read_edgelist(
    archivo,
    nodetype=int,
    data=False
)

# -------------------------
# Comunidades con Infomap
# -------------------------
im = Infmap()

for u, v in G.edges():
    im.add_link(int(u), int(v))

im.run()

comunidades_dict = {}

for node in im.tree:
    if node.is_leaf:
        comunidades_dict[node.node_id] = node.module_id

# Convertir a lista de conjuntos para NetworkX
modulos = {}

for nodo, comunidad in comunidades_dict.items():
    modulos.setdefault(comunidad, set()).add(nodo)

comunidades = list(modulos.values())

# -------------------------
# Métricas estructurales
# -------------------------
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