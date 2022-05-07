#ifndef _QUIT
#define _QUIT

#define BLANC (makecol(255, 255, 255))
#define VERT (makecol(0, 50, 0))

typedef struct
{
    float x, y;
} point, *pPoint;

typedef struct
{
    point p;
    BITMAP *im;
} imbmp, *pImbmp;

////////////////////
typedef struct
{
    int mode;
    int thBill;
    int styBoul;
    int score;
} carac, *pCarac;

/*recense toutes les caractéristiques du jeu :
FR :
mode : 1(1j), 2(2j), 3(entrainement)
thème billard : 0(normal), 1(bleu), 2(rouge), 3(métal), 4(universe),
style boules : 0(normal), 1(vif), 2(france), 3(énergie), 4(universe)
nb points : 0 , 1 , 2
*/

void fadeFinMenu(BITMAP *buffer);
void finAppui();
int choixPause(BITMAP *buf);
float longDiffEntre2Pts(point p1, point p2);
int sourisToucheBouton(point bouton, BITMAP *sprite_neutre);
int sourisSurBouton(BITMAP *buffer, point bouton, BITMAP *sprite_neutre, BITMAP *sprite_touche, int Vrenvoyee, int *choix);
void masqueTransparent(BITMAP *buf, int r, int g, int b, int a);

int choixQuitter(BITMAP *buffer);
int restaurerBibliotheque(const pCarac pC, char *nomFichier);
int sauvegarderBibliotheque(const pCarac pC, char *nomFichier);
int convertCharBitmap(BITMAP *buf, char *nom, BITMAP **im_nom);

#endif
