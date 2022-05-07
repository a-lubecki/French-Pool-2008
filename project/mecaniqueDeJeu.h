#ifndef _JEU
#define _JEU

#include "quit.h"
#define PI 3.1415926535

typedef struct
{
    point p;
    float v;
    float ang;
    BITMAP *im;
} boule, *pBoule;

typedef struct
{
    point p1;
    point p2;
    float ang;
} bande, *pBande;

typedef struct
{
    float a, b, c;
} droite, *pDroite;

void gestionPremiereCollision2(pBoule b1, pBoule b2, float D);
void gestionPremiereCollision(pBoule b1, pBoule b2);
void collisionBoules(pBoule b1, pBoule b2);
void recentrAngle(float *angle);
void placementSelonAngle(pBoule boule);
void diminutionVitesseProgressive(float *V, float Vs);
float calculDAngle(point p1, point p2);
point choixEffet(BITMAP **buf, imbmp table, imbmp bEffet, point mouse);

float distancePointDroite(point p, droite d);
float distanceBouleBande(boule boule, bande bande);

float rotationQueue(BITMAP *buffer, boule blanche, imbmp bEffet, imbmp table);
float translationQueue(BITMAP *buffer, boule blanche, imbmp bEffet, imbmp table);

int coupExecute(imbmp tapis, imbmp table, int nb_boules, boule *B, bande *bande, int nb_bandes, BITMAP **buffer, imbmp bEffet);

#endif
