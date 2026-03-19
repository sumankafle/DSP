# plot_convolution.gp — Plot convolution input vs output
# Usage: cd DSP && gnuplot scripts/plot_convolution.gp
#
# Reads:  data/conv_output.csv
# Output: data/conv_plot.png

set datafile separator ","
set grid
set key outside right top

set terminal pngcairo size 900,500 enhanced font "sans,11"
set output "data/conv_plot.png"

set xlabel "Sample index"
set ylabel "Amplitude"
set title "Convolution: Impulse Train * Rectangular Pulse (16-tap)"

plot "data/conv_output.csv" every ::1 using 1:2 with impulses lw 1.8 lc rgb "#2060C0" \
         title "Input (impulse train)", \
     "data/conv_output.csv" every ::1 using 1:3 with lines lw 1.6 lc rgb "#C02040" \
         title "Convolved output"

set output
print "Wrote data/conv_plot.png"
