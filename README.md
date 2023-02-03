# CY-METEO_Imad_et_-Amine


CY Météo 

Par Amine AIT MOUSSA 
et Imad-Eddine BOUAAMRI


Présentation :
Bonjour à vous ! Et merci de bien vouloir utiliser notre programme CY Météo ! Notre programme vous permet d’afficher sous forme de graphiques, des données météo que vous nous fournissez via un fichier en .csv, selon les choix de modes que vous effectuerez.

Exemple de commande d'exécution:
./script .sh <commandes>

Pour avoir de l’aide dans l’utilisation de CY Météo, tapez ./script  - -help


Fonctionnement :
Partie Shell :
Lors de l’exécution du programme, le code shell récupère les commandes entrées par l’utilisateur. De là, le programme sélectionne la ou les colonnes exactes à trier du fichier d’entrée(passé en paramètre par l’utilisateur) selon le ou les modes sélectionnés par l’utilisateur. Ensuite le code shell supprime/adapte/comble les données afin qu’elles soient exploitables par le code c ensuite. Par exemple, le mode “t1” n’utilise  que la température moyenne, minimale, maximale. Si la température minimale est absente, elle est remplacée par la température moyenne. De plus, il procède à des vérifications, comme l’intervalle de temps s’il est donné par l’utilisateur. Toutes ces données transformées ou réaménagées sont écrites dans un fichier temporaire, qui sera utilisé par le code c. Par la suite, le code shell compile le programme c à l’aide d’un “makefile” puis l’exécute avec les bons paramètres. Après l’exécution du code c( qui renvoie une valeur positive ou nulle), la valeur de retour est utilisée pour savoir si le tri dans le nouveau fichier de sortie s’est bien réalisé. Enfin, le GNUPLOT est réalisé pour pouvoir avoir une représentation graphique des données triées.

Partie C :
Le programme reçoit donc un fichier temporaire hérité du programme shell. 

Pour pouvoir fonctionner, le programme C a besoin des informations suivantes :
-un fichier d’entrée (exemple : -f monfichier.txt)
-un fichier de sortie (exemple : -o fichiersort.txt), dans le cas où ce dernier n’existe pas, le programme C le crée
-une option r optionnelle pour pouvoir trier dans l’ordre décroissant
-un mode de tri (exemple : « -t t1 » ou « -t w »)
-un type de tri optionnel (exemple : --abr), si aucun type n’est sélectionné le type « avl » est choisi par défaut

Dans le cas où une des informations n’est pas présentée ou présente plusieurs fois, le programme retourne une valeur d’erreur. Dans le cas où toutes ces informations sont précisées, nous commençons le tri des données du fichier temporaire. Dans le cas général, le programme récupère les données ligne par ligne, qui sont stockées dans des variables d’un type particulier. Ensuite, ces données sont ajoutées soit dans un avl, soit dans un abr, soit dans une liste chaînée contenant ces données, pour les ranger dans l’ordre croissant ou décroissant selon le mode ou le choix. Lorsque toutes les données sont triées, elles sont ensuite récupérées une par une pour être ajoutées dans le fichier de sortie, qui est créé si inexistant. Le programme, après avoir écrit dans le fichier de sortie, libère l'espace alloué par la mémoire. Lorsque le programme s'est terminé, ou qu'il a rencontré une erreur, il renvoie une valeur au programme Shell pour pouvoir ou non utiliser les nouvelles données pour les représenter graphiquement.
