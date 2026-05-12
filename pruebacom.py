import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from matplotlib.lines import Line2D


def leer_comunidades(path):
    df = pd.read_csv(path, sep=r"\s+", header=None)
    df.columns = ["nodo", "comunidad"]
    return dict(zip(df["nodo"], df["comunidad"]))


def leer_aristas(path):
    with open(path, "r") as f:
        lineas = f.readlines()

    n_nodos = int(lineas[0].strip())
    aristas = []

    for linea in lineas[1:]:
        partes = linea.strip().split()
        if len(partes) >= 2:
            i = int(partes[0])
            j = int(partes[1])
            aristas.append((i, j))

    return n_nodos, aristas


def plot_red_comunidades(
    archivo_comunidades,
    archivo_aristas,
    salida="red_comunidades.png",
    colores=None,      # <-- lista de colores (opcional)
    seed=7
):
    # Paleta por defecto (si no se pasa una lista de colores)
    if colores is None:
        colores = [
            "#789e97",
            "#2d6280",
            "#202642",
            "#6b2a31",
            "#c93c2c",
            "#d76840",
            "#e4a166"
        ]

    comunidades = leer_comunidades(archivo_comunidades)
    n_nodos, aristas = leer_aristas(archivo_aristas)

    G = nx.Graph()
    G.add_nodes_from(range(n_nodos))

    for nodo, comunidad in comunidades.items():
        G.nodes[nodo]["comunidad"] = comunidad

    G.add_edges_from(aristas)

    comunidades_unicas = sorted(set(comunidades.values()))

    if len(comunidades_unicas) > len(colores):
        raise ValueError(
            f"Hay {len(comunidades_unicas)} comunidades, "
            f"pero solo has dado {len(colores)} colores."
        )

    color_por_comunidad = {
        comunidad: colores[i]
        for i, comunidad in enumerate(comunidades_unicas)
    }

    node_colors = [
        color_por_comunidad.get(
            G.nodes[n].get("comunidad"),
            "lightgray"
        )
        for n in G.nodes()
    ]

    pos = nx.spring_layout(
        G,
        seed=seed,
        iterations=300,
        k=1 / np.sqrt(n_nodos)
    )

    plt.figure(figsize=(10, 7))

    nx.draw_networkx_edges(
        G,
        pos,
        edge_color="gray",
        alpha=0.15,
        width=0.5
    )
    
    nx.draw_networkx_nodes(
        G,
        pos,
        node_color=node_colors,
        node_size=40,
        linewidths=0
    )
    '''
    # Mostrar el número de cada nodo en el grafo
    nx.draw_networkx_labels(
        G,
        pos,
        labels={n: str(n) for n in G.nodes()},
        font_size=6,
        font_color='black'
    )
    '''
    leyenda = [
        Line2D(
            [0],
            [0],
            marker="o",
            color="w",
            label=f"Comunidad {comunidad}",
            markerfacecolor=color_por_comunidad[comunidad],
            markersize=9
        )
        for comunidad in comunidades_unicas
    ]

    plt.legend(
        handles=leyenda,
        loc="best",
        frameon=False,
        fontsize=10
    )

    plt.axis("off")
    plt.tight_layout()
    plt.savefig(salida, dpi=300, bbox_inches="tight")


# ============================================================
# LLAMADAS CON PALETAS DIFERENTES PARA CADA COMPONENTE
# (cambia los colores por los que tú quieras)
# ============================================================

# Componente 1 (7 colores de ejemplo)
plot_red_comunidades(
    archivo_comunidades="results/fig4/particion_comp1.txt",
    archivo_aristas="componentes_conexas/componente_1.txt",
    salida="plots/fig4/fig4_comp1grafo_prueba.png",
    colores=[
        "#789e97", "#2d6280", "#202642", "#6b2a31", "#c93c2c", "#d76840", "#e4a166"
    ]
)

# Componente 2 (6 colores de ejemplo)
plot_red_comunidades(
    archivo_comunidades="results/fig4/particion_comp2.txt",
    archivo_aristas="componentes_conexas/componente_2.txt",
    salida="plots/fig4/fig4_comp2grafo_prueba.png",
    colores=[
        "#789e97", "#2d6280", "#202642", "#c93c2c", "#d76840", "#e4a166"
    ]
)

# Componente 7 (4 colores de ejemplo)
plot_red_comunidades(
    archivo_comunidades="results/fig4/particion_comp7.txt",
    archivo_aristas="componentes_conexas/componente_7.txt",
    salida="plots/fig4/fig4_comp7grafo_prueba.png",
    colores=[
        "#789e97", "#202642", "#c93c2c", "#e4a166"
    ]
)