import networkx as net
import matplotlib.pyplot as plt
import numpy as np
import  infomap as Infmap


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
plt.savefig("figura_2b_distribucion_grado.png", dpi=300)
plt.show()
