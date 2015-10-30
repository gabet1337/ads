#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'
set title "Cache misses when inserting randomly"
set output 'insert random cache.png'
set xlabel "N (# of inserts)"
set ylabel "misses"
set style line 1 lc rgb '#0000FF' pt 5 ps 1 lt 1 lw 1
set style line 2 lc rgb '#00FF00' pt 4 ps 1 lt 1 lw 1
set style line 3 lc rgb '#FF0000' pt 8 ps 1 lt 1 lw 1
#set xtics autofreq
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0
set xrange [2**1:2**23]
set logscale x 2
#set logscale y 2
set format x "2^{%L}"
#set format y "2^{%L}"
set key top left
plot "inserts_random_bh_count.dat" using 1:6 ls 1 with linespoints title 'binary heap' , \
     "inserts_random_fq_count.dat" using 1:6 ls 2 with linespoints title 'fibonacci heap', \
     "inserts_random_veb_count.dat" using 1:6 ls 3 with linespoints title 'van emde boas'

