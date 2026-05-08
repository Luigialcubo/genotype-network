"""Agrega el número total de nodos como primera línea en los archivos de componente."""

import os

archivos = [
    "componentes_conexas/componente_1.txt",
    "componentes_conexas/componente_2.txt",
    "componentes_conexas/componente_7.txt"
]

for ruta in archivos:
    with open(ruta, 'r') as f:
        lineas = f.readlines()
    
    max_nodo = -1
    for linea in lineas[1:]:        # saltamos la cabecera original
        if not linea.strip():
            continue
        u, v = map(int, linea.split())
        if u > max_nodo:
            max_nodo = u
        if v > max_nodo:
            max_nodo = v
    
    n_nodos = max_nodo + 1
    print(f"{ruta}: nodos = {n_nodos}")
    
    # Reescribir el archivo con N como primera línea
    with open(ruta, 'w') as f:
        f.write(f"{n_nodos}\n")
        f.writelines(lineas[1:])    # saltamos la cabecera original, pero conservamos el resto