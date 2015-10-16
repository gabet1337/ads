#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'
set title "Delete 1 element worst case"
set output 'delmin_del_1_time_worst.png'
set xlabel "N (input size)"
set ylabel "running time"
set style line 1 lc rgb '#0000FF' pt 5 ps 1 lt 1 lw 1
set style line 2 lc rgb '#00FF00' pt 4 ps 1 lt 1 lw 1
set style line 3 lc rgb '#FF0000' pt 8 ps 1 lt 1 lw 1
#set xtics autofreq
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0
#set xrange [2:2**22]
set logscale x 2
#set logscale y 2
set format x "2^{%L}"
set key left top
plot "worst_del_1_bh.dat" using 1:2 ls 1 with linespoints title 'binary heap',\
"worst_del_1_fq.dat" using 1:2 ls 2 with linespoints title 'fibonacci heap'

