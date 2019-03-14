#include <iostream>
using namespace std;

#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles.h"	// pour la gestion de l'�cran

// D�claration des constantes
const int nbLignes = 30;
const int nbColonnes = 70;

// Prototy des fonctions ajout�es
int lireEntier();
int lireEntierIntervalle(int min, int max);
void menu();

// Partie I
// --------
void afficherTerrain(int nbLignes, int nbColonnes);
int recupererTouche();
int calculerDirectionTouche(int touche);
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY);
void deplacerSerpentI(int direction, int &posX, int &posY);

// Partie II
// ---------
int saisirNiveau();
void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris);
void afficherSouris(int sourisX[], int sourisY[], int nbSouris);
void deplacerSerpentII(int direction,
	int serpentX[], int serpentY[], int &tailleSerpent,
	int sourisX[], int sourisY[], int &nbSouris);
/*	ou	*/
void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &indiceTete, int &indiceQueue,
	int sourisX[], int sourisY[], int &nbSouris);
bool testerCollision(int x, int y, int sourisX[], int sourisY[], int &nbSouris);
bool testerCollisionSerpent(int serpentX[], int serpentY[], int indiceTete, int indiceQueue);
void dessinerSerpent(int serpentX[], int serpentY[], int indiceTete, int indiceQueue);

// Partie III (BONUS)
// ------------------
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris);
void placerMurs(int nbLignes, int nbColonnes, int nbMurs);

void attendreTemps(int milliseconds);

const int RIGHT = 119;
const int LEFT = 97;
const int UP = 115;
const int DOWN = 100;

const int NBSOURIS = 200;
const int TAILLE_TABLEAU_SERPENT = 50;




//  fonction principale
//  -------------------

int main()
{

	srand(time(NULL));
	int serpentX[TAILLE_TABLEAU_SERPENT], serpentY[TAILLE_TABLEAU_SERPENT];
	bool aGagner = false, gameOver = false;
	int sourisX[NBSOURIS], sourisY[NBSOURIS], choixUtilisateur;
	int nombreDeSouris;
	int tailleSerpent = 1;


	// Pour afficher le menu
	nombreDeSouris = saisirNiveau();
	system("cls");

	// Pour afficher le terrain, la serpent et les souris
	int touchePresse;
	afficherTerrain(nbLignes, nbColonnes);
	positionAleatoire(nbLignes, nbColonnes, serpentX[0], serpentY[0]);
	creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nombreDeSouris);
	afficherSouris(sourisX, sourisY, nombreDeSouris);

	/*initialisation des variables pour le main*/
	int indiceTete = 0;
	int indiceQueue = 0;
	int direction = 1;
	bool movementSouris = false;
	
	/*Boucle du jeau*/
	do
	{

		touchePresse = recupererTouche();
		int nouveauDirection = calculerDirectionTouche(touchePresse);
		dessinerSerpent(serpentX, serpentY, indiceTete, indiceQueue);

		/*Si la nouveau direction present du serpent n'est pas contraire au direction currant, prennez-le, otherwise, nne l'acceptez pas.*/
		if (nouveauDirection != -1)
		{
			if (nouveauDirection == 0 && direction != 1) {
				direction = nouveauDirection;
			}
			else if (nouveauDirection == 1 && direction != 0) {
				direction = nouveauDirection;
			}
			else if (nouveauDirection == 2 && direction != 3) {
				direction = nouveauDirection;
			}
			else if (nouveauDirection == 3 && direction != 2) {
				direction = nouveauDirection;
			}
		}
		deplacerSerpentII(direction, serpentX, serpentY, indiceTete, indiceQueue, sourisX, sourisY, nombreDeSouris);

		/*Game over si on touche le cadre dessin�*/
		if (serpentX[indiceTete] == 0 || serpentX[indiceTete] == nbColonnes || serpentY[indiceTete] == 0 || serpentY[indiceTete] == nbLignes){
			gameOver = true;
		}
		/*Game over si on finisse les souris*/
		if (nombreDeSouris == 0){
			aGagner = true;
		}
		/*Game over si la tete de la serpent touch son corps*/
		if (testerCollisionSerpent(serpentX, serpentY, indiceTete, indiceQueue)){
			gameOver = true;
		}
		/* Essay et commence pour faire bouger les souris (pas implement� ou termin�)*/
		if (movementSouris){
			deplacerSouris(sourisX, sourisY, nombreDeSouris);
			movementSouris = false;
		}
		else {
			movementSouris = true;
		}
		/*on donne du temps pour lire les consignes de Game over ou de scenario gagnant*/
		attendreTemps(200);

	} while (aGagner == false && touchePresse != 27 && gameOver == false);

	/*posicionament des phrases au milieu de la zone de jeu*/
	gotoXY(15, 15);
	if (gameOver)
	{
		cout << "********  GAME OVER!!  *********";
	}
	else
	{
		cout << "BRAVO VOUS AVEZ GAGNE !!!";
	}
	attendreTemps(3000);

	//system("pause");
	return 0;
}

