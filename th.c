#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include "th.h"

// fonctions pour avoir le maximum et le minimum
int min(int a,int b){
	if(a<=b){
		return a;
	}
	else{
		return b;
	}
}
int max(int a,int b){
	if(a>=b){
		return a;
	}
	else{
		return b;
	}
}
int min2(int a,int b, int c){
	if(a<=b && a<=c){
		return a;
	}
	else if(b<=a && b<=c){
		return b;
	}
	else{
		return c;
	}
}


int max2(int a,int b, int c){
	if(a>=b && a>=c){
		return a;
	}
	else if(b>=a && b>=c){
		return b;
	}
	else{
		return c;
	}
}
//fonction de creation de chainon
Chaineh* creerchainonh(Donneeh b){
    Chaineh* a=malloc(sizeof(Chaineh));
    a->elmt.idstation=b.idstation;
    a->elmt.altitudes=b.altitudes;
    a->elmt.lat=b.lat;
    a->elmt.lon=b.lon;
    a->suivant=NULL;
    return a;
}
//fonction d'ajout d'un chainon dans une chaine
Chaineh* ajouterCroissanth(Chaineh *pliste, Donneeh b){
    Chaineh *nouveau=creerchainonh(b);
	Chaineh *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->elmt.altitudes < b.altitudes){ // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->elmt.altitudes > b.altitudes){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if(p1->suivant->elmt.altitudes == b.altitudes){
            return pliste;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//Parcours de l'arbre et ecriture dans le fichier de sortie
void parcoursInfixeh(Noeudh* a, FILE* f){
    if(a!=NULL){
        parcoursInfixeh(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf\n",a->val.idstation, a->val.lat, a->val.lon, a->val.altitudes);
        parcoursInfixeh(a->fd, f);
	}
}
//suppression de l'avl
void suppravlh( Noeudh* a){
    if(a->fd!=NULL){
        suppravlh(a->fd);
	}
    if(a->fg!=NULL){
        suppravlh(a->fg);
	}
    free(a);
}
//fonction qui cree un nouveau noeud
Noeudh* nouveaunoeudh(Donneeh val){
	Noeudh*  noeud= malloc(sizeof(Noeudh));
	noeud->val.lat = val.lat;
    noeud->val.lon = val.lon;
    noeud->val.altitudes = val.altitudes;
	noeud->fg = NULL;
	noeud->fd = NULL;
	noeud->equilibre = 0; 
	return noeud;
}
//fonction pour effectuer la rotation gauche
Noeudh* rotationGaucheh(Noeudh* a){
	Noeudh* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer la rotation droite
Noeudh* rotationDroiteh(Noeudh* a){
	Noeudh* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer une double rotation droite
Noeudh* doubleRotationDroiteh(Noeudh* a){
	a->fg= rotationGaucheh(a->fg);
	return rotationDroiteh(a);
}

//fonction pour effectuer une double rotation gauche
Noeudh* doubleRotationGaucheh(Noeudh* a){
	a->fd= rotationDroiteh(a->fd);
	return rotationGaucheh(a);
}
//fonction pour equilibrer l'avl
Noeudh* equilibrerAVLh(Noeudh* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGaucheh(a);
		}
		else{
			return doubleRotationGaucheh(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroiteh(a);
		}
		else{
			return doubleRotationDroiteh(a);
		}
	}
	return a;
}
// fonction pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeudh* inserth(Noeudh* a, Donneeh e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeudh(e);
	}
	else if(e.altitudes<a->val.altitudes){
		a->fg=inserth(a->fg,e,h);
		*h= - *h;
	}
	else if(e.altitudes>a->val.altitudes){
		a->fd=inserth(a->fd,e,h);
	}
	else{
		*h=0;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVLh(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creation d'un noeud de l'abr
ABRh* creerArbreh(Donneeh e){
    ABRh* a=malloc(sizeof(ABRh));
	a->elmt.lat = e.lat;
    a->elmt.lon = e.lon;
    a->elmt.altitudes = e.altitudes;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}
//insertion des elements dans l'abr
ABRh* insertionABRh(ABRh* a, Donneeh e){
    if(a==NULL){
        return creerArbreh(e); 
	}
    else if(e.altitudes<a->elmt.altitudes){
        a->fg= insertionABRh(a->fg, e);
	}
    else if(e.altitudes>a->elmt.altitudes){
        a->fd= insertionABRh(a->fd, e);
	}
    else{
		return a;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABRh(ABRh* a, FILE* f){
    if(a->fg!=NULL){
        parcoursInfixeABRh(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf\n",a->elmt.idstation, a->elmt.lat, a->elmt.lon, a->elmt.altitudes);
        parcoursInfixeABRh(a->fd, f);
	}
}
//suppression de l'abr 
void supprabrh( ABRh* a){
    if(a->fd!=NULL){
        supprabrh(a->fd);
	}
    if(a->fg!=NULL){
        supprabrh(a->fg);
	}
    free(a);
}
//fonction pour le mode h
int h(FILE* fe, char tab[], FILE* fo, int r){ 
    Donneeh b;
    FILE* f1=NULL;
    int* h;
	Chaineh* a=NULL, *c;
	Noeudh* a2=NULL;
	ABRh* a3=NULL;
    if(strcmp(tab,"tab")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date), &(b.heure),&(b.decalage),&(b.lat), &(b.lon), &(b.altitudes),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			a=ajouterCroissanth(a, b);
        }
        while(a!=NULL){
            fprintf(fo, "%d;%lf;%lf;%lf\n",a->elmt.idstation, a->elmt.lat, a->elmt.lon, a->elmt.altitudes);
            c=a;
            a=a->suivant;
            free(c);
        }
        fclose(f1);
        return 0;   
    }
    else if(strcmp(tab,"avl")==0){ 
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date), &(b.heure),&(b.decalage),&(b.lat), &(b.lon), &(b.altitudes),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			a2=inserth(a2, b, h);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeh(a2, fo);
        //suppression de l'avl
        suppravlh(a2);
        return 0;
    }
    else{
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date), &(b.heure),&(b.decalage),&(b.lat), &(b.lon), &(b.altitudes),&(b.idstation)) != EOF){	
		//on ajoute les elements un à un dans la chaine pointée par a
		a3=insertionABRh(a3, b);	
        }
        //ecriture dans le fichier de sortie et parcours de l'abr
        parcoursInfixeABRh(a3, fo);
        //suppression de l'abr
        supprabrh(a3);
		return 0;
    }
    return 1;
}
//fonction creation d'un chainon
Chainem* creerchainonm(Donneem b){
    Chainem* a=malloc(sizeof(Chainem));
    a->elmt.idstation=b.idstation;
    a->elmt.humidite=b.humidite;
    a->elmt.lat=b.lat;
    a->elmt.lon=b.lon;
    a->suivant=NULL;
    return a;
}
//mettre à jour l'humidite max pour chaque station
Chainem* ajouterCroissantm1(Chainem *pliste, Donneem b){
	Chainem *nouveau=creerchainonm(b);
	Chainem *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->elmt.idstation > b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->elmt.idstation < b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->elmt.idstation == b.idstation){//quand la station est deja enregistree
            if(b.humidite > p1->suivant->elmt.humidite)
                p1->suivant->elmt.humidite=b.humidite;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//ajout des stations dans le chainon
Chainem* ajouterCroissantm2(Chainem *pliste, Donneem b){
	Chainem *nouveau=creerchainonm(b);
	Chainem *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->elmt.humidite < b.humidite) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->elmt.humidite > b.humidite){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//parcours de l'avl et ecriture dans le fichier de sortie
void parcoursInfixem(Noeudm* a, FILE* f){
    if(a!=NULL){
        parcoursInfixem(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf\n",a->val.idstation, a->val.lat, a->val.lon, a->val.humidite);
        parcoursInfixem(a->fd, f);
	}
}
//suppression de l'avl
void suppravlm( Noeudm* a){
    if(a->fd!=NULL)
        suppravlm(a->fd);
    if(a->fg!=NULL)
        suppravlm(a->fg);
    free(a);
}
//fonction qui cree un nouveau noeud
Noeudm* nouveaunoeudm(Donneem val){
	Noeudm*  noeud= malloc(sizeof(Noeudm));
	noeud->val.idstation = val.idstation;
    noeud->val.humidite= val.humidite;
    noeud->val.lat = val.lat;
    noeud->val.lon = val.lon;
	noeud->fg = NULL;
	noeud->fd = NULL;
	noeud->equilibre = 0; 
	return noeud;
}
//fonction pour effectuer la rotation gauche
Noeudm* rotationGauchem(Noeudm* a){
	Noeudm* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}

//fonction pour effectuer la rotation droite
Noeudm* rotationDroitem(Noeudm* a){
	Noeudm* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a+2, eq_a+eq_p+2, eq_p+1);
	a=pivot;
	return a;
}

//fonction pour effectuer une double rotation droite
Noeudm* doubleRotationDroitem(Noeudm* a){
	a->fg= rotationGauchem(a->fg);
	return rotationDroitem(a);
}

//fonction pour effectuer une double rotation gauche
Noeudm* doubleRotationGauchem(Noeudm* a){
	a->fd= rotationDroitem(a->fd);
	return rotationGauchem(a);
}

//fonction pour equilibrer l'avl
Noeudm* equilibrerAVLm(Noeudm* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGauchem(a);
		}
		else{
			return doubleRotationGauchem(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroitem(a);
		}
		else{
			return doubleRotationDroitem(a);
		}
	}
	return a;
}
// fonction pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeudm* insertm(Noeudm* a, Donneem e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeudm(e);
	}
	else if(e.humidite<a->val.humidite){
		a->fg=insertm(a->fg,e,h);
		*h= - *h;
	}
	else if(e.humidite>a->val.humidite){
		a->fd=insertm(a->fd,e,h);
	}
	else{
		*h=0;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVLm(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creation d'un noeud de l'abr
ABRm* creerArbrem(Donneem e){
    ABRm* a=malloc(sizeof(ABRm));
	a->elmt.idstation = e.idstation;
    a->elmt.humidite= e.humidite;
    a->elmt.lat = e.lat;
    a->elmt.lon = e.lon;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}
//insertion des elements dans l'abr
ABRm* insertionABRm(ABRm* a, Donneem e){
    if(a==NULL)
        return creerArbrem(e); 
    else if(e.humidite<a->elmt.humidite) 
        a->fg= insertionABRm(a->fg, e);
    else if(e.humidite>a->elmt.humidite)
        a->fd= insertionABRm(a->fd, e);
    else{
		return a;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABRm(ABRm* a, FILE* f){
    if(a->fg!=NULL){
        parcoursInfixeABRm(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf\n",a->elmt.idstation, a->elmt.lat, a->elmt.lon, a->elmt.humidite);
        parcoursInfixeABRm(a->fd, f);
	}
}
//suppression de l'abr
void supprabrm( ABRm* a){
    if(a->fd!=NULL)
        supprabrm(a->fd);
    if(a->fg!=NULL)
        supprabrm(a->fg);
    free(a);
}
//fonction du mode h
int m(FILE* fe, char tab[], FILE* fo, int r){
    Chainem* a=NULL, *abis=NULL, *c;
    Noeudm* aa;
    int* h; 
    Donneem b;
    ABRm* a3=NULL;
    while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.lat), &(b.lon), &(b.humidite),&(b.idstation)) != EOF){
		//on ajoute les elements un à un dans la chaine pointée par a, pour que chaque station soit associee a son humidite max
		a=ajouterCroissantm1(a, b);
        
    }
    if(strcmp(tab,"tab")==0){
        while(a!=NULL){
			//on ajoute dans le chainon et on supprime la premiere chaine
            abis=ajouterCroissantm2(abis, a->elmt);
            c=a;
            a=a->suivant;
            free(c);
        }
        while(abis!=NULL){
			//ecriture dans le fichier de sortie et suppression de abis
            fprintf(fo, "%d;%lf;%lf;%lf\n",abis->elmt.idstation, abis->elmt.lat, abis->elmt.lon, abis->elmt.humidite);
            c=abis;
            abis=abis->suivant;
            free(c);
        }
    return 0;
    }
    else if(strcmp(tab,"avl")==0){
        while(a!=NULL){
			//insertion dans l'avl
            aa=insertm(aa, a->elmt, h);
            c=a;
            a=a->suivant;
            free(c);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixem(aa, fo);
        //suppression de l'avl
        suppravlm(aa);
        return 0;
        
    }
    else{
        while(a!=NULL){
				//on ajoute les elements un à un dans la chaine pointée par a
				a3=insertionABRm(a3, a->elmt);
				c=a;
            	a=a->suivant;
            	free(c);
			
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeABRm(a3, fo);
        //suppression de l'avl
        supprabrm(a3);
		return 0;
    }
    return 1;
}
//créer le premier chainon de la liste chainée
Chaine1* creerchainon1(Donnee1 b){
    Chaine1* a=malloc(sizeof(Chaine1));
    a->station=b.idstation;
    a->min=b.valmin;
    a->max=b.valmax;
    a->somme=b.val;
    a->compteur=1;
    a->suivant=NULL;
    return a;
}
//ajouter les éléments dans la liste chainée
Chaine1* ajouterCroissant1(Chaine1 *pliste, Donnee1 b){
	Chaine1 *nouveau=creerchainon1(b);
	Chaine1 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->station > b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->station < b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->station == b.idstation){//quand la station est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->somme=p1->suivant->somme + b.val;
            if(b.valmax > p1->suivant->max)
                p1->suivant->max=b.valmax;
            if(b.valmin < p1->suivant->min)
                p1->suivant->min=b.valmin;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//version decroissante
Chaine1* ajouterCroissant1r(Chaine1 *pliste, Donnee1 b){
	Chaine1 *nouveau=creerchainon1(b);
	Chaine1 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->station < b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->station > b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->station == b.idstation){//quand la station est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->somme=p1->suivant->somme + b.val;
            if(b.valmax > p1->suivant->max)
                p1->suivant->max=b.valmax;
            if(b.valmin < p1->suivant->min)
                p1->suivant->min=b.valmin;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//parcours de l'avl et ecriture dans le fichier de sortie
void parcoursInfixe1(Noeud1* a, FILE* f){
    if(a!=NULL){
        parcoursInfixe1(a->fg, f);
		fprintf(f, "%d;%lf;%lf;%lf\n",a->val.idstation,((a->val.val)/(a->compteur)) , a->val.valmin, a->val.valmax);
        parcoursInfixe1(a->fd, f);
	}
}
//version decroissante
void parcoursInfixe1r(Noeud1* a, FILE* f){
    if(a!=NULL){
        parcoursInfixe1r(a->fd, f);
		fprintf(f, "%d;%lf;%lf;%lf\n",a->val.idstation,((a->val.val)/(a->compteur)) , a->val.valmin, a->val.valmax);
        parcoursInfixe1r(a->fg, f);
	}
}
//suppression de l'avl
void suppravl1( Noeud1* a){
    if(a->fd!=NULL)
        suppravl1(a->fd);
    if(a->fg!=NULL)
        suppravl1(a->fg);
    free(a);
}
//fonction qui cree un nouveau noeud
Noeud1* nouveaunoeud1(Donnee1 val){
	Noeud1*  noeud= malloc(sizeof(Noeud1));
	noeud->val = val;
	noeud->fg = NULL;
	noeud->fd = NULL;
	noeud->equilibre = 1;
    noeud->compteur = 1; 
	return noeud;
}
//fonction pour effectuer la rotation droite
Noeud1* rotationDroite1(Noeud1* a){
	Noeud1* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a+2, eq_a+eq_p+2, eq_p+1);
	a=pivot;
	return a;
}
//fonction pour effectuer la rotation gauche
Noeud1* rotationGauche1(Noeud1* a){
	Noeud1* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer une double rotation droite
Noeud1* doubleRotationDroite1(Noeud1* a){
	a->fg= rotationGauche1(a->fg);
	return rotationDroite1(a);
}
//fonction pour effectuer une double rotation gauche
Noeud1* doubleRotationGauche1(Noeud1* a){
	a->fd= rotationDroite1(a->fd);
	return rotationGauche1(a);
}
//fonction pour equilibrer l'avl
Noeud1* equilibrerAVL1(Noeud1* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGauche1(a);
		}
		else{
			return doubleRotationGauche1(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroite1(a);
		}
		else{
			return doubleRotationDroite1(a);
		}
	}
	return a;
}
// fonction recursive pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeud1* insert1(Noeud1* a, Donnee1 e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeud1(e);
	}
	else if(e.idstation<a->val.idstation){
		a->fg=insert1(a->fg,e,h);
		*h= -*h;
	}
	else if(e.idstation>a->val.idstation){
		a->fd=insert1(a->fd,e,h);
	}
	else{
		*h=0;
        a->val.val=a->val.val + e.val;
        a->compteur=a->compteur + 1;
        if(a->val.valmin>e.valmin)
            a->val.valmin=e.valmin;
        if(a->val.valmax<e.valmax)
            a->val.valmax=e.valmax;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVL1(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creation d'un noeud de l'abr
ABR1* creerArbre1(Donnee1 e){
    ABR1* a=malloc(sizeof(ABR1));
    a->elmt.idstation=e.idstation;
    a->elmt.val= e.val;
    a->elmt.valmin= e.valmin;
    a->elmt.valmax= e.valmax;
	a->compteur=1;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}
//insertion des elements dans l'abr
ABR1* insertionABR1(ABR1* a, Donnee1 e){
    if(a==NULL)
        return creerArbre1(e); 
    else if(e.idstation<a->elmt.idstation) 
        a->fg= insertionABR1(a->fg, e);
    else if(e.idstation>a->elmt.idstation)
        a->fd= insertionABR1(a->fd, e);
    else{
		a->compteur=a->compteur +1;
		a->elmt.val=a->elmt.val + e.val;
		if(a->elmt.valmin > e.valmin)
			a->elmt.valmin= e.valmin;
		if(a->elmt.valmax < e.valmax)
			a->elmt.valmax= e.valmax;
    }
    return a;
}
//version decroissante
ABR1* insertionABR1r(ABR1* a, Donnee1 e){
    if(a==NULL)
        return creerArbre1(e); 
    else if(e.idstation>a->elmt.idstation) 
        a->fg= insertionABR1r(a->fg, e);
    else if(e.idstation<a->elmt.idstation)
        a->fd= insertionABR1r(a->fd, e);
    else{
		a->compteur=a->compteur +1;
		a->elmt.val=a->elmt.val + e.val;
		if(a->elmt.valmin > e.valmin)
			a->elmt.valmin= e.valmin;
		if(a->elmt.valmax < e.valmax)
			a->elmt.valmax= e.valmax;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABR1(ABR1* a, FILE* f){
    if(a!=NULL){
        parcoursInfixeABR1(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf\n",a->elmt.idstation,((a->elmt.val)/(a->compteur)) , a->elmt.valmin, a->elmt.valmax);
        parcoursInfixeABR1(a->fd, f);
	}
}
//suppression de l'abr
void supprabr1( ABR1* a){
    if(a->fd!=NULL)
        supprabr1(a->fd);
    if(a->fg!=NULL)
        supprabr1(a->fg);
    free(a);
}
//fonction pour le mode t1
int mode1(FILE* fe, char tab[], FILE* fo, int r){
	int* h;
    double moy;
    Donnee1 b;
	Chaine1* a=NULL, *c;
	ABR1* a3=NULL;
    //si le type de tri est un abr
    if(strcmp(tab,"tab")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.valmin),&(b.valmax),&(b.idstation)) != EOF){
			//on verifie que la ligne est complete
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a=ajouterCroissant1(a, b);
			else
				a=ajouterCroissant1r(a, b);
        }
        while(a!=NULL){
            //on calcule la valeur moyenne pour chaque station
            moy=(a->somme)/(a->compteur);
            fprintf(fo, "%d;%lf;%lf;%lf\n",a->station, moy, a->min, a->max);//ecrire dans le fichier la ligne avec la station, temp min, max et moy(somme/compteur)
            c=a;
            a=a->suivant;
            free(c);
        }
        return 0;
    }
    else if(strcmp(tab,"avl")==0){
        Noeud1* a=NULL;
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.valmin),&(b.valmax),&(b.idstation)) != EOF){
            //on verifie que la ligne est complete
			//on ajoute les elements un à un dans la chaine pointée par a
			a=insert1(a, b, h);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
		if(r==0)
        	parcoursInfixe1(a, fo);
		else
			parcoursInfixe1r(a, fo);
        //suppression de l'avl
        suppravl1(a);
        return 0;
    }
    else{
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.valmin),&(b.valmax),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a3=insertionABR1(a3, b);
			else
				a3=insertionABR1r(a3, b);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeABR1(a3, fo);
        //suppression de l'avl
        supprabr1(a3);
		return 0;
    }
    return 1;
}
//remettre la date au bon format(d'un entier a un format yyyy-mm-dd)
void fonction_date2(int a, char* z){
    char int_char[9];
    sprintf(int_char, "%d", a);
    z[0]=int_char[0];
    z[1]=int_char[1];
    z[2]=int_char[2];
    z[3]=int_char[3];
    z[4]='-';
    z[5]=int_char[4];
    z[6]=int_char[5];
    z[7]='-';
    z[8]=int_char[6];
    z[9]=int_char[7];
}
//creer un nouveau noeud
Chaine2* creerchainon2(Donnee2 b){
    Chaine2* a=malloc(sizeof(Chaine2));
    a->date=b.date;
	a->heure=b.heure;
    a->somme= b.val;
    a->compteur=1;
    a->suivant=NULL;
   return a;
}
//ajouter les elements
Chaine2* ajouterCroissant2(Chaine2 *pliste, Donnee2 b){
	Chaine2 *nouveau=creerchainon2(b);
	Chaine2 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if ((pliste->date)*10+(pliste->heure) > (b.date)*10+(b.heure)) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
    else if ((pliste->date)*10+(pliste->heure) == (b.date)*10+(b.heure)) { // dans ce cas il faut placer l'element au début
		pliste->compteur=pliste->compteur +1;
        pliste->somme=pliste->somme + b.val;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && (p1->suivant->date)*10+(p1->suivant->heure) < (b.date)*10+(b.heure)){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if ((p1->suivant->date)*10+(p1->suivant->heure) == (b.date)*10+(b.heure)){//quand l'heure est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->somme=p1->suivant->somme + b.val;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//version decroissante
Chaine2* ajouterCroissant2r(Chaine2 *pliste, Donnee2 b){
	Chaine2 *nouveau=creerchainon2(b);
	Chaine2 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if ((pliste->date)*10+(pliste->heure) < (b.date)*10+(b.heure)) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
    else if ((pliste->date)*10+(pliste->heure) == (b.date)*10+(b.heure)) { // dans ce cas il faut placer l'element au début
		pliste->compteur=pliste->compteur +1;
        pliste->somme=pliste->somme + b.val;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && (p1->suivant->date)*10+(p1->suivant->heure) > (b.date)*10+(b.heure)){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if ((p1->suivant->date)*10+(p1->suivant->heure) == (b.date)*10+(b.heure)){//quand l'heure est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->somme=p1->suivant->somme + b.val;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;

}
//parcours de l'arbre et ecriture dans le fichier de sortie
void parcoursInfixe2(Noeud2* a, FILE* f){
	char z[10];
    if(a!=NULL){
        parcoursInfixe2(a->fg, f);
		fonction_date2(a->date, z);
    	fprintf(f, "%s;%d:00:00;%lf\n", z, a->heure, (a->somme)/(a->compteur));
        parcoursInfixe2(a->fd, f);
	}
}
//version decroissante
void parcoursInfixe2r(Noeud2* a, FILE* f){
	char z[10];
    if(a!=NULL){
        parcoursInfixe2r(a->fd, f);
		fonction_date2(a->date, z);
    	fprintf(f, "%s;%d:00:00;%lf\n", z, a->heure, (a->somme)/(a->compteur));
        parcoursInfixe2r(a->fg, f);
	}
}
//suppression de l'avl
void suppravl2( Noeud2* a){
    if(a->fd!=NULL)
        suppravl2(a->fd);
    if(a->fg!=NULL)
        suppravl2(a->fg);
    free(a);
}
//fonction qui cree un nouveau noeud
Noeud2* nouveaunoeud2(Donnee2 val){
	Noeud2*  noeud= malloc(sizeof(Noeud2));
	noeud->date = val.date;
	noeud->heure = val.heure;
	noeud->fg = NULL;
	noeud->fd = NULL;
    noeud->somme= val.val;
    noeud->compteur=1;
	noeud->equilibre = 0; 
	return noeud;
}
//fonction pour effectuer la rotation gauche
Noeud2* rotationGauche2(Noeud2* a){
	Noeud2* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer la rotation droite
Noeud2* rotationDroite2(Noeud2* a){
	Noeud2* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a+2, eq_a+eq_p+2, eq_p+1);
	a=pivot;
	return a;
}
//fonction pour effectuer une double rotation droite
Noeud2* doubleRotationDroite2(Noeud2* a){
	a->fg= rotationGauche2(a->fg);
	return rotationDroite2(a);
}
//fonction pour effectuer une double rotation gauche
Noeud2* doubleRotationGauche2(Noeud2* a){
	a->fd= rotationDroite2(a->fd);
	return rotationGauche2(a);
}
//fonction pour equilibrer l'avl
Noeud2* equilibrerAVL2(Noeud2* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGauche2(a);
		}
		else{
			return doubleRotationGauche2(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroite2(a);
		}
		else{
			return doubleRotationDroite2(a);
		}
	}
	return a;
}
// fonction pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeud2* insert2(Noeud2* a, Donnee2 e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeud2(e);
	}
	else if((a->date)*10+(a->heure) > (e.date)*10+(e.heure)){
		a->fg=insert2(a->fg,e,h);
		*h=-*h;
	}
	else if((a->date)*10+(a->heure) < (e.date)*10+(e.heure)){
		a->fd=insert2(a->fd,e,h);
	}
	else{
		*h=0;
        a->somme=a->somme + e.val;
        a->compteur= a->compteur +1;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVL2(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creer un noeud dans l'abr
ABR2* creerArbre2(Donnee2 e){
    ABR2* a=malloc(sizeof(ABR2));
	a->date=e.date;
	a->heure=e.heure;
	a->compteur=1;
	a->somme=e.val;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}
//insertion de l'element dans l'abr
ABR2* insertionABR2(ABR2* a, Donnee2 e){
    if(a==NULL)
        return creerArbre2(e); 
    else if((a->date)*10+(a->heure) > (e.date)*10+(e.heure)) 
        a->fg= insertionABR2(a->fg, e);
    else if((a->date)*10+(a->heure) < (e.date)*10+(e.heure))
        a->fd= insertionABR2(a->fd, e);
    else{
		a->compteur=a->compteur+1;
		a->somme=a->somme + e.val;
    }
    return a;
}
//version decroissante
ABR2* insertionABR2r(ABR2* a, Donnee2 e){
    if(a==NULL)
        return creerArbre2(e); 
    else if((a->date)*10+(a->heure) < (e.date)*10+(e.heure)) 
        a->fg= insertionABR2r(a->fg, e);
    else if((a->date)*10+(a->heure) > (e.date)*10+(e.heure))
        a->fd= insertionABR2r(a->fd, e);
    else{
		a->compteur=a->compteur+1;
		a->somme=a->somme + e.val;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABR2(ABR2* a, FILE* f){
	char z[10];
    if(a!=NULL){
        parcoursInfixeABR2(a->fg, f);
		fonction_date2(a->date, z);
    	fprintf(f, "%s;%d:00:00;%lf\n",z ,a->heure, (a->somme)/(a->compteur));
        parcoursInfixeABR2(a->fd, f);
	}
}
//suppression de l'abr
void supprabr2( ABR2* a){
    if(a->fd!=NULL)
        supprabr2(a->fd);
    if(a->fg!=NULL)
        supprabr2(a->fg);
    free(a);
}
//fonction pour le mode 2, cad p2 et t2
int mode2(FILE* fe, char tab[], FILE* fo, int r){
    Donnee2 b;
    double moy;
    int* h;
	Noeud2* a2=NULL;
	Chaine2* a=NULL, *c;
	ABR2* a3=NULL;
	char z[10];
    if(strcmp(tab,"tab")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.idstation))!=EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a=ajouterCroissant2(a, b);
			else
				a=ajouterCroissant2r(a, b);
        }
        while(a!=NULL){
			fonction_date2(a->date, z);
            //on calcule la moyenne de chaque horaire et on l'écrit dans le fichier de sortie
            moy=(a->somme)/(a->compteur);
            fprintf(fo, "%s;%d:00:00;%lf\n",z,a->heure, moy);
            c=a;
            a=a->suivant;
            free(c);
        }
        return 0;
    }
    else if(strcmp(tab,"avl")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.idstation)) != EOF){
				//on ajoute les elements un à un dans la chaine pointée par a
				a2=insert2(a2, b, h);
        }
        //  parcours de l'avl et ecriture dans le fichier de sortie  parcours de l'avl
		if(r==0)
        	parcoursInfixe2(a2, fo);
		else
			parcoursInfixe2r(a2, fo);
        //suppression de l'avl
        suppravl2(a2);
        return 0;
    }
    else{
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a3=insertionABR2(a3, b);
			else
				a3=insertionABR2r(a3, b);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeABR2(a3, fo);
        //suppression de l'avl
        supprabr2(a3);
		return 0;
    }
	return 1;
}
//fonction pour la remise en forme de la date
void fonction_date3(int a, char* z){
    char int_char[9];
    sprintf(int_char, "%d", a);    
    z[0]=int_char[0];
    z[1]=int_char[1];
    z[2]=int_char[2];
    z[3]=int_char[3];
    z[4]='-';
    z[5]=int_char[4];
    z[6]=int_char[5];
    z[7]='-';
    z[8]=int_char[6];
    z[9]=int_char[7];
}
//creer un nouveau chainon dans la liste des horaires
Chaine31 *creerchainon31(Donnee3 b){
    Chaine31* a=malloc(sizeof(Chaine31));
    a->date=b.date;
	a->heure=b.heure;
    a->suivant=NULL;
    a->val=b.val;
    return a;
}
//creer un nouveau chainon dans la liste des stations
Chaine30* creerchainon30(Donnee3 b){
    Chaine30* a=malloc(sizeof(Chaine30));
    a->idstation=b.idstation;
    a->chaine=creerchainon31(b);
    a->suivant=NULL;
    return a;
}
//ajouter un element dans la liste des mesures par heures
Chaine31* ajouterCroissant31(Chaine31 *pliste, Donnee3 b){
    Chaine31 *nouveau=creerchainon31(b);
	Chaine31 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if ((pliste->date)*10+(pliste->heure) > (b.date)*10+(b.heure)) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && (p1->suivant->date)*10+(p1->suivant->heure) < (b.date)*10+(b.heure)){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//ajouter un element dans la liste des stations
Chaine30* ajouterCroissant30(Chaine30* pliste, Donnee3 b){
    Chaine30 *nouveau=creerchainon30(b);
	Chaine30 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->idstation > b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->idstation < b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if(p1->suivant->idstation == b.idstation){ //la station est deja enregistree
            p1->suivant->chaine=ajouterCroissant31(p1->suivant->chaine, b);
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//ajouter un element dans la liste des mesures par heures version decroissante
Chaine31* ajouterCroissant31r(Chaine31 *pliste, Donnee3 b){
    Chaine31 *nouveau=creerchainon31(b);
	Chaine31 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if ((pliste->date)*10+(pliste->heure) < (b.date)*10+(b.heure)) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && (p1->suivant->date)*10+(p1->suivant->heure) > (b.date)*10+(b.heure)){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//ajouter un element dans la liste des stations version decroissant
Chaine30* ajouterCroissant30r(Chaine30* pliste, Donnee3 b){
    Chaine30 *nouveau=creerchainon30(b);
	Chaine30 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->idstation < b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->idstation > b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if(p1->suivant->idstation == b.idstation){ //la station est deja enregistree
            p1->suivant->chaine=ajouterCroissant31r(p1->suivant->chaine, b);
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//parcours de l'avl et ecriture
void parcoursInfixe3(Noeud3* a, FILE* f){
    Chaine31* c;
    char z[10];
    if(a!=NULL){
            parcoursInfixe3(a->fg, f);
            while(a->chaine!=NULL){
                fonction_date3(a->chaine->date, z);
                fprintf(f,"%d;%s;%d;%lf\n",a->idstation, z, a->chaine->heure, a->chaine->val);
                c=a->chaine;
                a->chaine=a->chaine->suivant;
                free(c);
            }
            parcoursInfixe3(a->fd, f);
    }
}
//suppression de l'avl
void suppravl3( Noeud3* a){
    if(a->fd!=NULL)
        suppravl3(a->fd);
    if(a->fg!=NULL)
        suppravl3(a->fg);
    free(a);
}
//fonction qui cree un nouveau noeud
Noeud3* nouveaunoeud3(Donnee3 val){
	Noeud3*  noeud= malloc(sizeof(Noeud3));
	noeud->idstation = val.idstation;
	noeud->fg = NULL;
	noeud->fd = NULL;
	noeud->equilibre = 0; 
    noeud->chaine=creerchainon31(val);
	return noeud;
}
//fonction pour effectuer la rotation gauche
Noeud3* rotationGauche3(Noeud3* a){
	Noeud3* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer la rotation droite
Noeud3* rotationDroite3(Noeud3* a){
	Noeud3* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a+2, eq_a+eq_p+2, eq_p+1);
	a=pivot;
	return a;
}
//fonction pour effectuer une double rotation droite
Noeud3* doubleRotationDroite3(Noeud3* a){
	a->fg= rotationGauche3(a->fg);
	return rotationDroite3(a);
}
//fonction pour effectuer une double rotation gauche
Noeud3* doubleRotationGauche3(Noeud3* a){
	a->fd= rotationDroite3(a->fd);
	return rotationGauche3(a);
}
//fonction pour equilibrer l'avl
Noeud3* equilibrerAVL3(Noeud3* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGauche3(a);
		}
		else{
			return doubleRotationGauche3(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroite3(a);
		}
		else{
			return doubleRotationDroite3(a);
		}
	}
	return a;
}
// fonction pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeud3* insert3(Noeud3* a, Donnee3 e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeud3(e);
	}
	else if(e.idstation<a->idstation){
		a->fg=insert3(a->fg,e,h);
		*h=-*h;
	}
	else if(e.idstation>a->idstation){
		a->fd=insert3(a->fd,e,h);
	}
	else{
		*h=0;
        a->chaine=ajouterCroissant31(a->chaine, e);
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVL3(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//version decroissante
Noeud3* insert3r(Noeud3* a, Donnee3 e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeud3(e);
	}
	else if(e.idstation>a->idstation){
		a->fg=insert3r(a->fg,e,h);
		*h=-*h;
	}
	else if(e.idstation<a->idstation){
		a->fd=insert3r(a->fd,e,h);
	}
	else{
		*h=0;
        a->chaine=ajouterCroissant31r(a->chaine, e);
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVL3(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creer un nouveau noeud de l'abr
ABR3* creerArbre3(Donnee3 e){
    ABR3* a=malloc(sizeof(ABR3));
	a->chaine=creerchainon31(e);
	a->idstation=e.idstation;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}
//inserer un element dans l'abr
ABR3* insertionABR3(ABR3* a, Donnee3 e){
    if(a==NULL)
        return creerArbre3(e); 
    else if(a->idstation > e.idstation) 
        a->fg= insertionABR3(a->fg, e);
    else if(a->idstation < e.idstation)
        a->fd= insertionABR3(a->fd, e);
    else{
		a->chaine= ajouterCroissant31(a->chaine, e);
		return a;
    }
    return a;
}
//inserer un element dans l'abr version decroissante
ABR3* insertionABR3r(ABR3* a, Donnee3 e){
    if(a==NULL)
        return creerArbre3(e); 
    else if(a->idstation < e.idstation) 
        a->fg= insertionABR3r(a->fg, e);
    else if(a->idstation > e.idstation)
        a->fd= insertionABR3r(a->fd, e);
    else{
		a->chaine= ajouterCroissant31r(a->chaine, e);
		return a;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABR3(ABR3* a, FILE* f){
	Chaine31* c2;
	char z[10];
    if(a->fg!=NULL)
        parcoursInfixeABR3(a->fg, f);
	while(a->chaine!=NULL){
		fonction_date3(a->chaine->date, z);
    	fprintf(f, "%d;%s;%d:00:00;%lf\n",a->idstation, z,a->chaine->heure,a->chaine->val);
		c2=a->chaine;
        a->chaine=a->chaine->suivant;
        free(c2);
	}
    if(a->fd!=NULL)
        parcoursInfixeABR3(a->fd, f);
}
//suppression de l'abr
void supprabr3( ABR3* a){
    if(a->fd!=NULL)
        supprabr3(a->fd);
    if(a->fg!=NULL)
        supprabr3(a->fg);
    free(a);
}
//fonction pour le mode 3, donc t3 et p3
int mode3(FILE* fe, char tab[], FILE* fo, int r){
    Chaine30* a=NULL, *c1;
    Chaine31 *c2;
    Donnee3 b;
    int* h;
	Noeud3* a2=NULL;
	ABR3* a3=NULL;
	char z[10];
    if(strcmp(tab,"tab")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.idstation))!=EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a=ajouterCroissant30(a, b);
			else
				a=ajouterCroissant30r(a, b);
        }
		while(a!=NULL){
			while(a->chaine!=NULL){
				fonction_date3(a->chaine->date, z);
				fprintf(fo,"%d;%s;%d:00:00;%lf\n",a->idstation, z, a->chaine->heure, a->chaine->val);
				c2=a->chaine;
				a->chaine=a->chaine->suivant;
				free(c2);
			}
			c1=a;
			a=a->suivant;
			free(c1);
		}
        return 0;
    }
    else if(strcmp(tab,"avl")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%d\n",&(b.date), &(b.heure), &(b.decalage),&(b.val),&(b.idstation))!=EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a2=insert3(a2, b, h);
			else
				a2=insert3r(a2, b, h);
			
        }
        // parcours de l'avl et ecriture dans le fichier de sortie  parcours de l'avl
        parcoursInfixe3(a2, fo);
        //suppression de l'avl
        suppravl3(a2);
        return 0;
    }
    else{
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a3=insertionABR3(a3, b);
			else
				a3=insertionABR3r(a3, b);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeABR3(a3, fo);
        //suppression de l'avl
        supprabr3(a3);
		return 0;
    }
    return 1;
}
//creation d'un nouveau chainon
Chainew* creerchainonw(Donneew b){
    Chainew* a=malloc(sizeof(Chainew));
    a->val.idstation=b.idstation;
    a->val.x=b.x;
    a->val.y=b.y;
    a->val.vent=b.vent;
    a->compteur=1;
    a->suivant=NULL;
    return a;
}
//on regarde quand c'est égal, on ajoute +1 au compteur et on actualise la temp min et max
Chainew* ajouterCroissantw(Chainew *pliste, Donneew b){
	Chainew *nouveau=creerchainonw(b);
	Chainew *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->val.idstation > b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
    else if (pliste->val.idstation == b.idstation){//quand la station est deja enregistree au début de la chaine
		pliste->compteur=pliste->compteur +1;
		pliste->val.vent.direction=pliste->val.vent.direction + b.vent.direction;
		pliste->val.vent.vitesse=pliste->val.vent.vitesse + b.vent.vitesse;
		pliste->val.x=pliste->val.x + b.x;
		pliste->val.y=pliste->val.y + b.y;
    }
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->val.idstation < b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->val.idstation == b.idstation){//quand la station est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->val.vent.direction=p1->suivant->val.vent.direction + b.vent.direction;
            p1->suivant->val.vent.vitesse=p1->suivant->val.vent.vitesse + b.vent.vitesse;
            p1->suivant->val.x=p1->suivant->val.x + b.x;
            p1->suivant->val.y=p1->suivant->val.y + b.y;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//version decroissante
Chainew* ajouterCroissantwr(Chainew *pliste, Donneew b){
Chainew *nouveau=creerchainonw(b);
	Chainew *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->val.idstation > b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
    else if (pliste->val.idstation == b.idstation){//quand la station est deja enregistree au début de la chaine
		pliste->compteur=pliste->compteur +1;
		pliste->val.vent.direction=pliste->val.vent.direction + b.vent.direction;
		pliste->val.vent.vitesse=pliste->val.vent.vitesse + b.vent.vitesse;
		pliste->val.x=pliste->val.x + b.x;
		pliste->val.y=pliste->val.y + b.y;
    }
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->val.idstation > b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->val.idstation == b.idstation){//quand la station est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->val.vent.direction=p1->suivant->val.vent.direction + b.vent.direction;
            p1->suivant->val.vent.vitesse=p1->suivant->val.vent.vitesse + b.vent.vitesse;
            p1->suivant->val.x=p1->suivant->val.x + b.x;
            p1->suivant->val.y=p1->suivant->val.y + b.y;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//parcours de l'avl et ecriture dans le fichier de sortie
void parcoursInfixew(Noeudw* a, FILE* f){

    if(a!=NULL){
        parcoursInfixew(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf;%lf\n",a->val.idstation, (a->val.x)/(a->compteur), (a->val.y)/(a->compteur), (a->val.vent.direction)/(a->compteur), (a->val.vent.vitesse)/(a->compteur));
        parcoursInfixew(a->fd, f);
	}
}
//version decroissante
void parcoursInfixewr(Noeudw* a, FILE* f){

    if(a!=NULL){
        parcoursInfixewr(a->fd, f);
    	fprintf(f, "%d;%lf;%lf;%lf;%lf\n",a->val.idstation, (a->val.x)/(a->compteur), (a->val.y)/(a->compteur), (a->val.vent.direction)/(a->compteur), (a->val.vent.vitesse)/(a->compteur));
        parcoursInfixewr(a->fg, f);
	}
}
//suppression de l'avl
void suppravlw( Noeudw* a){
    if(a->fd!=NULL)
        suppravlw(a->fd);
    if(a->fg!=NULL)
        suppravlw(a->fg);
    free(a);
}
//fonction qui cree un nouveau noeud
Noeudw* nouveaunoeudw(Donneew a){
	Noeudw*  noeud= malloc(sizeof(Noeudw));
	noeud->val = a;
	noeud->fg = NULL;
	noeud->fd = NULL;
    noeud->compteur=1;
	noeud->equilibre = 0; 
	return noeud;
}
//fonction pour effectuer la rotation gauche
Noeudw* rotationGauchew(Noeudw* a){
	Noeudw* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer la rotation droite
Noeudw* rotationDroitew(Noeudw* a){
	Noeudw* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a+2, eq_a+eq_p+2, eq_p+1);
	a=pivot;
	return a;
}
//fonction pour effectuer une double rotation droite
Noeudw* doubleRotationDroitew(Noeudw* a){
	a->fg= rotationGauchew(a->fg);
	return rotationDroitew(a);
}
//fonction pour effectuer une double rotation gauche
Noeudw* doubleRotationGauchew(Noeudw* a){
	a->fd= rotationDroitew(a->fd);
	return rotationGauchew(a);
}
//fonction pour equilibrer l'avl
Noeudw* equilibrerAVLw(Noeudw* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGauchew(a);
		}
		else{
			return doubleRotationGauchew(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroitew(a);
		}
		else{
			return doubleRotationDroitew(a);
		}
	}
	return a;
}
// fonction pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeudw* insertw(Noeudw* a, Donneew e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeudw(e);
	}
	else if(e.idstation<a->val.idstation){
		a->fg=insertw(a->fg,e,h);
		*h=-*h;
	}
	else if(e.idstation>a->val.idstation){
		a->fd=insertw(a->fd,e,h);
	}
	else{
		*h=0;
        a->compteur=a->compteur +1;
        a->val.vent.direction=a->val.vent.direction +e.vent.direction;
        a->val.vent.vitesse=a->val.vent.vitesse +e.vent.vitesse;
        a->val.x=a->val.x +e.x;
        a->val.y=a->val.y +e.y;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVLw(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creation d'un noeud de l'abr
ABRW* creerArbrew(Donneew b){
    ABRW* a=malloc(sizeof(ABRW));
    a->val.idstation=b.idstation;
    a->val.x=b.x;
    a->val.y=b.y;
    a->val.vent=b.vent;
    a->compteur=1;
    a->fd=NULL;
	a->fg=NULL;
    return a;
}
//insertion des elements dans l'abr
ABRW* insertionABRW(ABRW* a, Donneew e){
    if(a==NULL)
        return creerArbrew(e); 
    else if(e.idstation<a->val.idstation) 
        a->fg= insertionABRW(a->fg, e);
    else if(e.idstation>a->val.idstation)
        a->fd= insertionABRW(a->fd, e);
    else{
		a->compteur=a->compteur +1;
		a->val.vent.direction=a->val.vent.direction + e.vent.direction;
		a->val.vent.vitesse=a->val.vent.vitesse + e.vent.vitesse;
		a->val.x=a->val.x + e.x;
		a->val.y=a->val.y + e.y;
    }
    return a;
}
//version decroissante
ABRW* insertionABRWr(ABRW* a, Donneew e){
    if(a==NULL)
        return creerArbrew(e); 
    else if(e.idstation>a->val.idstation) 
        a->fg= insertionABRWr(a->fg, e);
    else if(e.idstation<a->val.idstation)
        a->fd= insertionABRWr(a->fd, e);
    else{
		a->compteur=a->compteur +1;
		a->val.vent.direction=a->val.vent.direction + e.vent.direction;
		a->val.vent.vitesse=a->val.vent.vitesse + e.vent.vitesse;
		a->val.x=a->val.x + e.x;
		a->val.y=a->val.y + e.y;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABRW(ABRW* a, FILE* f){

    if(a!=NULL){
        parcoursInfixeABRW(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf;%lf\n",a->val.idstation, (a->val.x)/(a->compteur), (a->val.y)/(a->compteur), (a->val.vent.direction)/(a->compteur), (a->val.vent.vitesse)/(a->compteur));
        parcoursInfixeABRW(a->fd, f);
	}
}
//suppression de l'abr
void supprabrw( ABRW* a){
    if(a->fd!=NULL)
        supprabrw(a->fd);
    if(a->fg!=NULL)
        supprabrw(a->fg);
    free(a);
}
//fonction du mode w
int w(FILE* fe, char tab[], FILE* fo, int r){
    double moy;
    Chainew* a=NULL, *c; 
    Donneew b;
    FILE* f1=NULL;
    int* h;
	Noeudw* a2=NULL;
    ABRW* a3=NULL;
    if(strcmp(tab,"tab")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.x),&(b.y),&(b.vent.direction),&(b.vent.vitesse),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a=ajouterCroissantw(a, b);
			else
				a=ajouterCroissantwr(a, b);
		
        }
		while(a!=NULL){
			fprintf(fo, "%d;%lf;%lf;%lf;%lf\n",a->val.idstation, (a->val.x)/(a->compteur), (a->val.y)/(a->compteur), (a->val.vent.direction)/(a->compteur), (a->val.vent.vitesse)/(a->compteur));
			c=a;
			a=a->suivant;
			free(c);
		}
		fclose(f1);
		return 0;
    }
    else if(strcmp(tab,"avl")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.x),&(b.y),&(b.vent.direction),&(b.vent.vitesse),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			a2=insertw(a2, b, h);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
		if(r==0)
        	parcoursInfixew(a2, fo);
		else
			parcoursInfixewr(a2, fo);
        //suppression de l'avl
        suppravlw(a2);
        return 0;
    }
    else{
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.x),&(b.y),&(b.vent.direction),&(b.vent.vitesse),&(b.idstation)) != EOF){
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a3=insertionABRW(a3, b);
			else
				a3=insertionABRWr(a3, b);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeABRW(a3, fo);
        //suppression de l'avl
        supprabrw(a3);
		return 0;
    }
    return 1;
}
//créer le premier chainon de la liste chainée
Chainep1* creerchainonp1(Donneep1 b){
    Chainep1* a=malloc(sizeof(Chainep1));
    a->station=b.idstation;
    a->min=b.valmin;
    a->max=b.valmax;
    a->somme=b.val;
    a->compteur=1;
    a->suivant=NULL;
    return a;
}
//ajouter les éléments dans la liste chainée
Chainep1* ajouterCroissantp1(Chainep1 *pliste, Donneep1 b){         	
	Chainep1 *nouveau=creerchainonp1(b);
	Chainep1 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->station > b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->station < b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->station == b.idstation){//quand la station est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->somme=p1->suivant->somme + b.val;
            if(b.valmax > p1->suivant->max)
                p1->suivant->max=b.valmax;
            if(b.valmin < p1->suivant->min)
                p1->suivant->min=b.valmin;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//version decroissante
Chainep1* ajouterCroissantp1r(Chainep1 *pliste, Donneep1 b){
	Chainep1 *nouveau=creerchainonp1(b);
	Chainep1 *p1 =pliste;
	if(pliste==NULL){// si la liste est vide
		pliste=nouveau;
	}
	else if (pliste->station < b.idstation) { // dans ce cas il faut placer l'element au début
		nouveau->suivant=pliste;
        pliste=nouveau;
	}
	else{
		//on parcours la liste jusqu'à trouver où doit aller le nouveau élément
		while(p1->suivant!= NULL && p1->suivant->station > b.idstation){ 
			p1=p1->suivant;
		}
		if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
			p1->suivant=nouveau;
		}
        else if (p1->suivant->station == b.idstation){//quand la station est deja enregistree
            p1->suivant->compteur=p1->suivant->compteur +1;
            p1->suivant->somme=p1->suivant->somme + b.val;
            if(b.valmax > p1->suivant->max)
                p1->suivant->max=b.valmax;
            if(b.valmin < p1->suivant->min)
                p1->suivant->min=b.valmin;
        }
		else{ // il faut inserer le maillon en millieu de chaîne, après p1
			nouveau->suivant=p1->suivant;
			p1->suivant = nouveau;
		}
	}
	return pliste;
}
//parcours de l'avl et ecriture dans le fichier de sortie
void parcoursInfixep1(Noeudp1* a, FILE* f){
    if(a!=NULL){
        parcoursInfixep1(a->fg, f);
		fprintf(f, "%d;%lf;%lf;%lf\n",a->val.idstation,((a->val.val)/(a->compteur)) , a->val.valmin, a->val.valmax);
        parcoursInfixep1(a->fd, f);
	}
}
//version decroissante
void parcoursInfixep1r(Noeudp1* a, FILE* f){

    if(a!=NULL){
        parcoursInfixep1r(a->fd, f);
		fprintf(f, "%d;%lf;%lf;%lf\n",a->val.idstation,((a->val.val)/(a->compteur)) , a->val.valmin, a->val.valmax);
        parcoursInfixep1r(a->fg, f);
	}
}
//suppression de l'avl
void suppravlp1( Noeudp1* a){
    if(a->fd!=NULL)
        suppravlp1(a->fd);
    if(a->fg!=NULL)
        suppravlp1(a->fg);
    free(a);
}
//fonction qui cree un nouveau noeud
Noeudp1* nouveaunoeudp1(Donneep1 val){
	Noeudp1*  noeud= malloc(sizeof(Noeudp1));
	noeud->val = val;
	noeud->fg = NULL;
	noeud->fd = NULL;
	noeud->equilibre = 1;
    noeud->compteur = 1; 
	return noeud;
}
//fonction pour effectuer la rotation droite
Noeudp1* rotationDroitep1(Noeudp1* a){
	Noeudp1* pivot;
	int eq_a, eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - min(eq_p, 0) +1;
	pivot->equilibre=max2( eq_a+2, eq_a+eq_p+2, eq_p+1);
	a=pivot;
	return a;
}
//fonction pour effectuer la rotation gauche
Noeudp1* rotationGauchep1(Noeudp1* a){
	Noeudp1* pivot;
	int eq_a, eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre= eq_a - max(eq_p, 0) -1;
	pivot->equilibre=min2( eq_a-2, eq_a+eq_p-2, eq_p-1);
	a=pivot;
	return a;
}
//fonction pour effectuer une double rotation droite
Noeudp1* doubleRotationDroitep1(Noeudp1* a){
	a->fg= rotationGauchep1(a->fg);
	return rotationDroitep1(a);
}
//fonction pour effectuer une double rotation gauche
Noeudp1* doubleRotationGauchep1(Noeudp1* a){
	a->fd= rotationDroitep1(a->fd);
	return rotationGauchep1(a);
}
//fonction pour equilibrer l'avl
Noeudp1* equilibrerAVLp1(Noeudp1* a){
	if(a->equilibre>=2){// sous-arbre droit plus profond
		if(a->fd->equilibre>= 0){
			return rotationGauchep1(a);
		}
		else{
			return doubleRotationGauchep1(a);
		}
	}
	else if(a->equilibre<=-2){ // sous-arbre gauche plus profond
		if(a->fg->equilibre<=0){ 
			return rotationDroitep1(a);
		}
		else{
			return doubleRotationDroitep1(a);
		}
	}
	return a;
}
// fonction recursive pour inserer un nouvel element, et qui renvoie un pointeur vers la racine
Noeudp1* insertp1(Noeudp1* a, Donneep1 e, int* h){
	if(a==NULL){
		*h=1;
		return nouveaunoeudp1(e);
	}
	else if(e.idstation<a->val.idstation){
		a->fg=insertp1(a->fg,e,h);
		*h= -*h;
	}
	else if(e.idstation>a->val.idstation){
		a->fd=insertp1(a->fd,e,h);
	}
	else{
		*h=0;
        a->val.val=a->val.val + e.val;
        a->compteur=a->compteur + 1;
        if(a->val.valmin>e.valmin)
            a->val.valmin=e.valmin;
        if(a->val.valmax<e.valmax)
            a->val.valmax=e.valmax;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+ *h;
		a=equilibrerAVLp1(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}
//creation d'un noeud de l'abr
ABRp1* creerArbrep1(Donneep1 e){
    ABRp1* a=malloc(sizeof(ABR1));
    a->elmt.idstation=e.idstation;
    a->elmt.val= e.val;
    a->elmt.valmin= e.valmin;
    a->elmt.valmax= e.valmax;
	a->compteur=1;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}
//insertion des elements dans l'abr
ABRp1* insertionABRp1(ABRp1* a, Donneep1 e){
    if(a==NULL)
        return creerArbrep1(e); 
    else if(e.idstation<a->elmt.idstation) 
        a->fg= insertionABRp1(a->fg, e);
    else if(e.idstation>a->elmt.idstation)
        a->fd= insertionABRp1(a->fd, e);
    else{
		a->compteur=a->compteur +1;
		a->elmt.val=a->elmt.val + e.val;
		if(a->elmt.valmin > e.valmin)
			a->elmt.valmin= e.valmin;
		if(a->elmt.valmax < e.valmax)
			a->elmt.valmax= e.valmax;
    }
    return a;
}
//version decroissante
ABRp1* insertionABRp1r(ABRp1* a, Donneep1 e){
    if(a==NULL)
        return creerArbrep1(e); 
    else if(e.idstation>a->elmt.idstation) 
        a->fg= insertionABRp1(a->fg, e);
    else if(e.idstation<a->elmt.idstation)
        a->fd= insertionABRp1(a->fd, e);
    else{
		a->compteur=a->compteur +1;
		a->elmt.val=a->elmt.val + e.val;
		if(a->elmt.valmin > e.valmin)
			a->elmt.valmin= e.valmin;
		if(a->elmt.valmax < e.valmax)
			a->elmt.valmax= e.valmax;
    }
    return a;
}
//parcours de l'abr et ecriture dans le fichier de sortie
void parcoursInfixeABRp1(ABRp1* a, FILE* f){
    if(a!=NULL){
        parcoursInfixeABRp1(a->fg, f);
    	fprintf(f, "%d;%lf;%lf;%lf\n",a->elmt.idstation,((a->elmt.val)/(a->compteur)) , a->elmt.valmin, a->elmt.valmax);
        parcoursInfixeABRp1(a->fd, f);
	}
}
//suppression de l'abr
void supprabrp1( ABRp1* a){
    if(a->fd!=NULL)
        supprabrp1(a->fd);
    if(a->fg!=NULL)
        supprabrp1(a->fg);
    free(a);
}
//fonction pour le mode p1
int modep1(FILE* fe, char tab[], FILE* fo, int r){
	int* h;
    double moy;
    Donneep1 b;
	Chainep1* a=NULL, *c;
	ABRp1* a3=NULL;
	Noeudp1* a2=NULL;
    //si le type de tri est un abr
    if(strcmp(tab,"tab")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.valmin),&(b.valmax),&(b.idstation)) != EOF){
			b.valmin=b.val;
			b.valmax=b.val;
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a=ajouterCroissantp1(a, b);
			else
				a=ajouterCroissantp1r(a, b);
        }
        while(a!=NULL){
            //on calcule la valeur moyenne pour chaque station
            moy=(a->somme)/(a->compteur);
            fprintf(fo, "%d;%lf;%lf;%lf\n",a->station, moy, a->min, a->max);//ecrire dans le fichier la ligne avec la station, temp min, max et moy(somme/compteur)
            c=a;
            a=a->suivant;
            free(c);
        }
        return 0;
    }
    else if(strcmp(tab,"avl")==0){
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.valmin),&(b.valmax),&(b.idstation)) != EOF){
            b.valmin=b.val;
			b.valmax=b.val;
			//on ajoute les elements un à un dans la chaine pointée par a
			a2=insertp1(a2, b, h);
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
		if(r==0)
        	parcoursInfixep1(a2, fo);
		else
			parcoursInfixep1r(a2, fo);
        //suppression de l'avl
        suppravlp1(a2);
        return 0;
    }
    else{
        while(fscanf(fe,"%dT%d:00:00+%d:00;%lf;%lf;%lf;%d\n",&(b.date),&(b.heure),&(b.decalage),&(b.val),&(b.valmin),&(b.valmax),&(b.idstation)) != EOF){
            //on verifie que la ligne est complete
			b.valmin=b.val;
			b.valmax=b.val;
			//on ajoute les elements un à un dans la chaine pointée par a
			if(r==0)
				a3=insertionABRp1(a3, b);
			else
				a3=insertionABRp1r(a3, b);
			
        }
        //ecriture dans le fichier de sortie et parcours de l'avl
        parcoursInfixeABRp1(a3, fo);
        //suppression de l'avl
        supprabrp1(a3);
		return 0;
    }
    return 1;
}