# plot_median.gp — Plot median filter: noisy vs cleaned signal
# Usage: cd DSP && gnuplot scripts/plot_median.gp
#
# Reads:  data/median_output.csv
# Output: data/median_plot.png

set datafile separator ","
set grid
set key outside right top

set terminal pngcairo size 900,500 enhanced font "sans,11"
set output "data/median_plot.png"

set xlabel "Sample index"
set ylabel "Amplitude"
set title "Median Filter: Spike Removal from Sine Wave"

plot "data/median_output.csv" every ::1 using 1:2 with lines lw 0.8 lc rgb "#B0B0B0" \
         title "Noisy (salt-and-pepper spikes)", \
     "data/median_output.csv" every ::1 using 1:3 with lines lw 1.8 lc rgb "#2060C0" \
         title "Median filtered (5-point)"

set output
print "Wrote data/median_plot.png"
