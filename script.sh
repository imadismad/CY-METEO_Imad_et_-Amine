#Parametrage des parametres:

localisation=0
intervalle=0
commune=0

temperature=0
pression=0
vent=0
altitude=0
humidite=0

val_min=0
val_max=30000000

comm_min=0
comm_max=100000

tri_decroissant=0
tri=0
type_tri="--avl"

sens_tri=""
executable_c=prog_tri

echo -e "\nBienvenue sur:\n"
echo "
░█▀▀█ ░█──░█ ── ░█▀▄▀█ ░█▀▀▀ ▀▀█▀▀ ░█▀▀▀ ░█▀▀▀█ 
░█─── ░█▄▄▄█ ▀▀ ░█░█░█ ░█▀▀▀ ─░█── ░█▀▀▀ ░█──░█ 
░█▄▄█ ──░█── ── ░█──░█ ░█▄▄▄ ─░█── ░█▄▄▄ ░█▄▄▄█"

echo -e "\nFait par: Amine AIT MOUSSA\n          Imad-Eddine BOUAAMRI\nPré-Ing 2\nGP:4\n"

#affichage de l'aide:

Aide()
{
echo -e "Veuillez choisir quelles sont le(s) paramètre(s) à traiter:\n\n     Température         -t <mode> 1 ou 2 ou 3\n     Pression atmos.     -p <mode> 1 ou 2 ou 3\n     Vent                -w\n     Humidité            -m\n     Altitude            -h\n\n"
echo -e "Veuillez choisir, en option, la région des données à traiter:\n\n     (F)rance                        -F\n     (G)uyane française.             -G\n     (S)aint-Pierre et Miquelon      -S\n     (A)ntilles                      -A\n     (O)céan indien                  -O\n     antarcti(Q)ue                   -Q\n\n"
echo -e "Veuillez choisir, en option, l'intervalle temporel des données à traiter:\n\n     (d)ates         -d <min>..<max> (YYYY-MM-DD)\n\n"
echo -e "Veuillez choisir, en option, l'intervalle geographique suivant les communes des données à traiter:\n\n     (c)ommunes         -c <min>..<max>\n\n"
echo -e "Veuillez choisir, en option, l'intervalle geographique suivant les coordonnées des données à traiter:\n\n     (l)atitudes         -l <min>..<max>\n\n"
echo -e "Veuillez choisir, en option, l'intervalle geographique suivant les coordonnées des données à traiter:\n\n     lon(g)itudes         -g <min>..<max>\n\n"

}

droit(){
    chmod 777 "$1"
}

execution_gnuplot (){
    
    case "$1" in
        t1)  
            droit "gnuT1.sh"
            ./gnuT1.sh "$2"
            ;;

        t2)  
            droit "gnuT2.sh"
            ./gnuT2.sh "$2"
            ;;

        t3)  
            droit "gnuT3.sh"
            ./gnuT3.sh "$2"
            ;;

        p1)  
            droit "gnuP1.sh"
            ./gnuP1.sh "$2"
            ;;
        p2)  
            droit "gnuP2.sh"
            ./gnuP2.sh "$2"
            ;;
        p3)  
            droit "gnuP3.sh"
            ./gnuP3.sh "$2"
            ;;
        w)  
            droit "gnuW.sh"
            ./gnuW.sh "$2"
            ;;
        h)  
            droit "gnuH.sh"
            ./gnuH.sh "$2"
            ;;
        m)  
            droit "gnuM.sh"
            ./gnuM.sh "$2"
            ;;
    esac

}

verification_val_retour_c (){
    case "$1" in
        0)
        echo -e "\nTri réalisé avec succès\n"
        ;;

        1)
        echo -e "\nErreur sur les options activées (mauvaise combinaison, option obligatoire manquante…)\n"
        ;;

        2)
        echo -e "\nErreur avec le fichier de données d'entrée (impossible de l'ouvrir, impossible de lire le contenu, format de données incorrect…)\n"
        ;;

        3)
        echo -e "\nErreur avec le fichier de données de sortie (impossible de l'ouvrir, d'écrire dedans…)\n"
        ;;
        
        4)
        echo -e "\nErreur d'exécution interne\n"
        ;;
    esac    
}

