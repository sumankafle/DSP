# plot_noise.gp — Plot Gaussian and uniform noise
# Usage: cd DSP && gnuplot scripts/plot_noise.gp
#
# Reads:  data/noise_gaussian.csv, noise_uniform.csv
# Output: data/noise_plot.png

set datafile separator ","
set grid

set terminal pngcairo size 900,600 enhanced font "sans,11"
set output "data/noise_plot.png"

set multiplot layout 2,1 title "Random Noise Generators" font ",13"

set xlabel "Sample index"
set ylabel "Amplitude"
set key off

set title "Gaussian Noise (zero-mean, unit variance)"
plot "data/noise_gaussian.csv" every ::1 using 1:2 with lines lw 0.8 lc rgb "#4060C0"

set title "Uniform Noise [-0.5, 0.5]"
plot "data/noise_uniform.csv" every ::1 using 1:2 with lines lw 0.8 lc rgb "#C06040"

unset multiplot
set output
print "Wrote data/noise_plot.png"
