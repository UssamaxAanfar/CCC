#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// les structures utilisées
typedef struct Auteur
{
    char nom[100];
    char prenom[100];
    char cin[10];
    int age;
    char nationalite[100];
} Auteur;

typedef struct Livre
{
    char titre[100];
    int cote;
    int annee_de_edition;
    char categorie[100];
    Auteur auteur;
} Livre;

int numLivres = 0;

// les fonctions principales
void menu(Livre **tableauLivres);
void AjouterLivre(Livre **tableauLivres);
void SupprimerLivre(Livre **tableauLivres);
void AfficherBibliotheque(Livre **tableauLivres);
void AfficherLivres(Livre **tableauLivres);
void auteurPlusDeLivres(Livre **tableauLivres);
void Quitter(Livre **tableauLivres);

// les fonctions secondaires
void fillUpInfo(Livre **tableauLivres, int pos);
void insertion(Livre **tableauLivres, int pos);
void Afficher(Livre **tableauLivres);
void lireLigne(char *str, int taillestr);
void Sup_Livre(Livre **tableauLivres, int pos);
int authorExists(Livre **tableauLivres, char *cin);

int authorExists(Livre **tableauLivres, char *cin)
{
    for (int i = 0; i < numLivres; i++)
    {
        if (strcmp((*tableauLivres + i)->auteur.cin, cin) == 0)
            return 1;
    }
    return 0;
}

void auteurPlusDeLivres(Livre **tableauLivres)
{
    int i, max, precedent = 0, pos = 0;
    for (i = 0; i < numLivres; i++)
    {
        max = 0;
        for (int j = 0; j < numLivres; j++)
        {
            if (strcmp((*tableauLivres + i)->auteur.cin, (*tableauLivres + j)->auteur.cin) == 0)
                max++;
        }
        if (max > precedent) // si le nombre de livres de l'auteur est superieur au precedent
        {
            precedent = max;
            pos = i;
        }
    }
    printf("L'auteur ayant le plus de livres est %s %s avec %d livres\n", (*tableauLivres + pos)->auteur.nom, (*tableauLivres + pos)->auteur.prenom, precedent);
    menu(tableauLivres);
}

