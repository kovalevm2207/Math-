set terminal pdfcairo enhanced color size 18cm,9cm font "Arial,12"
set output "Graph/graph2.pdf"
set xrange [-0.5:0.5]
set yrange [-15:15]
set samples 10000
set xtics 1
set ytics 1
set mxtics 10
set mytics 10
set arrow from graph 0, first 0 to graph 1, first 0 nohead lw 2 lc "black" front
set arrow from first 0, graph 0 to first 0, graph 1 nohead lw 2 lc "black" front
set grid xtics ytics mxtics mytics lt 1 lc rgb "#777a7b" lw 1, lt 0 lc rgb "#777a7b" lw 0.5
set key right top
set arrow from 9.5, 0 to 10, 0 head size 0.08,20 lw 2 lc "black" front
set arrow from 0, 4.5 to 0, 5 head size 0.08,20 lw 2 lc "black" front
plot cos(x**3*15)*3*x**2*15+5*cos(7*x+9)**4*(-1)*sin(7*x+9)*7 with lines lw 2 lc rgb "#aa00ff" title "f(x)", (-1)*sin(x**3*15)*3*x**2*15*3*x**2*15+cos(x**3*15)*3*2*x*15+5*4*cos(7*x+9)**3*(-1)*sin(7*x+9)*7*(-1)*sin(7*x+9)*7+5*cos(7*x+9)**4*(-1)*cos(7*x+9)*7*7 with lines lw 2 lc rgb "#00ffee"  title "g(x)"
