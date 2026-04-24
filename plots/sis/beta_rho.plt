set term png
set output 'beta_rho.png'

set title 'Evolucion de un sistema SIS' font ',16'
set xlabel 'β' font ',12'
set ylabel 'ρ' font ',12'
set grid
set pointsize 0.15
set ticslevel 0

set autoscale x
set xrange [0:1]

set autoscale y
set yrange [0:*]

# Definimos el color común para ambas líneas
set style line 1 linecolor rgb "orange"  # Usamos el color naranja

unset key
plot "../../resultados/datos_sis.txt" using 1:2 with linespoints
