#fichier de données
data_file="$1"

#Titre et labels des axes
title="Pression atmosphérique moyenne en fonction du jour et l'heure par station"
xlabel="Jour et l'heure"
ylabel="Pression en Pa"

#nom fichier sortie
graph="P3.png"

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
set mxtics 12

set key outside
set tics out nomirror

set output '$graph'

set title font "arial,18"

set title "$title"
set xlabel "$xlabel"
set ylabel "$ylabel"


set xdata time
set timefmt "%Y-%m-%d"
set format x "%Y/%m/%d\n%H:%M"

plot "$data_file" using 2:4:1 with linespoints lc variable notitle
EOF
)

# Run the gnuplot script
echo "$gnuplot_script" | gnuplot
xdg-open "$graph"

