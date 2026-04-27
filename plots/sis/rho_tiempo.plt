set term png
set output 'rho_tiempo.png'

set title 'Evolucion de ρ en funcion del tiempo' font ',16'
set xlabel 't' font ',12'
set ylabel 'ρ' font ',12'
set grid
set pointsize 0.15
set ticslevel 0

set autoscale x
set xrange [0:80]

set autoscale y
set yrange [0:*]

unset key
plot "../../resultados/rho_tiempo.txt" using 1:2 
