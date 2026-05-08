from pathlib import Path
import networkx as nx

BASE_DIR = Path(__file__).resolve().parent
OUT_DIR = BASE_DIR / "componentes_conexas_xml"

archivos = sorted(
    OUT_DIR.glob("componente_*.edge"),
    key=lambda p: int(p.stem.split("_")[1])
)

print("Número de archivos encontrados:", len(archivos))

for archivo in archivos[:10]:
    G = nx.read_edgelist(archivo, nodetype=str, data=False)

    print("\nArchivo:", archivo.name)
    print("Nodos:", G.number_of_nodes())
    print("Enlaces:", G.number_of_edges())
    print("¿Es conexa?:", nx.is_connected(G))
    print("Número de componentes dentro:", nx.number_connected_components(G))