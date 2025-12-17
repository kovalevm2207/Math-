set terminal pdfcairo enhanced color size 18cm,9cm font "Arial,12"
set output "Graph/taylor.pdf"
set xrange [-0.2:0.4]
set yrange [0.9:2.3]
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
plot sin(x**3*15)+cos(7*x+9)**4+1 with lines lw 2 lc rgb "#D81B60" title "f(x)", 1.80404+(-7.35084)*(x-0.11)**1+(-42.0808)*(x-0.11)**2+609.661*(x-0.11)**3+1220.2*(x-0.11)**4+(-19042.7)*(x-0.11)**5+(-19886.7)*(x-0.11)**6+335046*(x-0.11)**7+234658*(x-0.11)**8+(-3.59705e+06)*(x-0.11)**9 with lines lw 2 lc rgb "#1E88E5"  title "g(x)", (-7.35084)*x+2.61263 with lines lw 2 lc rgb "#43A047"  title "h(x)", '-' with points pt 7 ps 0.5 lc "orange" title "A"
0.11 1.80404
e