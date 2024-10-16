+#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_GRILLE 8
#define NB_BATEAUX 3
#define JOUEUR_1 1
#define JOUEUR_2 2

typedef struct {
    int grille[TAILLE_GRILLE][TAILLE_GRILLE];
} Grille;

void initialiserGrille(Grille *grille) {
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            grille->grille[i][j] = 0;
        }
    }
}

void afficherGrille(Grille *grille) {
    printf("  ");
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            if (grille->grille[i][j] == 0)
                printf("~ ");
            else if (grille->grille[i][j] == -1)
                printf("X ");
            else
                printf("B ");
        }
        printf("\n");
    }
}

int placerBateauManuellement(Grille *grille, int taille) {
    int x, y, orientation;
    printf("Placer un bateau de taille %d\n", taille);
    printf("Entrez les coordonnées de départ (x y) : ");
    scanf("%d %d", &x, &y);
    printf("Choisissez l'orientation (0 pour horizontal, 1 pour vertical) : ");
    scanf("%d", &orientation);

    if (orientation == 0) { // Horizontal
        if (y + taille > TAILLE_GRILLE) {
            printf("Le bateau dépasse la grille. Choisissez un autre emplacement.\n");
            return 0; // Échec
        }
        for (int i = 0; i < taille; i++) {
            if (grille->grille[x][y + i] != 0) {
                printf("Le bateau chevauche un autre bateau. Choisissez un autre emplacement.\n");
                return 0; // Échec
            }
        }
        for (int i = 0; i < taille; i++) {
            grille->grille[x][y + i] = taille;
        }
    } else { // Vertical
        if (x + taille > TAILLE_GRILLE) {
            printf("Le bateau dépasse la grille. Choisissez un autre emplacement.\n");
            return 0; // Échec
        }
        for (int i = 0; i < taille; i++) {
            if (grille->grille[x + i][y] != 0) {
                printf("Le bateau chevauche un autre bateau. Choisissez un autre emplacement.\n");
                return 0; // Échec
            }
        }
        for (int i = 0; i < taille; i++) {
            grille->grille[x + i][y] = taille;
        }
    }
    return 1; // Succès
}

void placerBateaux(Grille *grille) {
    int tailles_bateaux[NB_BATEAUX] = {2, 3, 4};
    for (int i = 0; i < NB_BATEAUX; i++) {
        int place = 0;
        while (!place) {
            place = placerBateauManuellement(grille, tailles_bateaux[i]);
        }
    }
}

int tirer(Grille *grille, int x, int y) {
    if (grille->grille[x][y] > 0) {
        int tailleBateau = grille->grille[x][y];
        
        // Parcourir la grille et marquer tout le bateau comme coulé
        for (int i = 0; i < TAILLE_GRILLE; i++) {
            for (int j = 0; j < TAILLE_GRILLE; j++) {
                if (grille->grille[i][j] == tailleBateau) {
                    grille->grille[i][j] = -1; // Marquer la case comme coulé
                }
            }
        }
        return 1; // Le bateau a été touché et coulé
    }
    return 0; // Manqué
}

int estCoule(Grille *grille, int taille) {
    // Vérifie si tous les segments du bateau de taille donnée sont touchés
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            if (grille->grille[i][j] == taille) {
                return 0; // Le bateau n'est pas complètement coulé
            }
        }
    }
    return 1; // Le bateau est coulé
}

int estGagne(Grille *grille) {
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            if (grille->grille[i][j] > 0) {
                // S'il y a encore une case qui n'est pas coulé (> 0), la partie continue
                return 0;
            }
        }
    }
    // Si toutes les cases de bateau sont coulées, le joueur a perdu
    return 1;
}


void sauvegarderPartie(Grille *grilleJ1, Grille *grilleJ2, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "wb");
    if (fichier == NULL) {
        printf("Erreur lors de la sauvegarde de la partie.\n");
        return;
    }
    fwrite(grilleJ1, sizeof(Grille), 1, fichier);
    fwrite(grilleJ2, sizeof(Grille), 1, fichier);
    fclose(fichier);
    printf("Partie sauvegardée.\n");
}

