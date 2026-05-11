import os
import sys
import networkx as nx
import matplotlib.pyplot as plt


#Directorios 
carpeta_plots = "plots"
if not os.path.exists(carpeta_plots):
    os.makedirs(carpeta_plots)

carpeta_redes = "results/redes_txt"
if not os.path.exists(carpeta_redes):
    os.makedirs(carpeta_redes)

#Creacion de la red estrella, devuelve un grafico G
def crear_cadena_estrellas(num_clusters, hojas_por_cluster):
    
    G = nx.Graph()
    desplazamiento = 0      # para renombrar nodos sin colisiones
    centros = []            # guarda los índices de los nodos centrales

    for c in range(num_clusters):
        centro = desplazamiento
        centros.append(centro)
        G.add_node(centro)
        # Añadir hojas y conectarlas al centro
        for hoja_num in range(hojas_por_cluster[c]):
            hoja = desplazamiento + 1 + hoja_num
            G.add_node(hoja)
            G.add_edge(centro, hoja)
        # Avanzar el desplazamiento: 1 centro + L hojas
        desplazamiento += 1 + hojas_por_cluster[c]

    # Conectar los centros consecutivos
    for i in range(len(centros) - 1):
        G.add_edge(centros[i], centros[i + 1])

    return G

#Reticula lattice
def crear_reticula(filas, columnas):
    
    G = nx.grid_2d_graph(filas, columnas, periodic=False)
    G = nx.convert_node_labels_to_integers(G)
    return G

#Plot
def guardar_grafico(G, nombre_archivo, titulo, con_etiquetas=False):
    
    plt.figure(figsize=(16, 12))
    # Usamos spring_layout para una disposición legible 
    pos = nx.spring_layout(G, seed=42, k=0.5)
    nx.draw(G, pos, with_labels=con_etiquetas, node_color='lightblue',
            edge_color='gray', node_size=80, font_size=8)
    plt.title(titulo)
    ruta_completa = os.path.join(carpeta_plots, nombre_archivo)
    plt.savefig(ruta_completa, dpi=150, bbox_inches='tight')
    plt.close()  # cerrar la figura para no saturar la memoria
    print(f"Gráfico guardado: {ruta_completa}")

#Archivo texto
def exportar_txt(G, nombre_archivo):
    
    ruta = os.path.join(carpeta_redes, nombre_archivo)
    try:
        with open(ruta, 'w') as f:
            f.write(f"{G.number_of_nodes()}\n")
            for u, v in G.edges():
                f.write(f"{u} {v}\n")
        print(f"   -> Archivo TXT exportado: {ruta}")
    except Exception as e:
        print(f"   ERROR al escribir {ruta}: {e}")

#Generacion de las redes en main
if __name__ == "__main__":
    """
    #Red 1 homogenea, 5 clusters con 3 hojas cada uno
    hojas_homogenea = [40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40]
    red_homogenea = crear_cadena_estrellas(41, hojas_homogenea)
    guardar_grafico(red_homogenea, "cadena_estrellas_homogenea.png",
                    "Cadena de estrellas homogénea (11 clusters, 41 hojas)")
    exportar_txt(red_homogenea, "red_homogenea.txt")

    #Red 2 heterogenea, 5 clusters
    hojas_heterogenea = [31, 0, 0, 0, 43, 0, 0, 0, 27, 0, 0, 0, 38, 0, 0, 0, 22, 0, 0, 0, 44, 0, 0, 0, 35, 0, 0, 0, 41, 0, 0, 0, 29, 0, 0, 0, 37, 0, 0, 0, 42]
    red_heterogenea = crear_cadena_estrellas(41, hojas_heterogenea)
    guardar_grafico(red_heterogenea, "cadena_estrellas_heterogenea.png",
                    "Cadena de estrellas heterogénea")
    exportar_txt(red_heterogenea, "red_heterogenea.txt")
    

    #Reticula lattice
    red_lattice = crear_reticula(4, 50)
    guardar_grafico(red_lattice, "reticula_4x50.png",
                    "Retícula 2D de 4x50 nodos", con_etiquetas=False)
    exportar_txt(red_lattice, "reticula_4x50.txt")

    
    hojas_ceros = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    red_ceros = crear_cadena_estrellas(20, hojas_ceros)
    guardar_grafico(red_ceros, "cadena_estrellas_ceros.png",
                    "Cadena lineal de 20 clusters y ninguna hoja")
    exportar_txt(red_ceros, "red_ceros.txt")
    """

    #Red 1 homogenea, 5 clusters con 3 hojas cada uno
    hojas_homogenea = [40, 0, 0, 0, 40, 0, 0, 0, 40]
    red_homogenea = crear_cadena_estrellas(9, hojas_homogenea)
    #guardar_grafico(red_homogenea, "cadena_estrellas_homogenea.png",
                #    "Cadena de estrellas homogénea (11 clusters, 41 hojas)")
    exportar_txt(red_homogenea, "red_homogenea_3bloques.txt")

    #Red 2 heterogenea, 5 clusters
    hojas_heterogenea = [31, 0, 0, 0, 43, 0, 0, 0, 27]
    red_heterogenea = crear_cadena_estrellas(9, hojas_heterogenea)
    #guardar_grafico(red_heterogenea, "cadena_estrellas_heterogenea.png",
           #         "Cadena de estrellas heterogénea")
    exportar_txt(red_heterogenea, "red_heterogenea_3bloques.txt")
    

    #Reticula lattice
    red_lattice = crear_reticula(3, 50)
    #guardar_grafico(red_lattice, "reticula_3x50.png",
                #    "Retícula 2D de 3x50 nodos", con_etiquetas=False)
    exportar_txt(red_lattice, "reticula_3x50.txt")