execution_c(){
    ./"$executable_c" "-f" "$1" "-o" "$2" "$type_tri" "-t" "$parametre"
}

droit_et_execution_c_et_verif_retour_gnuplot(){
    droit "$executable_c"
    echo -e "Execution du tri en $type_tri"
    execution_c "$1" "$2"
    val_retour_c="$?"
    verification_val_retour_c "$val_retour_c"
    execution_gnuplot "$parametre" "$2"
}

verification_existence_executable_puis_compilation(){
    if [ -f "$executable_c" ]
    then
        droit_et_execution_c_et_verif_retour_gnuplot "$1" "$2"
    else
     make
     verification_existence_executable_puis_compilation "$1" "$2"
    fi
}

verification_intervalle(){
    if [ "$intervalle" -eq 0 ] #cas ou il n'y a pas d'intervalle
    then
        mv "$1" "$2" #renommer le fichier temporaire en fichier d'entrée pour le programme c
    else  
        echo -e "Filtrage selon l'intervalle selectionné:\n" 
        awk  -F';' '{ if ($1 >= "'"$val_min"'" && $1 <= "'"$val_max"'" ) { print $0 }}' <"$1"> "$2" #comparaison de la date sous forme de grand nombre
        rm "$1" #supression du ficher temporaire
    fi
}

###################################################################################################
verification_champs_vide_mode2 (){
    awk -F'[;]' '/;;/ {print $1";"$2";"$2";"$2";"$4}' <"$1"> "$2"
    rm "$1"
}
verification_champs_vide_mode1 (){
    awk -F'[;]' '/;;/ {print $1";"$2";"$2";"$2";"$5}' <"$1"> "$2"
    rm "$1"
}
verification_champs_vide (){
    awk -F'[;]' '! /;;/ {print $0}' <"$1"> "$2"
    rm "$1"
}