void chargerPartie(Grille *grilleJ1, Grille *grilleJ2, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rb");
    if (fichier == NULL) {
        printf("Erreur lors du chargement de la partie.\n");
        return;
    }
    fread(grilleJ1, sizeof(Grille), 1, fichier);
    fread(grilleJ2, sizeof(Grille), 1, fichier);
    fclose(fichier);
    printf("Partie chargée.\n");
}

void jouerTour(Grille *grilleAdverse, int joueur, Grille *grilleJ1, Grille *grilleJ2) {
    int choix;
    int x, y;

    while (1) {
        printf("Joueur %d, que voulez-vous faire ?\n", joueur);
        printf("1. Tirer\n2. Sauvegarder la partie\nChoix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            // Tirer
            printf("Entrez les coordonnées (x y) de votre tir : ");
            scanf("%d %d", &x, &y);
            if (x < 0 || x >= TAILLE_GRILLE || y < 0 || y >= TAILLE_GRILLE) {
                printf("Coordonnées invalides. Réessayez.\n");
            } else {
                if (tirer(grilleAdverse, x, y)) {
                    printf("Touché et coulé !\n");
                } else {
                    printf("Manqué.\n");
                }
                break; // Fin du tour
            }
        } else if (choix == 2) {
            // Sauvegarder la partie
            sauvegarderPartie(grilleJ1, grilleJ2, "sauvegarde.dat");
            printf("La partie a été sauvegardée.\n");
        } else {
            printf("Choix invalide. Réessayez.\n");
        }
    }
}

void jeu(Grille *grilleJ1, Grille *grilleJ2) {
    int tour = JOUEUR_1;

    while (1) {
        printf("\nGrille du joueur %d :\n", JOUEUR_1);
        afficherGrille(grilleJ1);
        printf("\nGrille du joueur %d :\n", JOUEUR_2);
        afficherGrille(grilleJ2);

        if (tour == JOUEUR_1) {
            jouerTour(grilleJ2, JOUEUR_1, grilleJ1, grilleJ2);
            if (estGagne(grilleJ2)) {
                printf("Joueur 1 a gagné !\n");
                break; // Fin du jeu
            }
            tour = JOUEUR_2;
        } else {
            jouerTour(grilleJ1, JOUEUR_2, grilleJ1, grilleJ2);
            if (estGagne(grilleJ1)) {
                printf("Joueur 2 a gagné !\n");
                break; // Fin du jeu
            }
            tour = JOUEUR_1;
        }
    }
}



int main() {
    srand(time(NULL));

    int choix;
    Grille grilleJ1, grilleJ2;
    
    printf("1. Nouvelle partie\n2. Charger une partie\nChoix : ");
    scanf("%d", &choix);

    if (choix == 2) {
        chargerPartie(&grilleJ1, &grilleJ2, "sauvegarde.dat");
    } else {
        initialiserGrille(&grilleJ1);
        initialiserGrille(&grilleJ2);

        printf("Joueur 1, placez vos bateaux.\n");
        placerBateaux(&grilleJ1);
        printf("Joueur 2, placez vos bateaux.\n");
                placerBateaux(&grilleJ2);

        printf("\nLes deux joueurs ont placé leurs bateaux. Début de la partie !\n");
    }

    // Boucle principale du jeu
    jeu(&grilleJ1, &grilleJ2);

    // À la fin de la partie, possibilité de sauvegarder
    printf("Voulez-vous sauvegarder la partie (1 pour oui, 0 pour non) ? ");
    scanf("%d", &choix);
    if (choix == 1) {
        sauvegarderPartie(&grilleJ1, &grilleJ2, "sauvegarde.dat");
    }

    return 0;
}
