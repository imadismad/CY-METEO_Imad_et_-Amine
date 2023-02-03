#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>

#include "th.h"


int main(int argc, char *argv[]){
    int a; //valeur de retour des fonctions
    int r=0, f=0, o=0, t=0, mo=0;    //compteur pour les arguments
    char *tri=NULL, *mode=NULL, *fichierentree=NULL, *fichiersortie=NULL; //c'est ici que l'on récupèrera les arguments
      
    // f pour le fichier d'entree
    // o pour le fichier en sortie 
    //r pour le mode décroissant
    //t pour le type de tri

    for (int i = 1; i < argc; i += 1) {   //parcours de tous les arguments et vérifications
        if (strcmp(argv[i], "-f") == 0) {
            f=f+1;
            fichierentree = argv[i+1];
            i=i+1;
            if(f>=2){
                    return 1;
                }
        } 
        else if (strcmp(argv[i], "-o") == 0) {
            o=o+1;
            fichiersortie = argv[i+1];
            i=i+1;
            if(o>=2){
                    return 1;
                }

        }
        else if (strcmp(argv[i], "-r") == 0) {
            r=r+1;
            if(r>=2){ //vérifier s'il n'y a pas trop de fois le même type d'argument
                    return 1;
                }
        }
        else if (strcmp(argv[i], "-t") == 0) {
            mo=mo+1;
            mode = argv[i+1];
            i=i+1;
            if(mo>=2){
                    return 1;
                }

        }
        else if (strcmp(argv[i], "--tab") == 0) {
            tri = "tab";

        }
        else if (strcmp(argv[i], "--abr") == 0) {
            tri = "abr";

        }
        else if (strcmp(argv[i], "--avl") == 0) {
            tri = "avl";

        }
        else{       //s'il s'agit d'un argument non reconnu, on sort
            return 1;
        }
    }
    if(f==0){ //si aucun fichier d'entrée n'est fourni
 
        return 2;
    }
    if(o==0){
      
        return 3;
    }
    if(mo==0){   //si un mode est choisi
     
        return 1;
    }
    if(tri==NULL){ //si aucun type de tri n'est choisi, faire avec un avl
        tri="avl";
    }
    
    //ouverture des fichiers et vérifier qu'ils s'ouvrent correctement
    FILE* fpe=fopen(fichierentree, "r");
 
    if (fpe==NULL){
        return 2;
    }

    FILE* fpo=fopen(fichiersortie, "w+");
 
    if (fpo==NULL){
        return 3;
    }

    //vérifier le mode choisi, et s'il existe le faire et si il faut le faire dans le sens décroissant avec r
    if(strcmp(mode,"t1")==0 || strcmp(mode,"p1")==0){    
        a=mode1(fpe, tri,fpo, r); //rajouter le fichier de sortie
        fclose(fpe);
        fclose(fpo);
        return a;
    }
    else if(strcmp(mode,"t2")==0 || strcmp(mode,"p2")==0){    
        a=mode2(fpe, tri, fpo, r); //valeur de retour de la fct qui prendra en paramètre un pointeur sur fichier et un type de tri;
        fclose(fpe);
        fclose(fpo);
        return a;
    }
    else if(strcmp(mode,"t3")==0 || strcmp(mode,"p3")==0){    
        a=mode3(fpe, tri, fpo, r); //valeur de retour de la fct qui prendra en paramètre un pointeur sur fichier et un type de tri;
        fclose(fpe);
        fclose(fpo);
        return a;
    }
    else if(strcmp(mode,"w")==0){    
        a=w(fpe, tri, fpo, r); //valeur de retour de la fct qui prendra en paramètre un pointeur sur fichier et un type de tri;
        fclose(fpe);
        fclose(fpo);
        return a;
    }
    else if(strcmp(mode,"h")==0){    
        a=h(fpe, tri, fpo, r); //valeur de retour de la fct qui prendra en paramètre un pointeur sur fichier et un type de tri;
        fclose(fpe);
        fclose(fpo);
        return a;
    }
    else if(strcmp(mode,"m")==0){    
        a=m(fpe, tri, fpo, r); //valeur de retour de la fct qui prendra en paramètre un pointeur sur fichier et un type de tri;
        fclose(fpe);
        fclose(fpo);
        return a;
    }
    //si le mode saisi ne correspond à aucun
    else{
        return 1;
    }

}
