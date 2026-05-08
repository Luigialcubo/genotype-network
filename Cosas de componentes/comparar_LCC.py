from pathlib import Path
import networkx as nx
from networkx.algorithms.graph_hashing import weisfeiler_lehman_graph_hash

BASE_DIR = Path(__file__).resolve().parent

archivo_lcc_original = BASE_DIR / "BWetal2022_LCC.edge"
archivo_lcc_generada = BASE_DIR / "componentes_conexas" / "componente_1.edge"

G1 = nx.read_edgelist(archivo_lcc_original, nodetype=int, data=False)
G2 = nx.read_edgelist(archivo_lcc_generada, nodetype=int, data=False)

print("=== LCC ORIGINAL ===")
print("Nodos:", G1.number_of_nodes())
print("Enlaces:", G1.number_of_edges())
print("Conexa:", nx.is_connected(G1))

print("\n=== COMPONENTE 1 GENERADA ===")
print("Nodos:", G2.number_of_nodes())
print("Enlaces:", G2.number_of_edges())
print("Conexa:", nx.is_connected(G2))

print("\n=== COMPARACIÓN ESTRUCTURAL ===")

# Comparar secuencia de grados
grados1 = sorted([d for _, d in G1.degree()])
grados2 = sorted([d for _, d in G2.degree()])

print("Misma secuencia de grados:", grados1 == grados2)

# Comparar clustering medio
print("Clustering original:", nx.average_clustering(G1))
print("Clustering generada:", nx.average_clustering(G2))

# Comparar assortatividad
print("Assortatividad original:", nx.degree_assortativity_coefficient(G1))
print("Assortatividad generada:", nx.degree_assortativity_coefficient(G2))

# Hash estructural aproximado
hash1 = weisfeiler_lehman_graph_hash(G1)
hash2 = weisfeiler_lehman_graph_hash(G2)

print("Hash WL original:", hash1)
print("Hash WL generada:", hash2)
print("Mismo hash WL:", hash1 == hash2)