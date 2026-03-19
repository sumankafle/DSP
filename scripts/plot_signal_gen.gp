# plot_signal_gen.gp — Plot generated waveforms
# Usage: cd DSP && gnuplot scripts/plot_signal_gen.gp
#
# Reads:  data/siggen_cosine.csv, siggen_multitone.csv, siggen_impulse.csv
# Output: data/siggen_plot.png

set datafile separator ","
set grid

set terminal pngcairo size 900,900 enhanced font "sans,11"
set output "data/siggen_plot.png"

set multiplot layout 3,1 title "Signal Generation" font ",13"

set xlabel "Sample index"
set ylabel "Amplitude"
set key off

set title "Single Cosine (f = 0.08)"
plot "data/siggen_cosine.csv" every ::1 using 1:2 with lines lw 1.4 lc rgb "#2060C0"

set title "Sum of 3 Cosines (f = 0.05, 0.12, 0.25)"
plot "data/siggen_multitone.csv" every ::1 using 1:2 with lines lw 1.4 lc rgb "#20A040"

set title "Unit Impulse (delay = 32)"
plot "data/siggen_impulse.csv" every ::1 using 1:2 with impulses lw 2.0 lc rgb "#C02040"

unset multiplot
set output
print "Wrote data/siggen_plot.png"
