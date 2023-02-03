typedef struct donneeh{
        int idstation;
        int date;
        int heure;
        int decalage;
        double altitudes;
        double lat, lon;

    } Donneeh;

//structure d'une chaine
typedef struct chaineh{
    Donneeh elmt;
    struct chaineh* suivant;
}Chaineh;



// structure du noeud
typedef struct noeudh{
	Donneeh val;
	struct noeudh *fg;
	struct noeudh *fd;
	int equilibre;
} Noeudh;

typedef struct abrh{
    Donneeh elmt;
    struct abrh* fg; 
    struct abrh* fd;
} ABRh;

Chaineh* creerchainonh(Donneeh b);
Chaineh* ajouterCroissanth(Chaineh *pliste, Donneeh b);
void parcoursInfixeh(Noeudh* a, FILE* f);
void suppravlh( Noeudh* a);
int min(int a,int b);
int max(int a,int b);
int min2(int a,int b, int c);
int max2(int a,int b, int c);
Noeudh* nouveaunoeudh(Donneeh val);
Noeudh* rotationGaucheh(Noeudh* a);
Noeudh* rotationDroiteh(Noeudh* a);
Noeudh* doubleRotationDroiteh(Noeudh* a);
Noeudh* doubleRotationGaucheh(Noeudh* a);
Noeudh* equilibrerAVLh(Noeudh* a);
Noeudh* inserth(Noeudh* a, Donneeh e, int* h);
ABRh* creerArbreh(Donneeh e);
ABRh* insertionABRh(ABRh* a, Donneeh e);
void parcoursInfixeABRh(ABRh* a, FILE* f);
void supprabrh( ABRh* a);
int h(FILE* fe, char tab[], FILE* fo, int r);
typedef struct donneem{
        int idstation;
        int date;
        int heure;
        int decalage;
        double humidite;
        double lon;
        double lat;

    } Donneem;

typedef struct chainem{
    Donneem elmt;
    struct chainem* suivant;
}Chainem;


// structure du noeud
typedef struct noeudm{
	Donneem val;
	struct noeudm *fg;
	struct noeudm *fd;
	int equilibre;
} Noeudm;



typedef struct abrm{
    Donneem elmt;
    struct abrm* fg; 
    struct abrm* fd;
}ABRm;





Chainem* creerchainonm(Donneem b);
Chainem* ajouterCroissantm1(Chainem *pliste, Donneem b);
Chainem* ajouterCroissantm2(Chainem *pliste, Donneem b);
void parcoursInfixem(Noeudm* a, FILE* f);
void suppravlm( Noeudm* a);
Noeudm* nouveaunoeudm(Donneem val);
Noeudm* rotationGauchem(Noeudm* a);
Noeudm* rotationDroitem(Noeudm* a);
Noeudm* doubleRotationDroitem(Noeudm* a);
Noeudm* doubleRotationGauchem(Noeudm* a);
Noeudm* equilibrerAVLm(Noeudm* a);
Noeudm* insertm(Noeudm* a, Donneem e, int* h);
ABRm* creerArbrem(Donneem e);
ABRm* insertionABRm(ABRm* a, Donneem e);
void parcoursInfixeABRm(ABRm* a, FILE* f);
void supprabrm( ABRm* a);
int m(FILE* fe, char tab[], FILE* fo, int r);



//structure des données
typedef struct donnee1{
        int idstation;
        int date;
        int heure;
        int decalage;
        double val;
        double valmin;
        double valmax;

    } Donnee1;


//structure liste chainée
typedef struct chaine1{
    int station;
    double min, max;
    double compteur;
    double somme;
    struct chaine1* suivant;
}Chaine1;

// structure du noeud
typedef struct noeud1{
	Donnee1 val;
    double compteur;
	struct noeud1 *fg;
	struct noeud1 *fd;
	int equilibre;
} Noeud1;

typedef struct abr1{
    Donnee1 elmt;
    double compteur;
    struct abr1* fg, *fd;
}ABR1;

