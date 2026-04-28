import os
import networkx as net
import matplotlib.pyplot as plt
import numpy as np
from infomap import Infomap
im = Infomap()

output_dir = "resultados"

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

archivo = "BWetal2022_2LCC.edge"

G = net.read_edgelist(
    archivo,
    nodetype=int,
    data=False
)

print("Nodos:", G.number_of_nodes())
print("Enlaces:", G.number_of_edges())
print("¿Es conexa?", net.is_connected(G))

grados = [d for _, d in G.degree()]

valores, cuentas = np.unique(grados, return_counts=True)
prob = cuentas / cuentas.sum()

plt.figure()
plt.scatter(valores, prob)
plt.xlabel("Grado k")
plt.ylabel("P(k)")
plt.title("Distribución de grados - H3N2")
plt.yscale("log")
plt.xscale("log")
plt.tight_layout()
ruta = os.path.join(output_dir, "figura_2b_distribucion_grado.png")
plt.savefig(ruta, dpi=300)

print("Figura guardada en:", ruta)

plt.show()

# Cada nodo es un genotipo de H3N2 y Cada enlace indica que dos genotipos difieren 
# por una mutación.
# Las comunidades detectadas por Infomap representan regiones del espacio genético
# donde el virus puede moverse fácilmente. 
# En el contexto del paper, estas comunidades son importantes porque pueden comportarse 
# como grupos de variantes capaces de sostener brotes o cambios de predominancia epidémica.

