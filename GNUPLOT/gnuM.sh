#fichier de données
data_file="$1"
echo -e "Gnuplot des données en cours:"


#Titre et labels des axes
title="Humidité en fonction de la zone géographique"
xlabel="axe Ouest-Est"
ylabel="axe Sud-Nord"

#Nom fichier sortie
graph="M.png"

#Creation du script gnuplot
gnuplot_script=$(cat <<EOF

#preparation de la lecture du fichier delimité par ;
set datafile separator ';'

#forme et esthetique
set terminal png enhanced font "arial,11" fontscale 1.0 size 1920, 1080
set title font "arial,18"

#bord et legende
set border 10
set mytics 5
set mxtics 5
set key outside
set tics out nomirror

set output '$graph'

set title "$title"
set xlabel "$xlabel"
set ylabel "$ylabel"
set zeroaxis linewidth 2 linetype 2


#preparation carte interpolée
set view map
set dgrid3d
set pm3d interpolate 8,8

splot "$data_file" using 3:2:4 with pm3d t "              Humidité en %"

EOF
)

# Run le script gnuplot 
echo "$gnuplot_script" | gnuplot

# Ouvrir le graph créé
xdg-open $graph