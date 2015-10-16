#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'

set title "Dijkstra on Random Graph (# Conditional Branches)"
set output 'd_random_br.png'
set xlabel "N (# of vertices)"
set ylabel "# Conditional Branches"
set style line 1 lc rgb '#0000FF' pt 5 ps 1 lt 1 lw 1
set style line 2 lc rgb '#00FF00' pt 4 ps 1 lt 1 lw 1
set style line 3 lc rgb '#FF0000' pt 8 ps 1 lt 1 lw 1
set xtics autofreq
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0

set logscale x 2
#set logscale y 2
set format x "2^{%L}"
set yrange [0:555957538]
#set logscale x 2
#set format x "%L/10000"

set key left top

plot "sssp_random_bh.dat" using 1:3 ls 1 with linespoints title 'binary heap', \
     "sssp_random_fq.dat" using 1:3 ls 2 with linespoints title 'fibonacci heap'
