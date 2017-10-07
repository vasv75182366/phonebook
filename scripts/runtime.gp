reset
set ylabel 'time(sec)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'
set style histogram cluster gap 10


plot [:][0:0.1]'output.txt' using 2:xtic(1) with histogram title 'original', \
'' using 3:xtic(1) with histogram title 'reduce size', \
'' using 4:xtic(1) with histogram title 'BKDR hash', \
'' using ($0 - 0.08):($2 + 0.01):2 with labels title ' ' rotate by 90, \
'' using ($0):($3 + 0.01):3 with labels title ' ' rotate by 90, \
'' using ($0 + 0.08):($4 + 0.01):4 with labels title ' ' rotate by 90, \
