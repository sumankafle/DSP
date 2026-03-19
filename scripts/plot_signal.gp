# plot_signal.gp — Plot FIR filter input vs output
# Usage: cd DSP && gnuplot scripts/plot_signal.gp
#
# Reads:  data/fir_output.csv
# Output: data/fir_plot.png (or interactive window if terminal changed)

set datafile separator ","
set key outside right top
set grid
set xlabel "Sample index"
set ylabel "Amplitude"
set title "FIR Low-Pass Filter: Input vs Filtered Output"

# PNG output (comment out for interactive)
set terminal pngcairo size 900,500 enhanced font "sans,11"
set output "data/fir_plot.png"

plot "data/fir_output.csv" every ::1 using 1:2 with lines lw 1.2 \
         title "Input (0.05 + 0.35 norm freq)", \
     "data/fir_output.csv" every ::1 using 1:3 with lines lw 1.8 \
         title "Filtered (low-pass)"

set output
print "Wrote data/fir_plot.png"