void lireLigne(char *str, int taillestr)
{
    fgets(str, taillestr, stdin);
    // Supprimer les caractères suivants le dernier caractère non nul
    if (str[0] != '\0' && str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
}

void fillUpInfo(Livre **tableauLivres, int pos)
{
    numLivres++;
    *tableauLivres = (Livre *)realloc(*tableauLivres, numLivres * sizeof(Livre));
    if (tableauLivres == NULL)
    {
        printf("Erreur Reallocation de memoire\n");
        exit(1);
    }

    while (getchar() != '\n')
        ;
    printf("Donner le titre du livre : ");
    lireLigne((*tableauLivres + pos)->titre, 100);

    printf("Donner la categorie du livre : ");
    lireLigne((*tableauLivres + pos)->categorie, 100);

    printf("Donner l'annee d'edition du livre : ");
    scanf("%d", &(*tableauLivres + pos)->annee_de_edition);
    getchar();

    char tmpCin[10];
    printf("Donner le cin de l'auteur du livre : ");
    lireLigne(tmpCin, 10);
    while (authorExists(tableauLivres, tmpCin) == 1)
    {
        char choix;
        do
        {
            printf("Ce cin existe deja, voulez-vous ajouter un autre livre pour cet auteur ? (o/n) : ");
            scanf(" %c", &choix);
        } while (choix != 'o' && choix != 'n');
        while (getchar() != '\n')
            ;

        if (choix == 'o')
        {
            // remplacer les informations de l'auteur
            for (int i = 0; i < numLivres; i++)
            {
                if (strcmp((*tableauLivres + i)->auteur.cin, tmpCin) == 0)
                    (*tableauLivres + pos)->auteur = (*tableauLivres + i)->auteur;
            }
            goto end;
        }
        else if (choix == 'n')
        {
            printf("Ce cin existe deja, donner un autre cin : ");
            lireLigne(tmpCin, 10);
            if (authorExists(tableauLivres, tmpCin) == 0)
                break;
        }
    }
    strcpy((*tableauLivres + pos)->auteur.cin, tmpCin);

    printf("Donner le nom de l'auteur du livre : ");
    lireLigne((*tableauLivres + pos)->auteur.nom, 100);

    printf("Donner le prenom de l'auteur du livre : ");
    lireLigne((*tableauLivres + pos)->auteur.prenom, 100);

    printf("Donner l'age de l'auteur du livre : ");
    scanf("%d", &(*tableauLivres + pos)->auteur.age);
    while (getchar() != '\n')
        ;

    printf("Donner la nationalite de l'auteur du livre : ");
    lireLigne((*tableauLivres + pos)->auteur.nationalite, 100);
    (*tableauLivres + pos)->cote = numLivres;

end:
    (*tableauLivres + pos)->cote = numLivres;
}

void insertion(Livre **tableauLivres, int pos)
{
    for (int i = numLivres - 1; i > pos; i--)
    {
        Livre temp = *(*tableauLivres + i);
        *(*tableauLivres + i) = *(*tableauLivres + i - 1);
        *(*tableauLivres + i - 1) = temp;
    }
}

void Sup_Livre(Livre **tableauLivres, int pos)
{
    for (int i = pos; i < numLivres - 1; i++)
    {
        Livre temp = *(*tableauLivres + i);
        *(*tableauLivres + i) = *(*tableauLivres + i + 1);
        *(*tableauLivres + i + 1) = temp;
    }
}

void Afficher(Livre **tableauLivres)
{
    for (int i = 0; i < numLivres; i++)
    {
        printf("******************\n");
        printf("Titre : %s\n", (*tableauLivres + i)->titre);
        printf("Cote : %d\n", (*tableauLivres + i)->cote);
        printf("Annee d'edition : %d\n", (*tableauLivres + i)->annee_de_edition);
        printf("Categorie : %s\n", (*tableauLivres + i)->categorie);
        printf("Auteur : %s %s\n", (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
    }
}

void AfficherBibliotheque(Livre **tableauLivres)
{
    if (numLivres == 0)
        printf("La bibliotheque est vide\n");
    else
    {
        printf("******************\n");
        for (int i = 0; i < numLivres; i++)
        {
            printf("+ Titre : %s , Ce livre est ecrit par %s %s\n", (*tableauLivres + i)->titre, (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
        }
    }
    menu(tableauLivres);
}

void AfficherLivres(Livre **tableauLivres)
{
    if (numLivres == 0)
    {
        printf("La bibliotheque est vide\n");
        menu(tableauLivres);
    }

    char choix;
    printf("a. Par auteur(cin)\n");
    printf("b. Par titre\n");
    printf("c. Par annee d'edition\n");
    do
    {
        printf("=> ");
        scanf(" %c", &choix);
    } while (choix != 'a' && choix != 'b' && choix != 'c');

    // par auteurs(cin)
    if (choix == 'a')
    {
        char cin[10];
        printf("Les livres par auteurs(cin) :");
        scanf("%s", cin);

        printf("%s\n", cin);

        for (int i = 0; i < numLivres; i++)
        {
            if (strcmp((*tableauLivres + i)->auteur.cin, cin) == 0)
            {
                printf("******************\n");
                printf("Titre : %s\n", (*tableauLivres + i)->titre);
                printf("Cote : %d\n", (*tableauLivres + i)->cote);
                printf("Annee d'edition : %d\n", (*tableauLivres + i)->annee_de_edition);
                printf("Categorie : %s\n", (*tableauLivres + i)->categorie);
                printf("Auteur : %s %s\n", (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
            }
        }
    }
    else if (choix == 'b') // par titre
    {
        char titre[100];
        printf("Les livres par titre :");
        scanf("%s", titre);

        for (int i = 0; i < numLivres; i++)
        {
            if (strcmp((*tableauLivres + i)->titre, titre) == 0)
            {
                printf("******************\n");
                printf("Titre : %s\n", (*tableauLivres + i)->titre);
                printf("Cote : %d\n", (*tableauLivres + i)->cote);
                printf("Annee d'edition : %d\n", (*tableauLivres + i)->annee_de_edition);
                printf("Categorie : %s\n", (*tableauLivres + i)->categorie);
                printf("Auteur : %s %s\n", (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
            }
        }
    }
    else if (choix == 'c') // par annee d'edition
    {
        int annee;
        printf("Les livres par annee d'edition :");
        scanf("%d", &annee);

        for (int i = 0; i < numLivres; i++)
        {
            if ((*tableauLivres + i)->annee_de_edition == annee)
            {
                printf("******************\n");
                printf("Titre : %s\n", (*tableauLivres + i)->titre);
                printf("Cote : %d\n", (*tableauLivres + i)->cote);
                printf("Annee d'edition : %d\n", (*tableauLivres + i)->annee_de_edition);
                printf("Categorie : %s\n", (*tableauLivres + i)->categorie);
                printf("Auteur : %s %s\n", (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
            }
        }
    }
}

void AjouterLivre(Livre **tableauLivres)
{
    char choix;
    printf("a. Au debut du tableau\n");
    printf("b. A la fin du tableau\n");
    printf("c. A une position donnee\n");
    printf("d. Apres un livre identifie par sa cote\n");

    do
    {
        printf("=> ");
        scanf(" %c", &choix);
    } while (choix != 'a' && choix != 'b' && choix != 'c' && choix != 'd');

    if (choix == 'a')
    {
        fillUpInfo(tableauLivres, numLivres);
        insertion(tableauLivres, 0);
        Afficher(tableauLivres);
        menu(tableauLivres);
    }
    else if (choix == 'b')
    {
        fillUpInfo(tableauLivres, numLivres);
        insertion(tableauLivres, numLivres);
        Afficher(tableauLivres);
        menu(tableauLivres);
    }
    else if (choix == 'c')
    {
        int pos;
        do
        {
            printf("Donner la position : ");
            scanf("%d", &pos);
        } while (pos < 1 || pos > numLivres);

        fillUpInfo(tableauLivres, numLivres);
        insertion(tableauLivres, pos - 1);
        Afficher(tableauLivres);
        menu(tableauLivres);
    }
    else if (choix == 'd')
    {
        int cote;
        do
        {
            printf("Donner la cote : ");
            scanf("%d", &cote);
        } while (cote < 1 || cote > numLivres);

        for (int i = 0; i < numLivres; i++)
        {
            if ((*tableauLivres + i)->cote == cote)
            {
                fillUpInfo(tableauLivres, numLivres);
                insertion(tableauLivres, i + 1);
                Afficher(tableauLivres);
                menu(tableauLivres);
            }
        }
    }
}

void SupprimerLivre(Livre **tableauLivres)
{
    char choix;
    printf("a. Au debut\n");
    printf("b. A la fin\n");
    printf("c. En une position donnee\n");
    printf("d. Par titre\n");
    printf("e. Par auteurs(par son cin)\n");
    printf("f. Dont le titre commence par une lettre donnee\n");

    do
    {
        printf("=> ");
        scanf(" %c", &choix);
    } while (choix != 'a' && choix != 'b' && choix != 'c' && choix != 'd' && choix != 'e' && choix != 'f');

    if (choix == 'a')
    {
        Sup_Livre(tableauLivres, 0);
    }
    else if (choix == 'b')
    {
        Sup_Livre(tableauLivres, numLivres - 1);
    }
    else if (choix == 'c')
    {
        int pos;
        do
        {
            printf("Donner la position : ");
            scanf("%d", &pos);
        } while (pos < 1 || pos > numLivres);
        Sup_Livre(tableauLivres, pos - 1);
    }
    else if (choix == 'd')
    {
        char titre[100];
        printf("Donner le titre : ");
        lireLigne(titre, 100);
        for (int i = 0; i < numLivres; i++)
        {
            if (strcmp((*tableauLivres + i)->titre, titre) == 0)
                Sup_Livre(tableauLivres, i);
        }
    }
    else if (choix == 'e')
    {
        char cin[10];
        printf("Donner le cin : ");
        scanf("%s", cin);
        for (int i = 0; i < numLivres; i++)
        {
            if (strcmp((*tableauLivres + i)->auteur.cin, cin) == 0)
                Sup_Livre(tableauLivres, i);
        }
    }
    else if (choix == 'f')
    {
        char lettre;
        printf("Donner la lettre : ");
        scanf(" %c", &lettre);
        for (int i = 0; i < numLivres; i++)
        {
            if ((*tableauLivres + i)->titre[0] == lettre)
                Sup_Livre(tableauLivres, i);
        }
    }
    numLivres--; // decrementer le nombre de livres car on a supprimer un livre
    *tableauLivres = (Livre *)realloc(*tableauLivres, numLivres * sizeof(Livre));
    menu(tableauLivres);
}

void RechercherLivre(Livre **tableauLivres) {
    char choix;
    printf("a. Par titre\n");
    printf("b. Par auteur (CIN)\n");

   
        scanf(" %c", &choix);
    

    if (choix =='a') { // Search by title
        char titre[100];
        printf("Les livres par titre :");
        scanf("%s", titre);
int found=0;
        for (int i = 0; i < numLivres; i++)
        {
            if (strcmp((*tableauLivres + i)->titre, titre) == 0)
            {
                found=1;
                printf("******************\n");
                printf("Titre : %s\n", (*tableauLivres + i)->titre);
                printf("Cote : %d\n", (*tableauLivres + i)->cote);
                printf("Annee d'edition : %d\n", (*tableauLivres + i)->annee_de_edition);
                printf("Categorie : %s\n", (*tableauLivres + i)->categorie);
                printf("Auteur : %s %s\n", (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
            }
        }
        if (!found) {
            printf("Aucun livre avec ce titre n'a ete trouve.\n");
        }
    } else if (choix == 'b') { // Search by author's CIN
        char cin[10];
        printf("Entrez le CIN de l'auteur : ");
        scanf("%s", cin);

        int found = 0;
        for (int i = 0; i < numLivres; i++) {
            if (strcmp((*tableauLivres + i)->auteur.cin, cin) == 0) {
                found = 1;
                printf("******************\n");
                printf("Titre : %s\n", (*tableauLivres + i)->titre);
                printf("Cote : %d\n", (*tableauLivres + i)->cote);
                printf("Annee d'edition : %d\n", (*tableauLivres + i)->annee_de_edition);
                printf("Categorie : %s\n", (*tableauLivres + i)->categorie);
                printf("Auteur : %s %s\n", (*tableauLivres + i)->auteur.nom, (*tableauLivres + i)->auteur.prenom);
            }
        }
        if (!found) {
            printf("Aucun livre de cet auteur n'a ete trouve.\n");
        }
    }
    menu(tableauLivres);
}

void TrierLivres(Livre **tableauLivres) {
    char choix;
    printf("a. Trier par titre\n");
    printf("b. Trier par categorie\n");

    do {
        printf("=> ");
        scanf(" %c", &choix);
    } while (choix != 'a' && choix != 'b');

    if (choix == 'a') { // Sort by title
        // Bubble sort algorithm
        for (int i = 0; i < numLivres - 1; i++) {
            for (int j = 0; j < numLivres - i - 1; j++) {
                if (strcmp((*tableauLivres + j)->titre, (*tableauLivres + j + 1)->titre) > 0) {
                    // Swap books
                    Livre temp = *(*tableauLivres + j);
                    *(*tableauLivres + j) = *(*tableauLivres + j + 1);
                    *(*tableauLivres + j + 1) = temp;
                }
            }
        }
    } else if (choix == 'b') { // Sort by category
        // Bubble sort algorithm
        for (int i = 0; i < numLivres - 1; i++) {
            for (int j = 0; j < numLivres - i - 1; j++) {
                if (strcmp((*tableauLivres + j)->categorie, (*tableauLivres + j + 1)->categorie) > 0) {
                    // Swap books
                    Livre temp = *(*tableauLivres + j);
                    *(*tableauLivres + j) = *(*tableauLivres + j + 1);
                    *(*tableauLivres + j + 1) = temp;
                }
            }
        }
    }

    printf("Livres tries :\n");
    Afficher(tableauLivres);
    menu(tableauLivres);
}

void Quitter(Livre **tableauLivres)
{
    char choix;
    do
    {
        printf("Voulez-vous vraiment quitter ? (o/n) : ");
        scanf(" %c", &choix);
    } while (choix != 'o' && choix != 'n');
    if (choix == 'o')
    {
        printf("Merci d'avoir utiliser notre application\n");
        free(tableauLivres);
    }
    else if (choix == 'n')
        menu(tableauLivres);
}

void menu(Livre **tableauLivres)
{
    int choix;
    printf("################## Menu Gestion des livres ##################\n");
    printf("1. Afficher bibliotheque\n");
    printf("2. Afficher les informations de l'auteur ayant le plus de livres\n");
    printf("3. Afficher les livres\n");
    printf("4. Ajouter un livre\n");
    printf("5. Supprimer un livre\n");
    printf("6. recherche un livre\n");
    printf("7. trier les livres\n");
    printf("8. Quitter\n");
    printf("############################################################\n");

    do
    {
        printf("=> ");
        scanf("%d", &choix);
    } while (choix < 1 || choix > 8);

    if (choix == 1)
        AfficherBibliotheque(tableauLivres);
    else if (choix == 2)
        auteurPlusDeLivres(tableauLivres);
    else if (choix == 3)
        AfficherLivres(tableauLivres);
    else if (choix == 4)
        AjouterLivre(tableauLivres);
    else if (choix == 5)
        SupprimerLivre(tableauLivres);
    else if (choix == 6)
        RechercherLivre(tableauLivres);
    else if (choix == 7)
        TrierLivres(tableauLivres);
    else if (choix == 8)
        Quitter(tableauLivres);
}

int main() {
    Livre *tableauLivres = NULL; // Initialize the array of books

    // Adding one book initially
    numLivres++; // Increment the number of books
    tableauLivres = (Livre *)realloc(tableauLivres, numLivres * sizeof(Livre)); // Allocate memory for one book
    if (tableauLivres == NULL) {
        printf("Erreur d'allocation de memoire\n");
        exit(1);
    }

    // Fill up information for the first book
    strcpy(tableauLivres[0].titre, "prince");
    tableauLivres[0].cote = 1;
    tableauLivres[0].annee_de_edition = 1943;
    strcpy(tableauLivres[0].categorie, "conte");
    strcpy(tableauLivres[0].auteur.nom, "saint-exupery");
    strcpy(tableauLivres[0].auteur.prenom, "antoine de");
    strcpy(tableauLivres[0].auteur.cin, "1234567890");
    tableauLivres[0].auteur.age = 44;
    strcpy(tableauLivres[0].auteur.nationalite, "french");

    printf("Initial book added successfully!\n");

    // Display the menu
    menu(&tableauLivres);

    return 0;
}
