# plot_fft.gp — Plot time-domain signal and FFT magnitude spectrum
# Usage: cd DSP && gnuplot scripts/plot_fft.gp
#
# Reads:  data/fft_signal.csv, data/fft_spectrum.csv
# Output: data/fft_plot.png

set datafile separator ","
set grid

# PNG output
set terminal pngcairo size 900,700 enhanced font "sans,11"
set output "data/fft_plot.png"

set multiplot layout 2,1 title "FFT Analysis: 3-Tone Signal" font ",13"

# --- top panel: time-domain signal ---
set xlabel "Sample index"
set ylabel "Amplitude"
set title "Windowed Signal (Hamming)"
set key off
plot "data/fft_signal.csv" every ::1 using 1:2 with lines lw 1.2 lc rgb "#2060C0"

# --- bottom panel: magnitude spectrum ---
set xlabel "Frequency bin"
set ylabel "Magnitude"
set title "FFT Magnitude Spectrum (N/2 bins)"
set key off
plot "data/fft_spectrum.csv" every ::1 using 1:2 with impulses lw 1.8 lc rgb "#C02040"

unset multiplot
set output
print "Wrote data/fft_plot.png"
