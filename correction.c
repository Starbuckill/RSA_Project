   
    /*
     * 
     *  Author : Klement Serralta, Corentin Sadowski, Gabin Lhomme
     *  Creation : 05/01/2018
     *  Debugger :
     *  Date de debugging :
     * 
    */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

// Retourne un nombre alÃ©atoire de BorneMin Ã  BorneMax 
int genNbPremier(int borneMin, int borneMax)
{
    int nombrePremier = rand()%(borneMax)+(borneMin);
    return nombrePremier; 
}

// Prend un nombre puis ressort un bit qui dÃ©termine si le nombre entier est premier ou non
int testNbPremier(int nombre)
{
    int x = 0; 
    int i = 2;
    int racCarree = sqrt(nombre);
    while ( (x == 0) && (i <= racCarree))
    {
        if ( nombre%i == 0 ) x++;
        else if (i==2)  i++;
        else i += 2;
    }
    // Le nombre n'est pas premier
    if ( x == 1 ) return 0;
    // Le nombre est premier;
    else return 1 ;
}  

// Teste le nombre gÃ©nÃ©rÃ© et ne donne un rÃ©sultat que quand il est premier
int generationNombrePremier(int borneMin, int borneMax)
{
    int nombrePremier = genNbPremier(borneMin, borneMax);
    // Tantque le nombre n'est pas premier, relancer la gÃ©nÃ©ration
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
    // Ne sert Ã  rien de tester des plus grand diviseur
    while ( i <= petitNombre)
    {
        if ( grandNombre%i == 0 && petitNombre%i == 0) x = i;
        i++;
    }
    return x;
}

// Donne la valeur de N
unsigned int getN(int pPetit, int qGrand)
{
    unsigned int n = pPetit * qGrand;
    return n;
}

// Fonction qui sert Ã  trouver e, e faisant partie de la clef publique
//unsigned int getE(int pPetit, int qGrand, int phi)
unsigned int getE(int phi,unsigned int n)
{
    // e doit Ãªtre plus petit que phi, d'ou l'initialiser Ã  phi + 1
    int e = phi + 1;
    int nombre = phi;
    while(e > phi)
    {  
        nombre--;
        if( PGCD2Nombres(n,nombre) == 1 ) e = nombre;
        //if(phi%nombreAleatoire != 0 && ppcm%nombreAleatoire != 0) e = nombreAleatoire;
    }
    return e;
}

// Trouver d telque d*e congru Ã  1 modulo phi
int getD (int phi,unsigned int e,unsigned int n) 
{
    int d = phi-1;
    // Application d'une fonction de temps pour Ã©viter de relancer nous mÃªme le programme si le programme dure trop longtemps
    // Peut durer longtemps oÃ¹ mÃªme planter car la recherche du d peut planter car pc pas assez puissant pour les nombres Ã  sortir
    //int temps = time(NULL);
    //int t2 = time(NULL);
    //while (((d*e)%n != 1) && (t2-temps < 10) && (d!=0)) 
    while (((d*e)%n != 1) && (d!=0)) 
    {
        //t2 = time(NULL);
        d--;
    }
    //if (t2-temps >= 10) return 0;
    //else return d;
    return d;
}

// Fontion permettant de transformer la phrase en code ASCII.
void convertToASCII ( char * msgAConvertir, unsigned int * msgConvertis )
{
    for ( int i=0;i<strlen( msgAConvertir );i++) 
    {
        msgConvertis[i] = (unsigned int)msgAConvertir[i];
        printf("code ascii %u \n ",msgConvertis[i]);
    }
}

// Fontion permettant de forcer la phrase en int.
unsigned int* convertToBlocInt ( char * msgAConvertir, unsigned int * msgInt, int longueur)
{
    msgInt = (unsigned int*) malloc(longueur * sizeof (int));
    for ( int i=0; i<strlen( msgAConvertir )/10; i++)
    {
        msgInt[i] = (((unsigned int)msgAConvertir[10*i])-48)*1000000000 
        + (((unsigned int)msgAConvertir[10*i+1])-48)*100000000
        + (((unsigned int)msgAConvertir[10*i+2])-48)*10000000
        + (((unsigned int)msgAConvertir[10*i+3])-48)*1000000
        + (((unsigned int)msgAConvertir[10*i+4])-48)*100000
        + (((unsigned int)msgAConvertir[10*i+5])-48)*10000
        + (((unsigned int)msgAConvertir[10*i+6])-48)*1000
        + (((unsigned int)msgAConvertir[10*i+7])-48)*100
        + (((unsigned int)msgAConvertir[10*i+8])-48)*10
        + (((unsigned int)msgAConvertir[10*i+9])-48);
    }
    return msgInt;
}

unsigned int* toBloc2AtASCII (unsigned int * tabBlocDe2, unsigned int * tabASCII, int longueur)
{
    int temp;
    for (int i = 0; i < longueur; i++)
    {
        temp = *(tabBlocDe2+i)/256;
        *(tabASCII+(2*i)) = temp;
        if (temp * 256 != *(tabBlocDe2))
        {
            temp = *(tabBlocDe2+i) - 256 * temp;
            *(tabASCII+(2*i+1)) = temp;
        }
        else *(tabASCII+(2*i+1)) = 10;
    }
    return tabASCII;
}

