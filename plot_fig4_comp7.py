'''Ejecutar: py plot_fig4_comp7.py'''

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Cargar los datos
df = pd.read_csv('results/fig4/sims_comp7.txt', sep='\s+', header=None,
                 names=['x', 'y1', 'y2', 'y3', 'y4'])

# --- Normalización por fila (cada fila suma 1) ---
# Suma de las cuatro columnas por fila
row_sum = df[['y1', 'y2', 'y3', 'y4']].sum(axis=1)
# Evitar división por cero (si alguna fila suma 0, se rellena con 0)
df_norm = df[['y1', 'y2', 'y3', 'y4']].div(row_sum, axis=0).fillna(0)
# Añadir la columna x
df_norm.insert(0, 'x', df['x'])

# Colores y etiquetas
colores = ['blue', 'green', 'red', 'orange']
etiquetas = ['Columna 2', 'Columna 3', 'Columna 4', 'Columna 5']

# Crear figura con dos subplots
fig, (ax1, ax2) = plt.subplots(
    2,
    1,
    figsize=(12, 10),
    sharex=True,
    gridspec_kw={'height_ratios': [3, 1.5]}
)

# ---- Gráfica 1: valores originales (área bajo cada curva individual) ----
for i, col in enumerate(['y1', 'y2', 'y3', 'y4']):
    ax1.plot(df['x'], df[col], color=colores[i], linewidth=1.5, label=etiquetas[i])
    ax1.fill_between(df['x'], df[col], alpha=0.3, color=colores[i])
ax1.set_ylabel('Valores originales')
ax1.set_title('Curvas originales y área bajo cada una')
ax1.legend()
ax1.grid(True, linestyle='--', alpha=0.5)

# ---- Gráfica 2: valores normalizados por fila (área apilada, suma 1 en cada x) ----
# Usamos stackplot para rellenar el área apilada
ax2.stackplot(df_norm['x'], 
              df_norm['y1'], df_norm['y2'], df_norm['y3'], df_norm['y4'],
              labels=etiquetas, colors=colores, alpha=0.7)
ax2.set_xlabel('Primera columna (X)')
ax2.set_ylabel('Proporción (normalizada por fila)')
ax2.set_ylim(0, 1)  # Aseguramos que el eje Y llegue hasta 1
ax2.set_title('Contribución relativa de cada columna a lo largo de X (áreas apiladas)')
ax2.legend(loc='upper right')
ax2.grid(True, linestyle='--', alpha=0.5)

# Guardar la figura en plots/fig4
salida_dir = 'plots/fig4'
os.makedirs(salida_dir, exist_ok=True)
plt.tight_layout()
plt.savefig(os.path.join(salida_dir, 'fig4_comp7.png'), dpi=300)
print(f'Figura guardada en {os.path.join(salida_dir, "fig4_comp7.png")}')
plt.show()