set term png
set output 'rho_R0.png'

set title 'Evolucion de ρ en funcion de R_0' font ',16'
set xlabel 'R0' font ',12'
set ylabel 'ρ' font ',12'
set grid
set pointsize 0.15
set ticslevel 0

set autoscale x
set xrange [0:10]

set autoscale y
set yrange [0:*]

unset key
plot "../../results/sis_txt/rho_R0.txt" using 1:2 with lines