Chaine1* creerchainon1(Donnee1 b);
Chaine1* ajouterCroissant1(Chaine1 *pliste, Donnee1 b);
Chaine1* ajouterCroissant1r(Chaine1 *pliste, Donnee1 b);
void parcoursInfixe1(Noeud1* a, FILE* f);
void parcoursInfixe1r(Noeud1* a, FILE* f);
void suppravl1( Noeud1* a);
Noeud1* nouveaunoeud1(Donnee1 val);
Noeud1* rotationDroite1(Noeud1* a);
Noeud1* rotationGauche1(Noeud1* a);
Noeud1* doubleRotationDroite1(Noeud1* a);
Noeud1* doubleRotationGauche1(Noeud1* a);
Noeud1* equilibrerAVL1(Noeud1* a);
Noeud1* insert1(Noeud1* a, Donnee1 e, int* h);
ABR1* creerArbre1(Donnee1 e);
ABR1* insertionABR1(ABR1* a, Donnee1 e);
ABR1* insertionABR1r(ABR1* a, Donnee1 e);
void parcoursInfixeABR1(ABR1* a, FILE* f);
void supprabr1( ABR1* a);

int mode1(FILE* fe, char tab[], FILE* fo, int r);


typedef struct donnee2{
        int idstation;
        int date;
        int heure;
        int decalage;
        double val;
    } Donnee2;




typedef struct chaine2{
    int date;
    int heure;
    int compteur;
    int somme;
    struct chaine2* suivant;
}Chaine2;


// structure du noeud
typedef struct noeud2{
	int date;
    int heure;
    double somme;
    double compteur;
	struct noeud2 *fg;
	struct noeud2 *fd;
	int equilibre;
} Noeud2;


typedef struct abr2{
    int date;
    int heure;
    double somme;
    double compteur;
    struct abr2* fg, *fd;
}ABR2;


void fonction_date2(int a, char* z);
Chaine2* creerchainon2(Donnee2 b);
Chaine2* ajouterCroissant2(Chaine2 *pliste, Donnee2 b);
Chaine2* ajouterCroissant2r(Chaine2 *pliste, Donnee2 b);
void parcoursInfixe2(Noeud2* a, FILE* f);
void parcoursInfixe2r(Noeud2* a, FILE* f);
void suppravl2( Noeud2* a);
Noeud2* nouveaunoeud2(Donnee2 val);
Noeud2* rotationGauche2(Noeud2* a);
Noeud2* rotationDroite2(Noeud2* a);
Noeud2* doubleRotationDroite2(Noeud2* a);
Noeud2* doubleRotationGauche2(Noeud2* a);
Noeud2* equilibrerAVL2(Noeud2* a);
Noeud2* insert2(Noeud2* a, Donnee2 e, int* h);
ABR2* creerArbre2(Donnee2 e);
ABR2* insertionABR2(ABR2* a, Donnee2 e);
ABR2* insertionABR2r(ABR2* a, Donnee2 e);
void parcoursInfixeABR2(ABR2* a, FILE* f);
void supprabr2( ABR2* a);
int mode2(FILE* fe, char tab[], FILE* fo, int r);


typedef struct donnee3{
        int idstation;
        double val;
        int date;
        int heure;
        int decalage;
    } Donnee3;




typedef struct chaine31{
    int date;
    int heure;
    double val;
    struct chaine31* suivant;
}Chaine31;


typedef struct chaine30{
    int idstation;
    Chaine31* chaine;
    struct chaine30* suivant;
}Chaine30;



// structure du noeud
typedef struct noeud3{
	int idstation;
    Chaine31* chaine;
	struct noeud3 *fg;
	struct noeud3 *fd;
	int equilibre;
} Noeud3;


typedef struct abr3{
    Chaine31* chaine;
    int idstation;
    struct abr3* fg; 
    struct abr3* fd;
}ABR3;


void fonction_date3(int a, char* z);
Chaine31 *creerchainon31(Donnee3 b);
Chaine30* creerchainon30(Donnee3 b);
Chaine31* ajouterCroissant31(Chaine31 *pliste, Donnee3 b);
Chaine30* ajouterCroissant30(Chaine30* pliste, Donnee3 b);
Chaine31* ajouterCroissant31r(Chaine31 *pliste, Donnee3 b);
Chaine30* ajouterCroissant30r(Chaine30* pliste, Donnee3 b);
void parcoursInfixe3(Noeud3* a, FILE* f);
void suppravl3( Noeud3* a);
Noeud3* nouveaunoeud3(Donnee3 val);
Noeud3* rotationGauche3(Noeud3* a);
Noeud3* rotationDroite3(Noeud3* a);
Noeud3* doubleRotationDroite3(Noeud3* a);
Noeud3* doubleRotationGauche3(Noeud3* a);
Noeud3* equilibrerAVL3(Noeud3* a);
Noeud3* insert3(Noeud3* a, Donnee3 e, int* h);
Noeud3* insert3r(Noeud3* a, Donnee3 e, int* h);
ABR3* creerArbre3(Donnee3 e);
ABR3* insertionABR3(ABR3* a, Donnee3 e);
ABR3* insertionABR3r(ABR3* a, Donnee3 e);
void parcoursInfixeABR3(ABR3* a, FILE* f);
void supprabr3( ABR3* a);
int mode3(FILE* fe, char tab[], FILE* fo, int r);

