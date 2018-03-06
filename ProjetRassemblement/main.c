   
    /*
     * 
     *  Author : Klement Serralta, Corentin Sadowski, Gabin Lhomme
     *  Creation : 05/01/2018
     * 
    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Retourne un nombre aléatoire de BorneMin à BorneMax 
int genNbPremier(int borneMin, int borneMax)
{
    int nombrePremier = rand()%(borneMax)+(borneMin);
    return nombrePremier; 
}

// Prend un nombre puis ressort un bit qui détermine si le nombre entier est premier ou non
int testNbPremier(int nombre)
{
    int x = 0; 
    int i = 2;
    int racCarree = sqrt(nombre);
    while ( (x == 0) && (i <= racCarree))
    {
        if ( nombre%i == 0 ) x++;
        else i++;
    }
    // Le nombre n'est pas premier
    if ( x == 1 ) return 0;
    // Le nombre est premier;
    else return 1 ;
}  

// Teste le nombre généré et ne donne un résultat que quand il est premier
int generationNombrePremier(int borneMin, int borneMax)
{
    int nombrePremier = genNbPremier(borneMin, borneMax);
    // Tantque le nombre n'est pas premier, relancer la génération
    while (testNbPremier(nombrePremier)==0)
    {
        nombrePremier = genNbPremier(borneMin, borneMax);
    }  
    return nombrePremier;
}

// Prend deux nombres puis ressort le Plus Grand Commun Diviseur de ces nombres.
int PGCD2Nombres(int grandNombre, int petitNombre) 
{
    int i = 1;
    int x = 1;
    // Ne sert à rien de tester des plus grand diviseur
    while ( i <= petitNombre)
    {
        if ( grandNombre%i == 0 && petitNombre%i == 0) x = i;
        i++;
    }
    return x;
}

// Donne la valeur de N
int getN(int pPetit, int qGrand)
{
    int n = pPetit * qGrand;
    return n;
}

// Fonction qui sert à trouver e, e faisant partie de la clef publique
int getE(int pPetit, int qGrand, int phi)
{
    int i = 2;
    // prevision de diviser pPetit ET qGrand d'ou les stocker
    int p = pPetit-1;
    int q = qGrand-1;
    
    // Boucle permetant de calculer le ppcm
    while(i <= p)
    {
        if((p%i == 0) && (q%i == 0))
        {
            p = p/i;
            q = q/i;
        }
        else i++;
    }
    // ppcm = Plus petit commun multiple
    int ppcm = (pPetit-1)*q;
    printf("Voici ppcm : %d \n", ppcm);
    
    // e doit être plus petit que le ppcm, d'ou l'initialiser à ppcm + 1
    int e = ppcm + 1;
    while(e > ppcm)
    {  
        int nombreAleatoire = rand()%(ppcm-4)+3;
        if( PGCD2Nombres(ppcm,nombreAleatoire) == 1 ) e = nombreAleatoire;
        //if(phi%nombreAleatoire != 0 && ppcm%nombreAleatoire != 0) e = nombreAleatoire;
    }
    return e;
}

// Trouver d telque d*e congru à 1 modulo phi
int getD (int phi, int e) 
{
    int d = 0;
    // Application d'une fonction de temps pour éviter de relancer nous même le programme si le programme dure trop longtemps
    // Peut durer longtemps où même planter car la recherche du d peut planter car pc pas assez puissant pour les nombres à sortir
    int temps = time(NULL);
    int t2 = time(NULL);
    while (((d*e)%phi != 1) && (t2-temps < 10)) 
    {
        t2 = time(NULL);
        d++;
    }
    if (t2-temps >= 10) return 0;
    else return d;
}

// Fontion permettant de créer une phrase que phillipe nous envérais.
void convertToASCII ( char * msgAConvertir, char * msgConvertis )
{
    msgConvertis = realloc( msgConvertis, strlen( msgAConvertir) );
    for ( int i=0;i<strlen( msgAConvertir );i++) msgConvertis[i] = (int)msgAConvertir[i];
}

// fonction qui sert a trouver la longueur du message a convertir, celle-ci servira a allouer la taille du tableau à blocs.
int getLongueur (char * msgAConvertir)
{
    int longueur = strlen(msgAConvertir)-1;
    if(longueur%2 != 0) longueur = (longueur / 2)+1; 
    else longueur = longueur /2; 
    return longueur;
}

// Fonction permettant de transformer un tableau de charactère de Int en Bloc de Int contenant à l'origine les codes ascii de 2 charactères
void creerBlocs ( char * msgConvertis, unsigned int * tabBlocDe2, int longueur) 
{
    // boucle permettant de remplir le tableau de bloc
    for(int i=0; i<longueur ;i++)
    {
        unsigned int somme = 0;
        // boucle permetant de selectionner les charactères deux à deux
        for(int j=0;j<=1;j++)
        {
            int multiplicateur;
            // si j == 0 alors multiplicateur = 256 puissance 1 soit 256 cer il est le premier
            if (j == 0) {multiplicateur = 256;} 
            // sinon j == 1 alors multiplicateur = 256 puissance 0 soit 1 car il est le dernier
            else {multiplicateur = 1;}
            somme = somme + *(msgConvertis+(i*2+j)) * multiplicateur;
        }
        tabBlocDe2[i] = somme;
    }
}

// Fonction de cryptage décryptage des blocs de 2 charactères de façon RSA
unsigned long int cryptographie(unsigned long int nombreACrypte, int puissance, int modulo)
{
    int multiplicateur = nombreACrypte%modulo;
    for (int i = 2; i <= puissance; i++) {
        nombreACrypte = nombreACrypte * multiplicateur;
        nombreACrypte = nombreACrypte%modulo;
        
    }
    printf ("\n");
    return nombreACrypte;
}

// Règle tous les termes cryptés sur le même nombres de digits
void creationFinaleMessageCrypte( int *tabCrypte, int longueur )
{
    // Tableau final à envoyer
    char * messageFinal = malloc(longueur*6);
    // Tableau temporaire pour la gestion 
    char * temp = malloc(6);
    for (int i=0; i<longueur; i++)
    {
        if (*(tabCrypte+i)<10)
        {
            sprintf(temp,"00000%ld", *(tabCrypte+i));
        }
        else if (*(tabCrypte+i)<100)
        {
            sprintf(temp,"0000%ld", *(tabCrypte+i));
        }
        else if (*(tabCrypte+i)<1000)
        {
            sprintf(temp,"000%ld", *(tabCrypte+i));
        }
        else if (*(tabCrypte+i)<10000)
        {
            sprintf(temp,"00%ld", *(tabCrypte+i));
        }
        else if (*(tabCrypte+i)<100000)
        {
            sprintf(temp,"0%ld", *(tabCrypte+i));
        }
        else sprintf(temp,"%ld", *(tabCrypte+i));
        strcat(messageFinal, temp);   
    printf(" %s \n \n", messageFinal);
    }
}


void ecrire(FILE *original, int *tableauCrypte, int longueur)
{
    if(original == NULL) 
        printf("ouverture du fichier %13s impossible \n");
    else
    {
        printf("Transcription des blocs cryptés vers fichier texte ...\n");
        for(int i = 0; i < longueur; i++)
        {
            fprintf(original,"%d",*(tableauCrypte+i));
        }
        printf("Transcription terminée !!!!!!\n");
        fclose(original);
    }  
}

void copierDansFichier(int *tableauCrypte, int longueur)
{
    FILE *original = fopen("original.txt", "w");
    if(original == NULL)
    {
        printf("Erreur de création \n"); 
        fclose(original);
    }
    else
    {
        ecrire(original, tableauCrypte, longueur);
    }
}

void lireFichier(int *tableauADecrypter)
{
    FILE *toRead = fopen("original.txt", "r");
    if(toRead == NULL)
    {
        printf("Erreur de lecture, le fichier n'a pas été trouvé?\n");
        fclose(toRead);
    }
    else
    {
        printf("Lecture du fichier... \n \n");
        int longueur_toRead;
        int caractere;
        
         
        int i = 0;
        while ((caractere = getc(toRead)) != EOF)
        {
            longueur_toRead++;
            //*(tableauADecrypter+i) = caractere;
            //printf("Voici le caractère %d : %d \n", i, *(tableauADecrypter+i));
        }
        tableauADecrypter = malloc(longueur_toRead);
        while ((caractere = getc(toRead)) != EOF)
        {
               
            *(tableauADecrypter+i) = caractere;
            printf("Voici le caractère %d : %d \n", i, *(tableauADecrypter+i));
            i++;
        }
        fclose(toRead);
         
    }
}

    
int main()
{
    srand(time(NULL));
    printf ("\n");
    int e, premierNombrePremier, deuxiemeNombrePremier, n, d = 0;
    // Boucle permettant de tous relancer si d prend trop de temps à être généré
    while ( d == 0)
    {
        n = 0;
        // Boucle permettant de relancer les nombres premiers si n est trop petit
        while ( n < 131584)
        {
            premierNombrePremier = generationNombrePremier(3,97);
            deuxiemeNombrePremier = generationNombrePremier(200,3000);
            n = getN(premierNombrePremier, deuxiemeNombrePremier);
        }
        int phi = (premierNombrePremier-1)*(deuxiemeNombrePremier-1);
        e = getE(premierNombrePremier, deuxiemeNombrePremier, phi);
        d = getD(phi,e);
    }
    // Simulation par print des envoies de clefs
    printf("Voici premierNombrePremier : %d \n", premierNombrePremier);
    printf("Voici deuxiemeNombrePremier : %d \n", deuxiemeNombrePremier);
    printf("Voici n : %d \n", n);
    printf("Voici e : %d \n", e);
    printf("Voici d : %d \n", d);
    printf ("\n");
        
    
    // Saisit du message à envoyer
    char *msgConvertis = malloc(4096);
    char *msgAConvertir = malloc(4096);        
    printf("Message a convertir : ");
    fgets( msgAConvertir, 4096, stdin);
    msgAConvertir = realloc( msgAConvertir, strlen( msgAConvertir) );
   
    convertToASCII( msgAConvertir, msgConvertis );
    
    // Début de la conversion d'une table de charactère de Int à une table de Int par bloc de 2 caractère
    int longueur = getLongueur(msgAConvertir);
    unsigned int *tabBlocDe2 = malloc(longueur);
    
    // Création d'un tableau contenant les blocs de 2 caractères
    creerBlocs ( msgConvertis, tabBlocDe2, longueur);
    for (int i = 0; i < longueur; i++) printf("\n Voici tabBlocDe2 crypté numero %d : %d ",i, *(tabBlocDe2+i));
    printf ("\n");
    
    // Création d'un tableau contenant les blocs de 2 caractères crypté
    unsigned int *tabCrypte = malloc(longueur);
    
    for (int i = 0; i < longueur;i++)
    {
        *(tabCrypte+i) = cryptographie(*(tabBlocDe2+i), e, n);
    }
    for (int i = 0; i < longueur; i++) printf("\n Voici tabCrypte numero %d : %ld ",i, *(tabCrypte+i));
    printf ("\n");
    
    // Création d'une phrase crypté pour l'envoie
    creationFinaleMessageCrypte(tabCrypte, longueur);
    
        // Envoie de la string au destinataire
    
        // Reception de la string que envoyé au destinataire
    
    for (int i = 0; i < longueur;i++)
    {
        *(tabBlocDe2+i) = cryptographie(*(tabCrypte+i), d, n);
    }
    for (int i = 0; i < longueur; i++) printf("\n Voici tabBlocDe2 décrypté numero %d : %d ",i, *(tabBlocDe2+i));
    printf ("\n");
    
    printf ("\n");
    
    copierDansFichier(tabCrypte, longueur);
    lireFichier(tabCrypte);
    return (EXIT_SUCCESS);
}