// Fonction de cryptage dÃ©cryptage des blocs de 2 charactÃ¨res de faÃ§on RSA
unsigned int cryptage (unsigned int nombreACrypte, unsigned int puissance, unsigned int modulo){
    if (puissance == 0) return 1;
    else if (puissance%2 == 0) return ((unsigned int)(pow((double) cryptage(nombreACrypte,puissance/2,modulo),2.))%modulo);
    else return ((nombreACrypte * cryptage(nombreACrypte,puissance-1,modulo))%modulo);
}

// Fonction de cryptage dÃ©cryptage des blocs de 2 charactÃ¨res de faÃ§on RSA
unsigned int decryptage (unsigned int nombreACrypte, unsigned int puissance, unsigned int modulo){
    if (puissance == 0) return 1;
    else if (puissance%2 == 0) return ((unsigned int)(pow((double) decryptage(nombreACrypte,puissance/2,modulo),2.))%modulo);
    else return ((nombreACrypte * decryptage(nombreACrypte,puissance-1,modulo))%modulo);
}


// RÃ¨gle tous les termes cryptÃ©s sur le mÃªme nombres de digits
void creationFinaleMessageCrypte( int *tabCrypte, int longueur )
{
    // Tableau final Ã  envoyer
    unsigned char * messageFinal = (unsigned char*) malloc(longueur*10);
    // Tableau temporaire pour la gestion 
    char * temp = (char*) malloc(10);
    for (int i=0; i<longueur; i++)
    {
        if (*(tabCrypte+i)<10) sprintf(temp,"000000000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<100) sprintf(temp,"00000000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<1000) sprintf(temp,"0000000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<10000) sprintf(temp,"000000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<100000) sprintf(temp,"00000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<1000000) sprintf(temp,"0000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<10000000) sprintf(temp,"000%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<100000000) sprintf(temp,"00%d", *(tabCrypte+i));
        else if (*(tabCrypte+i)<1000000000) sprintf(temp,"0%d", *(tabCrypte+i));
        else sprintf(temp,"%d", *(tabCrypte+i));
        strcat(messageFinal, temp);
    }
    printf("\n message a envoyer : \n %s \n ", messageFinal);
}

void creationFinaleMessageDecrypte( unsigned int *tabASCII, int longueur )
{
    // Tableau final Ã afficher
    char * messageFinal = (char*) malloc(longueur + 1);
    // Tableau temporaire pour la gestion 
    for (int i=0; i<longueur; i++) *(messageFinal+i) = (char*)(*(tabASCII+i));
    *(messageFinal+longueur) = "\0";
    printf("\n message recu : \n %s \n ", messageFinal);
}

// fonction qui sert a trouver la longueur du message a convertir, celle-ci servira a allouer la taille du tableau Ã  blocs.
int getLongueur (char * msgAConvertir)
{
    int longueur = strlen(msgAConvertir)-1;
    if(longueur%2 != 0) longueur = (longueur / 2)+1; 
    else longueur = longueur /2; 
    return longueur;
}

// Fonction permettant de transformer un tableau de charactÃ¨re de Int en Bloc de Int contenant Ã  l'origine les codes ascii de 2 charactÃ¨res
unsigned int* creerBlocs ( int * msgConvertis, unsigned int * tabBlocDe2, int longueur) 
{
    tabBlocDe2 = (unsigned int*) malloc(longueur * sizeof (int));
    // boucle permettant de remplir le tableau de bloc
    int k = 0;
    for(int i=0; i<longueur*2 ;i+=2)
    {
        int j = i;
        unsigned int somme = 0;
        somme = somme + (*(msgConvertis+j) * 256 + *(msgConvertis+j+1));
        tabBlocDe2[k++] = somme;
    }
    return tabBlocDe2;
}
    
int main()
{
    srand(time(NULL));
    int longueur, premierNombrePremier, deuxiemeNombrePremier, d = 0;
    unsigned int e,n;
    int choix = 1;
    while (choix != 0)
    {
        char *msgAConvertir = (char*) malloc(4096);
        printf ("\n\n");
        if (choix == 1)
        {
            printf (" Tapez 1 pour générer des clefs aléatoires. \n Tapez 2 pour insérer des clefs définies. \n \t choix : ");
            scanf ("%d",&choix);
            if (choix == 1 )
            {
                int puissance = 1;
                d = 0;
                // Boucle permettant de tous relancer si d prend trop de temps Ã  Ãªtre gÃ©nÃ©rÃ©
                while ( d == 0)
                {
                    n = 0;
                    // Boucle permettant de relancer les nombres premiers si n est trop petit
                    while ( puissance < 17)
                    {
                        //premierNombrePremier = generationNombrePremier(3,97);
                        //deuxiemeNombrePremier = generationNombrePremier(200,5000);
                        premierNombrePremier = 503;
                        deuxiemeNombrePremier = 563;
                        n = getN(premierNombrePremier, deuxiemeNombrePremier);
                        while (n-pow(2,puissance)>0) puissance++;
                        puissance--;
                    }
                    int phi = (premierNombrePremier-1)*(deuxiemeNombrePremier-1);
                    e = getE(phi,n);
                    d = getD(phi,e,n);
                    if(e<deuxiemeNombrePremier || d<deuxiemeNombrePremier) d = 0;
                    printf("dans le while d == 0\n");
                }
                // Simulation par print des envoies de clefs
                printf("Voici premierNombrePremier : %d \n", premierNombrePremier);
                printf("Voici deuxiemeNombrePremier : %d \n", deuxiemeNombrePremier);
                printf("puissance de n inf : %d ==> %ld\n",puissance,pow(2,puissance));
                printf("Voici n : %u \n", n);
                printf("Voici e : %u \n", e);
                printf("Voici d : %d \n", d);
            }
            else if (choix == 2)
            {
                printf ("Veuillez entrez la clef publique (e , n) et la clef privée (d , n). \n \t Entrez n : ");
                scanf ("%d",&n);
                printf (" \t Entrez e : ");
                scanf ("%d",&e);
                printf (" \t Entrez d : ");
                scanf ("%d",&d);
            }
            else
            { 
                printf (" Vous n'avez pas sélectionné 1 ou 2.");
            }
            choix = -1;
        }
        else if (choix == 2 )
        {
            // Saisit du message Ã envoyer
            printf("Message a crypter : ");
            fgets( msgAConvertir, 4096, stdin);
            fgets( msgAConvertir, 4096, stdin);
            msgAConvertir = (char*) realloc( msgAConvertir, strlen( msgAConvertir) );
            unsigned int *msgConvertis = (unsigned int*) malloc(strlen( msgAConvertir) * sizeof (int)); 
            convertToASCII( msgAConvertir, msgConvertis );
    
            longueur = getLongueur(msgAConvertir);          
            unsigned int *tabBlocDe2;
    
            // CrÃ©ation d'un tableau contenant les blocs de 2 caractÃ¨res
            tabBlocDe2 = creerBlocs ( msgConvertis, tabBlocDe2, longueur);
            for (int i = 0; i < longueur; i++) printf("\n Voici tabBlocDe2 non crypte numero %d : %u ",i, *(tabBlocDe2+i));
            printf ("\n");
    
            // CrÃ©ation d'un tableau contenant les blocs de 2 caractÃ¨res cryptÃ©
            unsigned int *tabCrypte = (unsigned int*) malloc(longueur * sizeof (int));
            for (int i = 0; i < longueur; i++) *(tabCrypte+i) = cryptage(*(tabBlocDe2+i), e, n);
            for (int i = 0; i < longueur; i++) printf("\n Voici tabCrypte numero %d : %u ",i, *(tabCrypte+i));
            printf ("\n");
    
            // CrÃ©ation d'une phrase cryptÃ© pour l'envoie
            creationFinaleMessageCrypte(tabCrypte, longueur);
            choix = -1;
        }
        else if (choix == 3)
        {
            //char *msgConvertis = malloc(1); 
            // Saisit du message Ã envoyer
            printf("Message a decrypter : ");
            fgets( msgAConvertir, 4096, stdin);
            fgets( msgAConvertir, 4096, stdin);
            msgAConvertir = realloc( msgAConvertir, strlen( msgAConvertir) );
            
            //longueur = getLongueur(msgAConvertir)/3;
            longueur = getLongueur(msgAConvertir)/5;
            unsigned int *msgInt;
            msgInt = convertToBlocInt( msgAConvertir, msgInt, longueur);
            unsigned int *tabBlocDe2 = (unsigned int*) malloc(longueur * sizeof (int));
			
            for (int i = 0; i < longueur; i++) printf("\n%u",*(msgInt+i));
            for (int i = 0; i < longueur; i++) *(tabBlocDe2+i) = decryptage(*(msgInt+i), d, n);
            for (int i = 0; i < longueur; i++) printf("\n Voici tabBlocDe2 decrypte numero %d : %u ",i, *(tabBlocDe2+i));
            
            unsigned int *tabASCII;
			tabASCII = (unsigned int*) malloc(longueur*2);
            tabASCII = toBloc2AtASCII(tabBlocDe2, tabASCII, longueur);
            for (int i = 0; i < longueur*2; i++) if(*(tabASCII+longueur-1) != 10) printf("\n Voici tabASCII numero %d : %u ",i, *(tabASCII+i));
			
            creationFinaleMessageDecrypte(tabASCII, longueur*2);
            choix = -1;
        }
        else 
        {
            printf(" Tapez 0 pour sortir. \n Tapez 1 pour générez ou insérer des clés. \n Tapez 2 pour crypter un message. \n Tapez 3 pour décrypter un message. \n Clef publique (%u , %u) \n Clef privée (%d , %u) \n \t choix : ",e,n,d,n);
            scanf ("%d",&choix);
        }
    }
    printf ("\n");
    return (EXIT_SUCCESS);
}