void attendreTemps(int milliseconds)
{
	int currentTime = clock();
	int beginTime = clock();

	while (currentTime - beginTime < milliseconds)
	{
		currentTime = clock();
	}
}

/*
	Validation de saisi pour le niveau de difficult�
*/
int lireEntier()
{
	int valeur;
	cin >> valeur;
	while (cin.fail() || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(512, '\n');
		cout << "Entier uniquement : ";
		cin >> valeur;
	}
	return valeur;
}

/*
	Validation intervalle
*/
int lireEntierIntervalle(int min, int max)
{
	int valeur = lireEntier();
	while (valeur<min || valeur>max)
	{
		cout << "              Seulement entier compris entre " << min << " et " << max << " inclusivement : ";
		valeur = lireEntier();
	}
	return valeur;
}

/*
	Pour afficher le menu
*/
void menu()
{
	cout << "\n\n\n";
	cout << "                        ********** MENU **********" << endl;
	cout << "                    1- Niveau de difficulte (10 souris)" << endl;
	cout << "                    2- Niveau de difficulte (20 souris)" << endl;
	cout << "                    3- Niveau de difficulte (30 souris)" << endl;
}

// ************
// * PARTIE I *
// ************
void afficherTerrain(int nbLignes, int nbColonnes)
/*
T�che: afficher le contour du terrain
Param�tres: le nombre de lignes et de colonnes du terrain rectangulaire
*/
{
	setDimensionFenetre(0, 0, 72, 32);
	cadre(0, 0, nbColonnes, nbLignes, 3);
	curseurVisible(false);
}

int recupererTouche()
/*
T�che: tester si le joueur a appuy� sur une touche
Retour: retourner le code ASCII de la touche press�e, -1 sinon
*/
{
	// 	utiliser la fonction _kbhit() de conio.h qui retourne une valeur non 
	//	nulle si une touche a �t� press�e

	if (_kbhit() != 0)
	{
		return tolower(_getch());
	}
	return -1;
}

int calculerDirectionTouche(int touche)
/*
T�che: calculer la direction correspondant � une touche ou -1
Param�tre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond � la touche
(0: droite, 1: gauche, 2: haut, 3: bas)
*/
{
	switch (touche)
	{
	case 'w':
		touche = 2;
		break;
	case 'a':
		touche = 1;
		break;
	case 's':
		touche = 3;
		break;
	case 'd':
		touche = 0;
		break;
	default:
		return -1;
		break;
	}
	return touche;
}

void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
/*
T�che: calculer une position al�atoire sur le terrain
Param�tres: les dimensions du terrain en entr�e et les coordonn�es de la position al�atoire en sortie
*/
{
	posX = (rand() % (nbColonnes - 1)) + 1;
	posY = (rand() % (nbLignes - 1)) + 1;
}

void deplacerSerpentI(int direction, int &posX, int &posY)
/*
T�che: d�placer le serpent dans la direction
Param�tres: la direction du serpent en entr�e, et la position du serpent en entr�e / sortie.
*/
{
	if (direction == 1) {
		//Je vais vers la gauche
		posX--;
	}
	else if (direction == 0) {
		//Je vais vers la droite
		posX++;
	}
	else if (direction == 2) {
		//Je vais vers le haut
		posY--;
	}
	else if (direction == 3) {
		//Je vais vers le bas
		posY++;
	}
}


// *************
// * PARTIE II *
// *************
int saisirNiveau()
/*
T�che: lire le niveau de difficult� avec tests de validation d'entr�e
Retour: le niveau (= le nombre de souris initialement sur le terrain)
*/
{
	int nombreDeSouris;
	int niveauDeDifficulte;
	menu();
	cout << "                    Quel niveau de jeu ? ";
	niveauDeDifficulte = lireEntierIntervalle(1, 3);

	switch (niveauDeDifficulte)
	{
	case 1:
		nombreDeSouris = 10;
		break;
	case 2:
		nombreDeSouris = 20;
		break;
	case 3:
		nombreDeSouris = 30;
		break;
	default:
		cout << "                    Choix indisponible...";
		break;
	}

	return nombreDeSouris;
}

void creerSouris(int nbLignes, int nbColonnes, int sourisX[], int sourisY[], int nbSouris)
/*
T�che: g�n�rer les nbSouris al�atoirement sur le terrain
Param�tres: les dimensions du terrain, les tableaux de coordonn�es et le
nombre de souris
*/
{
	for (int x = 0; x < nbSouris; x++)
	{
		sourisX[x] = (rand() % (nbColonnes - 1)) + 1;
		sourisY[x] = (rand() % (nbLignes - 1)) + 1;
	}
}

void afficherSouris(int sourisX[], int sourisY[], int nbSouris)
/*
T�che: afficher les souris
Param�tres: les tableaux de coordonn�es et le nombre de souris
*/
{
	for (int i = 0; i < nbSouris; i++)
	{
		gotoXY(sourisX[i], sourisY[i]);
		cout << "M";
	}
}

