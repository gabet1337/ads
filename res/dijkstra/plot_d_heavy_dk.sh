#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'

set title "Dijkstra on Heavy Graph (# DecreaseKey)"
set output 'd_heavy_dk.png'
set xlabel "N (# of vertices)"
set ylabel "# DecreaseKey"
set style line 1 lc rgb '#0000FF' pt 5 ps 1 lt 1 lw 1
set style line 2 lc rgb '#00FF00' pt 4 ps 1 lt 1 lw 1
set style line 3 lc rgb '#FF0000' pt 8 ps 1 lt 1 lw 1
set xtics autofreq
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0

set logscale x 2
#set logscale y 2
set format x "2^{%L}"
set yrange [0:1302955445]
#set logscale x 2
#set format x "%L/10000"

set key left top

plot "sssp_heavy_bh.dat" using 1:5 ls 1 with linespoints title 'binary heap', \
     "sssp_heavy_fq.dat" using 1:5 ls 2 with linespoints title 'fibonacci heap'