filtre_mode() {        
    echo -e "Filtrage selon le(s) mode(s) sélectionné(s):\n"

                if [ $temperature -eq "1" ]
                then
                    parametre="t1"  
                    echo -e "-$parametre: température 1\n"
                    
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$12";"$13";"$14";"$1}' <"$file"> Dossier_Temporaire/tempt1-temp1.txt
                    verification_champs_vide_mode1 Dossier_Temporaire/tempt1-temp1.txt Dossier_Temporaire/tempt1-temp.txt
                    verification_intervalle Dossier_Temporaire/tempt1-temp.txt Dossier_Temporaire/tempt1.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempt1.txt Dossier_Temporaire/outputt1.txt


                elif [ $temperature -eq "2" ] 
                then
                    parametre="t2"  
                    echo -e "$parametre: température 2\n"
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$12";"$1}' <"$file"> Dossier_Temporaire/tempt2-temp1.txt
                    verification_champs_vide Dossier_Temporaire/tempt2-temp1.txt Dossier_Temporaire/tempt2-temp.txt
                    verification_intervalle Dossier_Temporaire/tempt2-temp.txt Dossier_Temporaire/tempt2.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempt2.txt Dossier_Temporaire/outputt2.txt

                elif [ $temperature -eq "3" ] 
                then
                    parametre="t3"
                    echo -e "$parametre: température 3\n"
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$12";"$1}' <"$file"> Dossier_Temporaire/tempt3-temp1.txt
                    verification_champs_vide Dossier_Temporaire/tempt3-temp1.txt Dossier_Temporaire/tempt3-temp.txt
                    verification_intervalle Dossier_Temporaire/tempt3-temp.txt Dossier_Temporaire/tempt3.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempt3.txt Dossier_Temporaire/outputt3.txt

                fi

                if [ $pression -eq "1" ]
                then
                    parametre="p1"
                    echo -e "$parametre: pression 1\n"  
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$7";"$8";"$1}' <"$file"> Dossier_Temporaire/tempp1-temp1.txt
                    verification_champs_vide_mode2 Dossier_Temporaire/tempp1-temp1.txt Dossier_Temporaire/tempp1-temp.txt
                    verification_intervalle Dossier_Temporaire/tempp1-temp.txt Dossier_Temporaire/tempp1.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempp1.txt Dossier_Temporaire/outputp1.txt
            
                elif [ $pression -eq "2" ] 
                then
                    parametre="p2"
                    echo -e "$parametre: pression 2\n"
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$7";"$1}' <"$file"> Dossier_Temporaire/tempp2-temp1.txt
                    verification_champs_vide Dossier_Temporaire/tempp2-temp1.txt Dossier_Temporaire/tempp2-temp.txt
                    verification_intervalle Dossier_Temporaire/tempp2-temp.txt Dossier_Temporaire/tempp2.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempp2.txt Dossier_Temporaire/outputp2.txt

                elif [ $pression -eq "3" ] 
                then
                    parametre="p3"
                    echo -e "$parametre: pression 3\n"
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$7";"$1}' <"$file"> Dossier_Temporaire/tempp3-temp1.txt
                    verification_champs_vide Dossier_Temporaire/tempp3-temp1.txt Dossier_Temporaire/tempp3-temp.txt
                    verification_intervalle Dossier_Temporaire/tempp3-temp.txt Dossier_Temporaire/tempp3.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempp3.txt Dossier_Temporaire/outputp3.txt

                fi

                if [ $vent -eq "1" ]
                then
                    parametre="w"
                    echo -e "$parametre: vent\n" 
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max){print $2";"$10";"$11";"$4";"$5";"$1}' <"$file"> Dossier_Temporaire/tempw-temp1.txt
                    verification_champs_vide Dossier_Temporaire/tempw-temp1.txt Dossier_Temporaire/tempw-temp.txt
                    verification_intervalle Dossier_Temporaire/tempw-temp.txt Dossier_Temporaire/tempw.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempw.txt Dossier_Temporaire/outputw.txt
                fi

                if [ $altitude -eq "1" ]
                then
                    parametre="h"
                    echo -e "$parametre: altitude\n"

                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 > lat_min && $10+0 < lat_max && $11+0 < lon_max && $11+0 > lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$10";"$11";"$15";"$1}' <"$file"> Dossier_Temporaire/temph-temp1.txt
                    verification_champs_vide Dossier_Temporaire/temph-temp1.txt Dossier_Temporaire/temph-temp.txt
                    verification_intervalle Dossier_Temporaire/temph-temp.txt Dossier_Temporaire/temph.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/temph.txt Dossier_Temporaire/outputh.txt
                fi

                if [ $humidite -eq "1" ]
                then
                    parametre="m"
                    echo -e "$parametre: humidité\n"  
                    awk -F'[;,]' -v lat_min="$lat_min" -v lat_max="$lat_max" -v lon_min="$lon_min" -v lon_max="$lon_max" -v comm_min=$comm_min -v comm_max=$comm_max '{NR > 1 gsub(/-/,"",$2)} $10+0 >= lat_min && $10+0 <= lat_max && $11+0 <= lon_max && $11+0 >= lon_min || ($16 > comm_min && $16 < comm_max) {print $2";"$10";"$11";"$6";"$1}' <"$file"> Dossier_Temporaire/tempm-temp1.txt
                    verification_champs_vide Dossier_Temporaire/tempm-temp1.txt Dossier_Temporaire/tempm-temp.txt
                    verification_intervalle Dossier_Temporaire/tempm-temp.txt Dossier_Temporaire/tempm.txt
                    verification_existence_executable_puis_compilation Dossier_Temporaire/tempm.txt Dossier_Temporaire/outputm.txt
                fi
}

