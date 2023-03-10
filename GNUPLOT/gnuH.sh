#Fichier de données 
data_file="$1"

#Titre et labels des axes
title="Altitude en fonction de la zone géographique"
xlabel="axe Est-Ouest"
ylabel="axe Sud-Nord"

#nom fichier output
graph="H.png"

# Creation du script gnuplot
gnuplot_script=$(cat <<EOF

#preparation de la lecture du fichier delimité par ;
set datafile separator ';'

#forme et esthetique
set terminal png enhanced font "arial,11" fontscale 1.0 size 1920, 1080
set title font "arial,18"

#bord et legende
set border 10
set mytics 5
set key outside
set tics out nomirror

set output '$graph'

set title "$title"
set xlabel "$xlabel"
set ylabel "$ylabel"

#preparation carte interpolée
set view map
set dgrid3d
set pm3d interpolate 9,9

splot "$data_file" using 3:2:4 with pm3d t "        Altitude en m"

EOF
)

# Run le script gnuplot 
echo "$gnuplot_script" | gnuplot

# Ouvrir le graph créé

xdg-open $graph

