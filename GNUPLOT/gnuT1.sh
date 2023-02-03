#fichier de données
data_file="$1"

#Titre et labels des axes
title="Températures: moyenne, minimale et maximale en fonction de la station"
xlabel="ID Station"
ylabel="Température en °C"

#nom fichier sortie
graph="T1.png"

#Creation du script gnuplot
gnuplot_script=$(cat <<EOF

#preparation de la lecture du fichier delimité par ;
set datafile separator ';'

#forme et esthetique
set terminal png enhanced font "arial,11" fontscale 1.0 size 1920, 1080
set title font "arial,18"

#bord et legende
set border 3
set mytics 5
set mxtics 10
set key outside
set tics out nomirror

set output '$graph'

set title "$title"
set xlabel "$xlabel"
set ylabel "$ylabel"


Shadecolor = "#80E0A080"

plot "$data_file" using 1:3:4 \
     with filledcurve fc rgb Shadecolor title "Marge Min. / Max.",\
     '' using 1:2 smooth mcspline lw 2 title "Température moyenne"



EOF
)

# Run le script gnuplot
echo "$gnuplot_script" | gnuplot

# Ouvrir le graph créé
xdg-open "$graph"