verif_commune(){
    if (($commune == 1))
    then
        lat_min="0"
        lat_max="0"
        lon_min="0"
        lon_max="0"
    fi
}

verif_mode(){
    if (( ($temperature == 0) && ($pression == 0) && ($altitude == 0) && ($humidite == 0) && ($vent == 0) )) 
    then
        echo -e "Erreur pas de mode selectionné"
        exit
    fi
}

selection_localisation(){
    verif_mode
    mkdir Dossier_Temporaire
    case "$localisation" in

        0) #Tout le territoire
            echo -e "Données du Monde Entier\n"
            lat_min="-90"
            lat_max="90"
            lon_min="-180"
            lon_max="180"
            verif_commune
            ;;

        1) #France metropolitaine
            echo -e "Données de la France metropolitaine\n"
            lat_min="40"
            lat_max="52"
            lon_min="-5"
            lon_max="10"
            comm_min="5046"
            comm_max="91027"
            verif_commune
            ;;

        2) #Guyane
            echo -e "Données de la Gyuane\n"
            lat_min="2"
            lat_max="6.2"
            lon_min="-54"
            lon_max="-52"
            comm_min="97307"
            comm_max="97353"
            verif_commune
            ;;

        3) #Saint-Pierre et Miquelon
            echo -e "Données de Saint-Pierre et Miquelon\n"
            lat_min="46.0"
            lat_max="47.0"
            lon_min="-57.0"
            lon_max="-56.0"
            verif_commune
            ;;

        4) #Antilles
            echo -e "Données des Antilles\n"
            lat_min="14"
            lat_max="17"
            lon_min="-62.9"
            lon_max="-60"
            verif_commune
            ;;

        5) #Ocean Indien
            echo -e "Données de l'Ocean Indien\n"
            lat_min="-50"
            lat_max="-10"
            lon_min="40"
            lon_max="78"
            comm_min="97418"
            comm_max="97615" 
            verif_commune
            ;;

        6) #Antartique
            echo -e "Données de l'Antartique\n"
            lat_min="-67"
            lat_max="-66"
            lon_min="139"
            lon_max="141"
            verif_commune
            ;;
        
        7) #la zone personalisée entrée avec des latitudes et longitudes
            echo -e "Données d'une zone personalisée -a $lat_min $lat_max -g $lon_min $lon_max\n"
            verif_commune
            ;;
        
        8) #la zone personalisée entrée avec le code comunes
            echo -e "Données d'une zone personalisée -c $comm_min $comm_max\n"
            verif_commune
            ;;

    esac
    filtre_mode

}

verif_fichier_data(){
    if [ -f "$1" ]
    then
        file="$1"
    else
        echo "Erreur: fichier donnée renseigné non trouvé"
        exit
    fi
}

verification_tri_argument (){
    if (($tri != 0))
    then
        echo -e "\nErreur: La séléction du mode de tri est exclusive\n"
        exit
    else
        type_tri="--avl"
    fi
}

verification_localisation_argument () {
    if (($localisation != 0))
    then
        echo -e "\nErreur: La séléction de la zone geographique est exclusive\n"
        exit
    fi
}



#Programme principal:

for arg in "$@"; do
  shift
  case "$arg" in
    '--help')   set -- "$@" '-m'   ;;
    '--avl') set -- "$@" '-l'   ;;
    '--abr')   set -- "$@" '-e'   ;;
    '--tab')     set -- "$@" '-b'   ;;
    *)          set -- "$@" "$arg" ;;
  esac
done


