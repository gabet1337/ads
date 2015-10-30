#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'
set title "Asymptotic running times"
set output 'runtime_plot.png'
set xlabel "N"
set ylabel "Running time"
set style line 1 lc rgb '#0000FF' pt 5 ps 1 lt 1 lw 1
set style line 2 lc rgb '#00FF00' pt 4 ps 1 lt 1 lw 1
set style line 3 lc rgb '#FF0000' pt 8 ps 1 lt 1 lw 1
#set xtics autofreq
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0
set xrange [2**0:2**23]
set logscale x 2
#set logscale y 2
set format x "2^{%L}"
set key top left
plot log(x)/log(2) ls 1 with line title 'binary heap O(log_2(x))' , \
     1 ls 2 with line title 'fibonacci heap O(1)', \
     log(log(2**24)/log(2))/log(2) ls 3 with line title 'van emde boas O(log_2(log_2(2^{24})))'

