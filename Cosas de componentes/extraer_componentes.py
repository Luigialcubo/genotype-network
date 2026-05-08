from pathlib import Path
import networkx as nx

BASE_DIR = Path(__file__).resolve().parent

# Cambia el nombre si tu archivo .gexf se llama distinto
GEXF_FILE = BASE_DIR / "BWetal2022_FullNetworkData.gexf"

OUT_DIR = BASE_DIR / "componentes_conexas"
OUT_DIR.mkdir(exist_ok=True)

# Leer red
G = nx.read_gexf(GEXF_FILE)

# Convertir a red simple no dirigida
G = nx.Graph(G)

# Eliminar self-loops si existen
G.remove_edges_from(nx.selfloop_edges(G))

print("Red completa:")
print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())

# Obtener componentes conexas ordenadas por tamaño
componentes = sorted(nx.connected_components(G), key=len, reverse=True)

print("\nNúmero de componentes conexas encontradas:", len(componentes))

# Guardar cada componente
for i, nodos in enumerate(componentes, start=1):
    subG = G.subgraph(nodos).copy()

    # Renumerar nodos desde 0 hasta N-1 para evitar problemas posteriores
    subG = nx.convert_node_labels_to_integers(
        subG,
        first_label=0,
        ordering="default"
    )

    salida = OUT_DIR / f"componente_{i}.edge"

    nx.write_edgelist(
        subG,
        salida,
        data=False
    )

    print(
        f"Componente {i}: "
        f"{subG.number_of_nodes()} nodos, "
        f"{subG.number_of_edges()} enlaces, "
        f"guardada en {salida}"
    )

print("\nProceso terminado.")