void deplacerSerpentII(int direction, int serpentX[], int serpentY[], int &tailleSerpent, int sourisX[], int sourisY[], int &nbSouris)
/*
T�che: d�placer le serpent dans la direction
Param�tres: en entr�e : 	la direction du serpent,
en entr�e/sortie :	les tableaux de coordonn�es du serpent et
la taille du serpent.
*/
{
}

/*	ou	*/

void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &indiceTete, int &indiceQueue,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	T�che: d�placer le serpent dans la direction
	Param�tres: en entr�e :	la direction du serpent,
	en entr�e/sortie :	les tableaux de coordonn�es du serpent, les indices de t�te et de queue du serpent.
	*/
{
	/*creation des indices pour localizer la tete et faire un histoire/memoire dedans le tableau serpent*/
	int teteSerpentX = serpentX[indiceTete];
	int teteSerpentY = serpentY[indiceTete];
	deplacerSerpentI(direction, teteSerpentX, teteSerpentY);

	/*si on se trouve a la fin du tableau serpent, on mettre l'indice tete dedans l'indice 0 du tableau, otherwise, on le mettre dans la case suivant*/
	if (indiceTete == TAILLE_TABLEAU_SERPENT - 1){
		indiceTete = 0;
	}
	else{
		indiceTete++;
	}

	serpentX[indiceTete] = teteSerpentX;
	serpentY[indiceTete] = teteSerpentY;

	/* on appel � la fonction tester Collision pour "ovrir la fenetre du tableau" et augmenter la taille de la serpent*/
	bool mangeSouris = testerCollision(teteSerpentX, teteSerpentY, sourisX, sourisY, nbSouris);
	if (!mangeSouris){
		if (indiceQueue == TAILLE_TABLEAU_SERPENT - 1){
			indiceQueue = 0;
		}
		else {
			indiceQueue++;
		}
	}
}

bool testerCollision(int posX, int posY,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	T�che: tester si (posX, posY) est la position d'une souris. Si oui, retirer
	la souris de la population de souris
	Param�tres: la position de la t�te du serpent, les tableaux de coordonn�es et
	le nombre de souris
	Retour: true si collision, false sinon
	*/
{
	/*on fait l'iteration du tableau du suris et on compare si les coordonnes de la tete de la serpent sont egales pour retourne un valeur "vrai" si 
	la serpent a mang� un souris*/
	for (int i = 0; i < nbSouris; i++) {
		if (posX == sourisX[i] && posY == sourisY[i]) {
			sourisX[i] = sourisX[nbSouris - 1];
			sourisY[i] = sourisY[nbSouris - 1];
			nbSouris--;
			return true;
		}
	}
	return false;
}
/*on a cr�e un fonction tres similar au ce du souris pour tester si la serpent mange son propre corps*/
bool testerCollisionSerpent(int serpentX[], int serpentY[], int indiceTete, int indiceQueue)
{
	int teteSerpentX = serpentX[indiceTete];
	int teteSerpentY = serpentY[indiceTete];
	int indice = indiceTete; // on prend en memoire l'indice dans lequel la tete de la serpent est

	do
	{
		/*on fait un tour dedans le tableau de la serpent pour verifier si l'indice de la tete est le meme que ce de la queue*/
		if (indice == indiceQueue){
			return false; //on returne false pour le valeur de la collision
		}
		else if (indice == 0){// si la tete de la serpen est dedans le premier indice du tableau on doit evaluer le dernier indice du tableau pour la queue
			indice = TAILLE_TABLEAU_SERPENT - 1; 
		}
		else{
			indice--; //sinon on metre dedans l'evaluation l'indice dans le valeur anterieur
		}
		/*Finalement on verifie si la tete de la serpent est dans un indice passe et pourtant elle a mang� son propre corps on return vrai */
		if (serpentX[indice] == teteSerpentX && serpentY[indice] == teteSerpentY){
			return true; //on return vrai pour la collision de la tete avec le corps
		}

	} while (true);

	return false;
}

void dessinerSerpent(int serpentX[], int serpentY[], int indiceTete, int indiceQueue) {
	gotoXY(serpentX[indiceTete], serpentY[indiceTete]);
	cout << "O";
/*Quand la serpent bouge on doit effacer (avec l'impression d'un space) le derniere index, mais si on est dans l'index 0 on doit aller �
la fin du tableau*/
	if (indiceQueue == 0){
		gotoXY(serpentX[TAILLE_TABLEAU_SERPENT - 1], serpentY[TAILLE_TABLEAU_SERPENT - 1]);
	}
	else{
		gotoXY(serpentX[indiceQueue - 1], serpentY[indiceQueue - 1]);
	}
	cout << " ";
}






// **********************
// * PARTIE III : BONUS *
// **********************

void deplacerSouris(int sourisX[], int sourisY[], int nbSouris)
/*
T�che: d�placer les souris (al�atoirement ou intelligemment)
Param�tres: les tableaux de coordonn�es et le nombre de souris
*/
{

}

void placerMurs(int nbLignes, int nbColonnes, int nbMurs)
/*
T�che: placer des murs al�atoirement sur le terrain de jeu
Param�tres: les dimensions du terrain et le nombre de murs � placer
*/
{

}
