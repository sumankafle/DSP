# plot_stats.gp — Plot noisy signal with running mean and RMS envelope
# Usage: cd DSP && gnuplot scripts/plot_stats.gp
#
# Reads:  data/stats_signal.csv, stats_running.csv
# Output: data/stats_plot.png

set datafile separator ","
set grid
set key outside right top

set terminal pngcairo size 900,500 enhanced font "sans,11"
set output "data/stats_plot.png"

set xlabel "Sample index"
set ylabel "Amplitude"
set title "Running Statistics: Noisy Sine (f = 0.02, noise std = 0.3)"

plot "data/stats_signal.csv" every ::1 using 1:2 with lines lw 0.6 lc rgb "#B0B0B0" \
         title "Noisy signal", \
     "data/stats_running.csv" every ::1 using 1:2 with lines lw 1.8 lc rgb "#2060C0" \
         title "Running mean (win=32)", \
     "data/stats_running.csv" every ::1 using 1:3 with lines lw 1.8 lc rgb "#C02040" \
         title "Running RMS (win=32)"

set output
print "Wrote data/stats_plot.png"
