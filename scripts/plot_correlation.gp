# plot_correlation.gp — Plot auto and cross-correlation
# Usage: cd DSP && gnuplot scripts/plot_correlation.gp
#
# Reads:  data/corr_auto.csv, corr_cross.csv
# Output: data/corr_plot.png

set datafile separator ","
set grid

set terminal pngcairo size 900,600 enhanced font "sans,11"
set output "data/corr_plot.png"

set multiplot layout 2,1 title "Correlation Functions" font ",13"

set ylabel "Correlation"
set key off

# lag axis: CSV index 0..128 maps to lags -64..+64
set xlabel "Lag"
set title "Autocorrelation of Cosine (f = 0.08)"
plot "data/corr_auto.csv" every ::1 using ($1-64):2 with lines lw 1.4 lc rgb "#2060C0"

set title "Cross-correlation (signal vs 20-sample delayed copy)"
plot "data/corr_cross.csv" every ::1 using ($1-64):2 with lines lw 1.4 lc rgb "#C02040"

unset multiplot
set output
print "Wrote data/corr_plot.png"