while getopts ":t:welrbmzrhFGSAOQc:a:g:d:p:f:" option;
do
    case "$option" in
        
        t) #temperature

            if [ $OPTARG == 1 ]
            then
                temperature=1

            elif [ $OPTARG == 2 ] 
            then
                temperature=2

            elif [ $OPTARG == 3 ] 
            then
                temperature=3
            else
                echo "Erreur dans la précision du mode de la température"
            fi
            ;;

        p) #pression
            if [ $OPTARG == 1 ]
            then
                pression=1

            elif [ $OPTARG == 2 ] 
            then
                pression=2

            elif [ $OPTARG == 3 ] 
            then
                pression=3
            else
                echo "Erreur dans la précision du mode de la pression atmosphérique"
            fi
            ;;

        w) #vent
            vent=1
            ;;
        m) #Humidité
            humidite=1
            ;;

        h) #altitude
            altitude=1
            ;;
    
        f) #fichier base de données
            unset IFS
            verif_fichier_data "$OPTARG"
            ;;

        l) #avl
            verification_tri_argument
            tri=1
            type_tri="--avl"
            ;;

        e) #abr
            verification_tri_argument
            tri=2
            type_tri="--abr"
            ;;

        b) #tab
            verification_tri_argument
            tri=3
            type_tri="--tab"
            ;;
        r)
            tri_decroissant=1
            sens_tri="-r"
            ;;

        d) #date
            set -f # disable glob
            IFS='.-'   # split on space characters
            array=($OPTARG) # use the split+glob operator
            val_min="${array[0]}""${array[1]}""${array[2]}"
            val_max="${array[4]}""${array[5]}""${array[6]}"
            unset IFS
            if [ $val_min -le $val_max ]
            then
                intervalle=1
            else
                echo "Erreur de dates"
                exit;
            fi
            ;;

        F) #France
            verification_localisation_argument
            localisation=1
            ;;

        G) #Guyane
            verification_localisation_argument
            localisation=2
            ;;

        S) #Spm
            verification_localisation_argument
            localisation=3
            ;;

        A) #Antilles
            verification_localisation_argument
            localisation=4
            ;;

        O) #Oi
            verification_localisation_argument
            localisation=5
            ;;

        Q) #Antartique
            verification_localisation_argument
            localisation=6
            ;;
        
        a) #latitude
            verification_localisation_argument
            set -f # disable glob
            IFS='.'   # split on space characters
            array=($OPTARG) # use the split+glob operator
            lat_min="${array[0]}"
            lat_max="${array[2]}"
            unset IFS
            if [ $(bc <<< "$lat_min <= $lat_max") -eq 1 ]
            then
                latitude=1
                localisation=7

            else
                echo "Erreur de latitude"
                exit;            
            fi
            ;;
        
        g) #longitude
            verification_localisation_argument
            set -f # disable glob
            IFS='.'   # split on space characters
            array=($OPTARG) # use the split+glob operator
            lon_min="${array[0]}"
            lon_max="${array[2]}"
            unset IFS
            if [ $(bc <<< "$lon_min <= $lon_max") -eq 1 ]
            then
                longitude=1
                localisation=7

            else
                echo "Erreur de longitude"
                exit;
            fi            
            ;;
        c) #commune
            verification_localisation_argument
            set -f # disable glob
            IFS='.'   # split on space characters
            array=($OPTARG) # use the split+glob operator
            comm_min="${array[0]}"
            comm_max="${array[2]}"
            unset IFS
            if [ $(bc <<< "$comm_min <= $comm_max") -eq 1 ]
            then
                commune=1
                localisation=8

            else
                echo "Erreur de code commune"
                exit;
            fi            
            ;;


        z) # display Help
            Aide
            exit
            ;;

        :)
            echo "L'option $OPTARG requiert un argument"
            exit 1
            ;;

        \?) # Invalid option
            echo "Erreur: Options invalides"
            exit;;
    esac

done

shift $((OPTIND-1))


if [ $OPTIND -eq 1 ]; then 
    echo -e "Erreur: Aucune option n'a été selectionnée \n" 
    exit
fi



selection_localisation


#rm -rf Dossier_Temporaire

echo "Hello world!"
