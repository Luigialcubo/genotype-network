import networkx as nx
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent

for nombre in ["BWetal2022_LCC.edge", "BWetal2022_2LCC.edge"]:
    ruta = BASE_DIR / nombre

    print("\nArchivo:", nombre)
    print("Existe:", ruta.exists())

    G = nx.read_edgelist(ruta, nodetype=int, data=False)

    print("Nodos:", G.number_of_nodes())
    print("Enlaces:", G.number_of_edges())
    print("Primeras aristas:", list(G.edges())[:5])

    componentes = list(nx.connected_components(G))
    tamanos = sorted([len(c) for c in componentes], reverse=True)

    print("¿Es conexa?:", nx.is_connected(G))
    print("Número de componentes conexas:", len(componentes))
    print("Tamaños de componentes:", tamanos[:10])