typedef struct vent{
        double direction;
        double vitesse;
    } Vent;

typedef struct donneew{
        int idstation;
        int date;
        int heure;
        int decalage;
        Vent vent;
        double x;
        double y;

    } Donneew;

typedef struct chainew{
    Donneew val;
    double compteur;
    struct chainew* suivant;
}Chainew;

// structure du noeud
typedef struct noeudw{
	Donneew val;
	struct noeudw *fg;
	struct noeudw *fd;
    double compteur;
	int equilibre;
} Noeudw;


typedef struct abrw{
    Donneew val;
    double compteur;
    struct abrw* fg;
    struct abrw* fd;
}ABRW;


Chainew* creerchainonw(Donneew b);
Chainew* ajouterCroissantw(Chainew *pliste, Donneew b);
Chainew* ajouterCroissantwr(Chainew *pliste, Donneew b);
void parcoursInfixew(Noeudw* a, FILE* f);
void parcoursInfixewr(Noeudw* a, FILE* f);
void suppravlw( Noeudw* a);
Noeudw* nouveaunoeudw(Donneew a);
Noeudw* rotationGauchew(Noeudw* a);
Noeudw* rotationDroitew(Noeudw* a);
Noeudw* doubleRotationDroitew(Noeudw* a);
Noeudw* doubleRotationGauchew(Noeudw* a);
Noeudw* equilibrerAVLw(Noeudw* a);
Noeudw* insertw(Noeudw* a, Donneew e, int* h);
ABRW* creerArbrew(Donneew b);
ABRW* insertionABRW(ABRW* a, Donneew e);
ABRW* insertionABRWr(ABRW* a, Donneew e);
void parcoursInfixeABRW(ABRW* a, FILE* f);
void supprabrw( ABRW* a);
int w(FILE* fe, char tab[], FILE* fo, int r);


//structure des données
typedef struct donneep1{
        int idstation;
        int date;
        int heure;
        int decalage;
        double val, valmin, valmax;
    } Donneep1;


//structure liste chainée
typedef struct chainep1{
    int station;
    double min, max;
    double compteur;
    double somme;
    struct chainep1* suivant;
}Chainep1;

// structure du noeud
typedef struct noeudp1{
	Donneep1 val;
    double compteur;
	struct noeudp1 *fg;
	struct noeudp1 *fd;
	int equilibre;
} Noeudp1;


typedef struct abrp1{
    Donneep1 elmt;
    double compteur;
    struct abrp1* fg, *fd;
}ABRp1;

Chainep1* creerchainonp1(Donneep1 b);
Chainep1* ajouterCroissantp1(Chainep1 *pliste, Donneep1 b);
Chainep1* ajouterCroissantp1r(Chainep1 *pliste, Donneep1 b);
void parcoursInfixep1(Noeudp1* a, FILE* f);
void parcoursInfixep1r(Noeudp1* a, FILE* f);
void suppravlp1( Noeudp1* a);
Noeudp1* nouveaunoeudp1(Donneep1 val);
Noeudp1* rotationDroitep1(Noeudp1* a);
Noeudp1* rotationGauchep1(Noeudp1* a);
Noeudp1* doubleRotationDroitep1(Noeudp1* a);
Noeudp1* doubleRotationGauchep1(Noeudp1* a);
Noeudp1* equilibrerAVLp1(Noeudp1* a);
Noeudp1* insertp1(Noeudp1* a, Donneep1 e, int* h);
ABRp1* creerArbrep1(Donneep1 e);
ABRp1* insertionABRp1(ABRp1* a, Donneep1 e);
ABRp1* insertionABRp1pr(ABRp1* a, Donneep1 e);
void parcoursInfixeABRp1(ABRp1* a, FILE* f);
void supprabrp1( ABRp1* a);

int modep1(FILE* fe, char tab[], FILE* fo, int r);