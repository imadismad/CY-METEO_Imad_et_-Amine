#fichier de données
data_file="$1"

#Titre et labels des axes
title="Orientation et vitesse moyennes du vent en\n fonction de la zone géographique"
xlabel="axe Ouest-Est"
ylabel="axe Sud-Nord"

#Nom fichier sortie
graph="W.png"

#Creation du script gnuplot
gnuplot_script=$(cat <<EOF

#preparation de la lecture du fichier delimité par ;
set datafile separator ';'

#forme et esthetique
set terminal png enhanced font "arial,11" fontscale 1.0 size 1920, 1080
set title font "arial,18"
set size ratio 1 

#bord et legende
set border 10
set mytics 10
set mxtics 10
set grid mxtics mytics

set key outside
set tics out nomirror

set output '$graph'

set title "$title"
set xlabel "$xlabel"
set ylabel "$ylabel"

#preparation carte et vecteurs
set angle degrees
set zeroaxis linewidth 2 linetype 2

SCALE=0.5

set label 2 "0,2 m/s" at 315,55 

plot "$data_file" using 3:2:(5*SCALE):4 with arrows noborder t "Direction et vitesse du vent en degré et m/s"

EOF
)

# Run le script gnuplot 
echo "$gnuplot_script" | gnuplot

# Ouvrir le graph créé
xdg-open $graph