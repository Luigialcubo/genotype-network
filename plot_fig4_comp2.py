'''Ejecutar: py plot_fig4_comp2.py'''

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Cargar los datos (sin cabecera, separador de espacios/tabulaciones)
# El archivo tiene 7 columnas: x, y1, y2, y3, y4, y5, y6
df = pd.read_csv('results/fig4/sims_comp2.txt', sep=r'\s+', header=None)

# La primera columna es x
x = df[0]
# El resto son las curvas y
y_cols = df.iloc[:, 1:]  # columnas 1 a 6
n_curves = y_cols.shape[1]

# Nombres genéricos para las etiquetas
etiquetas = ['Comunidad 1','Comunidad 2','Comunidad 3','Comunidad 4','Comunidad 5','Comunidad 6']

# Colores: usar un mapa de colores para un número variable de curvas
colores = ['#789e97', '#2d6280', '#202642', '#c93c2c', '#d76840', '#e4a166']


# --- Normalización por fila (cada fila suma 1) ---
row_sum = y_cols.sum(axis=1)
# Evitar división por cero
df_norm = y_cols.div(row_sum, axis=0).fillna(0)
# Añadir la columna x
df_norm.insert(0, 'x', x)

# Crear figura con dos subplots (3:1.5 para mejor contraste)
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True,
                               gridspec_kw={'height_ratios': [3, 1.5]})

# ---- Gráfica 1: valores originales (área bajo cada curva individual) ----
for i, col in enumerate(y_cols.columns):
    ax1.plot(x, y_cols[col], color=colores[i], linewidth=1.5, label=etiquetas[i])
    ax1.fill_between(x, y_cols[col], alpha=0.3, color=colores[i])
ax1.set_ylabel('I(t)', fontsize=16)
ax1.set_title('COMPONENTE 2 INFLUENZA A', fontsize=20,fontweight='bold')
ax1.set_xlim(0, 1000)
ax1.set_ylim(0, 0.35)
ax1.legend(loc='upper right', fontsize='small')
ax1.grid(True, linestyle='--', alpha=0.5)

# ---- Gráfica 2: valores normalizados por fila (área apilada, suma = 1 en cada x) ----
# Usamos stackplot con todas las columnas normalizadas
stack_data = [df_norm.iloc[:, i] for i in range(1, n_curves + 1)]  # excluir columna 'x'
ax2.stackplot(df_norm['x'], *stack_data, labels=etiquetas, colors=colores, alpha=0.7)
ax2.set_xlabel('t (días)', fontsize=16)
ax2.set_ylabel(r"$I(t)^{rel}$", fontsize=16)
ax2.set_ylim(0, 1)
ax2.set_xlim(0, 1000)
ax2.legend(loc='upper right', fontsize='small')
ax2.grid(True, linestyle='--', alpha=0.5)

# Guardar la figura en plots/fig4
salida_dir = 'plots/fig4'
os.makedirs(salida_dir, exist_ok=True)
plt.tight_layout()
plt.savefig(os.path.join(salida_dir, 'fig4_comp2.png'), dpi=300)
print(f'Figura guardada en {os.path.join(salida_dir, "fig4_comp2.png")}')
