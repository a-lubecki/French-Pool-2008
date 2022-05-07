#ifndef _BILLARDS
#define _BILLARDS

#define NB_SPRITES_SS_MENU 12
#define NB_SPRITES_JEU_FR 3
#define NB_BOULES_FR 3 // 22//
#define MODES 3
#define TAPIS 5
#define BOULES 5
#define SCORE 3

typedef struct
{
    int mode;
    int theme;
    int styleB;
    int nbPts;
} options, *pOptions;

int menuJeu();
int billard_francais();

#endif
