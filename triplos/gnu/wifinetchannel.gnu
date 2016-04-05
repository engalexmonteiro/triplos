set terminal x11 size 800,600
set key outside Left
set title "TripleS - Smart Sense Spectre\nChannels x Time"
set yrange [0.00:15.00]
set grid
set xlabel "Time"
set ylabel "Channels"
plot "LabVCR2.dat" using ($1):($6) t"LabVCR2" with lines lt 1
replot "wifi-zone-ufam.dat" using ($1):($6) t"wifi-zone-ufam" with lines lt 2
replot ".dat" using ($1):($6) t"" with lines lt 3
replot "eduroam.dat" using ($1):($6) t"eduroam" with lines lt 4
replot "BDRI.dat" using ($1):($6) t"BDRI" with lines lt 5
replot "ICOMP_1A.dat" using ($1):($6) t"ICOMP_1A" with lines lt 6
replot ".dat" using ($1):($6) t"" with lines lt 7
replot "Android AM.dat" using ($1):($6) t"Android AM" with lines lt 8
replot "ICOMP_TE.dat" using ($1):($6) t"ICOMP_TE" with lines lt 9
replot "ICOMP_1A.dat" using ($1):($6) t"ICOMP_1A" with lines lt 10
replot "eduroam.dat" using ($1):($6) t"eduroam" with lines lt 11
replot "wifi-zone-ufam.dat" using ($1):($6) t"wifi-zone-ufam" with lines lt 12
replot ".dat" using ($1):($6) t"" with lines lt 13
replot "wifi-zone-ufam.dat" using ($1):($6) t"wifi-zone-ufam" with lines lt 14
replot ".dat" using ($1):($6) t"" with lines lt 15
replot "eduroam.dat" using ($1):($6) t"eduroam" with lines lt 16
replot "ICOMP_1A.dat" using ($1):($6) t"ICOMP_1A" with lines lt 17
replot "eduroam.dat" using ($1):($6) t"eduroam" with lines lt 18
replot "wifi-zone-ufam.dat" using ($1):($6) t"wifi-zone-ufam" with lines lt 19
replot ".dat" using ($1):($6) t"" with lines lt 20
replot "testeApp.dat" using ($1):($6) t"testeApp" with lines lt 21
replot "ICOMP_TE.dat" using ($1):($6) t"ICOMP_TE" with lines lt 22
replot "wifi-zone-ufam.dat" using ($1):($6) t"wifi-zone-ufam" with lines lt 23
replot ".dat" using ($1):($6) t"" with lines lt 24
replot "wifi-zone-ufam.dat" using ($1):($6) t"wifi-zone-ufam" with lines lt 25
replot "eduroam.dat" using ($1):($6) t"eduroam" with lines lt 26
replot "ICOMP_1A.dat" using ($1):($6) t"ICOMP_1A" with lines lt 27
pause -1
