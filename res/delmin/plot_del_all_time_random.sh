#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'
set title "Delete entire heap random case"
set output 'delmin_del_all_time_random.png'
set xlabel "N (# of delete min operations)"
set ylabel "running time (ns per delete)"
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
plot "random_del_all_bh.dat" using 5:($2/$5*1000) ls 1 with linespoints title 'binary heap' , \
     "random_del_all_fq.dat" using 5:($2/$5*1000) ls 2 with linespoints title 'fibonacci heap'

