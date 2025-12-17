set terminal pdfcairo enhanced color size 18cm,9cm font "Arial,12"
set output "Graph/taylor.pdf"
set xrange [0.5:1.5]
set yrange [-1:2.5]
set samples 10000
set xtics 1
set ytics 1
set mxtics 10
set mytics 10
set arrow from graph 0, first 0 to graph 1, first 0 nohead lw 2 lc "black" front
set arrow from first 0, graph 0 to first 0, graph 1 nohead lw 2 lc "black" front
set grid xtics ytics mxtics mytics lt 1 lc rgb "#aeb1b1" lw 1, lt 0 lc rgb "#aeb1b1" lw 0.5
set key right top
set arrow from 9.5, 0 to 10, 0 head size 0.08,20 lw 2 lc "black" front
set arrow from 0, 4.5 to 0, 5 head size 0.08,20 lw 2 lc "black" front
plot sin(x**3*15)+cos(7*x+9)**5+1 with lines lw 2 lc rgb "#D81B60" title "f(x)", 0.844806+(-25.7106)*(x-1)**1+(-629.603)*(x-1)**2+9384.8*(x-1)**3+142511*(x-1)**4+(-642671)*(x-1)**5+(-1.24499e+07)*(x-1)**6+(-2.27835e+06)*(x-1)**7 with lines lw 2 lc rgb "#1E88E5"  title "g(x)", (-25.7106)*x+26.5554 with lines lw 2 lc rgb "#43A047"  title "h(x)", '-' with points pt 7 ps 0.5 lc "orange" title "A"
1 0.844806
e