from pathlib import Path
import xml.etree.ElementTree as ET
import networkx as nx

BASE_DIR = Path(__file__).resolve().parent

GRAPHML_FILE = BASE_DIR / "BWetal2022_FullNetworkData.graphml"
OUT_DIR = BASE_DIR / "componentes_conexas_xml"
OUT_DIR.mkdir(exist_ok=True)

print("Leyendo GraphML:", GRAPHML_FILE)
print("Existe:", GRAPHML_FILE.exists())
print("Tamaño MB:", GRAPHML_FILE.stat().st_size / (1024 * 1024))

G = nx.Graph()

print("Parseando archivo XML...")

# Leer el GraphML directamente como XML
context = ET.iterparse(GRAPHML_FILE, events=("start",))

for event, elem in context:
    tag = elem.tag.split("}")[-1]  # elimina namespace si existe

    if tag == "node":
        node_id = elem.attrib.get("id")
        if node_id is not None:
            G.add_node(node_id)

    elif tag == "edge":
        source = elem.attrib.get("source")
        target = elem.attrib.get("target")

        if source is not None and target is not None:
            if source != target:
                G.add_edge(source, target)

print("Red cargada")
print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())

componentes = sorted(nx.connected_components(G), key=len, reverse=True)

print("Número de componentes conexas:", len(componentes))

for i, nodos in enumerate(componentes, start=1):
    subG = G.subgraph(nodos).copy()

    salida = OUT_DIR / f"componente_{i}.edge"

    nx.write_edgelist(subG, salida, data=False)

    print(
        f"Componente {i}: "
        f"{subG.number_of_nodes()} nodos, "
        f"{subG.number_of_edges()} enlaces, "
        f"conexa={nx.is_connected(subG)}"
    )

print("Componentes guardadas en:", OUT_DIR)
print("FIN")