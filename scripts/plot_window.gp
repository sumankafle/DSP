# plot_window.gp — Plot all window function shapes
# Usage: cd DSP && gnuplot scripts/plot_window.gp
#
# Reads:  data/window_shapes.csv
# Output: data/window_plot.png

set datafile separator ","
set grid
set key outside right top

set terminal pngcairo size 900,500 enhanced font "sans,11"
set output "data/window_plot.png"

set xlabel "Sample index"
set ylabel "Coefficient"
set title "Window Functions (N = 256)"
set yrange [0:1.1]

plot "data/window_shapes.csv" every ::1 using 1:2 with lines lw 1.4 title "Hamming", \
     "data/window_shapes.csv" every ::1 using 1:3 with lines lw 1.4 title "Hanning", \
     "data/window_shapes.csv" every ::1 using 1:4 with lines lw 1.4 title "Blackman", \
     "data/window_shapes.csv" every ::1 using 1:5 with lines lw 1.4 title "Blackman-Harris", \
     "data/window_shapes.csv" every ::1 using 1:6 with lines lw 1.4 title "Triangle"

set output
print "Wrote data/window_plot.png"
