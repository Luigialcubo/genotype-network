from pathlib import Path
import xml.etree.ElementTree as ET
import networkx as nx

BASE_DIR = Path(__file__).resolve().parent
GRAPHML_FILE = BASE_DIR / "BWetal2022_FullNetworkData.graphml"

OUT_DIR = BASE_DIR / "componentes_conexas_temporales"
OUT_DIR.mkdir(exist_ok=True)

G = nx.Graph()
node_time = {}
node_accession = {}

print("Leyendo GraphML y atributos temporales...")

for event, elem in ET.iterparse(GRAPHML_FILE, events=("end",)):
    tag = elem.tag.split("}")[-1]

    if tag == "node":
        node_id = elem.attrib.get("id")
        G.add_node(node_id)

        for child in elem:
            child_tag = child.tag.split("}")[-1]

            if child_tag == "data":
                key = child.attrib.get("key")
                value = child.text

                if key == "days" and value is not None:
                    day = int(value)
                    if day >= 0:
                        node_time[node_id] = day

                if key == "accession" and value is not None:
                    node_accession[node_id] = value

        elem.clear()

    elif tag == "edge":
        source = elem.attrib.get("source")
        target = elem.attrib.get("target")

        if source is not None and target is not None and source != target:
            G.add_edge(source, target)

        elem.clear()

print("Red cargada")
print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())
print("Nodos con day válido:", len(node_time))

componentes = sorted(nx.connected_components(G), key=len, reverse=True)

print("Número de componentes:", len(componentes))

for i, nodos in enumerate(componentes, start=1):
    subG = G.subgraph(nodos).copy()

    # Orden temporal:
    # 1) primero nodos con days válido, ordenados de menor a mayor
    # 2) luego nodos sin days válido
    nodos_ordenados = sorted(
        subG.nodes(),
        key=lambda n: (node_time.get(n, float("inf")), str(n))
    )

    mapping = {old: new for new, old in enumerate(nodos_ordenados)}

    subG_temp = nx.relabel_nodes(subG, mapping)

    salida_edge = OUT_DIR / f"componente_{i}_temporal.edge"
    salida_mapping = OUT_DIR / f"componente_{i}_mapping_temporal.txt"

    nx.write_edgelist(subG_temp, salida_edge, data=False)

    with open(salida_mapping, "w", encoding="utf-8") as f:
        f.write("nuevo_id\told_id\tdays\taccession\n")

        for old, new in mapping.items():
            f.write(
                f"{new}\t{old}\t{node_time.get(old, 'NA')}\t{node_accession.get(old, 'NA')}\n"
            )

    validos_comp = [node_time[n] for n in subG.nodes() if n in node_time]

    print(f"\nComponente {i}")
    print("Nodos:", subG_temp.number_of_nodes())
    print("Enlaces:", subG_temp.number_of_edges())
    print("Conexa:", nx.is_connected(subG_temp))
    print("Nodos con days válido:", len(validos_comp))

    if validos_comp:
        print("Day mínimo:", min(validos_comp))
        print("Day máximo:", max(validos_comp))
        print("Nodo inicial nuevo_id=0 tiene day:", node_time.get(nodos_ordenados[0], "NA"))
        print("Accession inicial:", node_accession.get(nodos_ordenados[0], "NA"))
    else:
        print("Esta componente no tiene nodos con fecha válida.")