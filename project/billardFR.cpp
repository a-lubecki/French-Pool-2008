#include <allegro.h>
#include <jpgalleg.h>
#include <alpng.h>
#include <math.h>
#include "billardFR.h"
#include "mecaniqueDeJeu.h"
#include "quit.h"
#define PI 3.1415926535

int menuJeu()
{ //+paramètre de la structure options

    carac c;
    if (restaurerBibliotheque(&c, "test.dat") == -1)
    {
        c.mode = 0;
        c.thBill = 0;
        c.styBoul = 0;
        c.score = 0;
    }

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    imbmp tab[NB_SPRITES_SS_MENU];

    tab[0].im = load_jpg("images/Menu principal/Fond menu.jpg", NULL);
    tab[1].im = load_png("images/Sous menu/Billard Francais led.png", NULL);
    tab[2].im = load_png("images/Choix quit/Cyan quit.png", NULL);
    tab[3].im = load_png("images/Choix quit/Vert quit.png", NULL);
    tab[4].im = load_png("images/Sous menu/Mode led.png", NULL);
    tab[5].im = load_png("images/Sous menu/Tapis led.png", NULL);
    tab[6].im = load_png("images/Sous menu/Boules led.png", NULL);
    tab[7].im = load_png("images/Sous menu/Score led.png", NULL);
    tab[8].im = load_png("images/Sous menu/Cyan deroul.png", NULL);
    tab[9].im = load_png("images/Sous menu/Vert deroul.png", NULL);
    tab[10].im = load_png("images/Sous menu/Jeu led.png", NULL);
    tab[11].im = load_png("images/Sous menu/Retour led.png", NULL);

    for (int i = 0; i <= NB_SPRITES_SS_MENU - 1; i++)
    {
        if (!(tab[i].im))
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Erreur ! Impossible de lire les images du menu du B. francais");
            return -1;
        }
    }

    tab[0].p.x = 0;
    tab[0].p.y = 0;
    tab[1].p.x = SCREEN_W / 2 - tab[1].im->w / 2;
    tab[1].p.y = SCREEN_H / 20;

    tab[4].p.y = tab[5].p.y = tab[6].p.y = tab[7].p.y = 150;

    tab[4].p.x = SCREEN_W / 5 - 45 - tab[2].im->w / 2;
    tab[5].p.x = SCREEN_W * 2 / 5 - 15 - tab[2].im->w / 2;
    tab[6].p.x = SCREEN_W * 3 / 5 + 15 - tab[2].im->w / 2;
    tab[7].p.x = SCREEN_W * 4 / 5 + 45 - tab[2].im->w / 2;

    tab[10].p.x = tab[11].p.x = tab[7].p.x;
    tab[10].p.y = SCREEN_H / 2 + 70;
    tab[11].p.y = SCREEN_H * 9 / 10;

    imbmp texte, ex1, ex2;
    texte.p.x = tab[4].p.x;
    ex1.p.x = SCREEN_W / 10;
    ex2.p.x = SCREEN_W / 2 - 25;
    ex1.p.y = ex2.p.y = texte.p.y = tab[11].p.y;

    tab[11].p.y -= tab[2].im->h;

    int val;
    int choix;
    int choix2;
    int nbBoutons;

    BITMAP *choixRetenu[4];
    choixRetenu[0] = load_png("images/Sous menu/1 Joueur led.png", NULL);
    choixRetenu[1] = load_png("images/Sous menu/Normal led.png", NULL);
    choixRetenu[2] = load_png("images/Sous menu/Normal led.png", NULL);
    choixRetenu[3] = load_png("images/Sous menu/20 led.png", NULL);

    while (!key[KEY_ENTER])
    {
        clear_bitmap(buffer);
        for (int i = 0; i <= 1; i++)
            draw_sprite(buffer, tab[i].im, (int)tab[i].p.x, (int)tab[i].p.y);

        // trois cadres verts transparents ATTENTION : longueurs et pos sont inversées
        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        set_multiply_blender(0, 0, 0, 170); // alpha

        rectfill(buffer, (int)texte.p.x, (int)texte.p.y, (int)texte.p.x + tab[2].im->w, (int)tab[10].p.y, makecol(0, 50, 0));

        // 2 boutons
        if (sourisSurBouton(buffer, tab[10].p, tab[2].im, tab[3].im, 1, &choix) == 1)
            textprintf(buffer, font, (int)texte.p.x + 10, (int)tab[10].p.y + 20, BLANC, "Jouer.");
        if (sourisSurBouton(buffer, tab[11].p, tab[2].im, tab[3].im, 2, &choix) == 1)
            textprintf(buffer, font, (int)texte.p.x + 10, (int)tab[10].p.y + 20, BLANC, "Retourner au menu.");

        // ecritures
        draw_sprite(buffer, tab[10].im, (int)(tab[10].p.x + tab[2].im->w / 2 - tab[10].im->w / 2), (int)(tab[10].p.y + +tab[2].im->h / 2 - tab[10].im->h / 2));
        draw_sprite(buffer, tab[11].im, (int)(tab[11].p.x + tab[2].im->w / 2 - tab[11].im->w / 2), (int)(tab[11].p.y + +tab[2].im->h / 2 - tab[11].im->h / 2));

        // 4boutons pour liste déroulante
        for (int i = 0; i <= 3; i++)
        {
            draw_sprite(buffer, tab[2].im, (int)tab[i + 4].p.x, (int)tab[i + 4].p.y);
        }
        // ecritures
        for (int i = 4; i <= 7; i++)
            draw_sprite(buffer, tab[i].im, (int)tab[i].p.x + tab[2].im->w / 2 - tab[i].im->w / 2, (int)tab[i].p.y + 5);

        if (choix == 1)
            break;
        if (choix == 2)
            return -1;

        choix = 0;
        choix2 = 0;
        val = 0;

        char ecriture[4][25];
        strcpy(ecriture[0], "Mode de jeu :\0");
        strcpy(ecriture[1], "Style de la table :\0");
        strcpy(ecriture[2], "Couleurs des boules :\0");
        strcpy(ecriture[3], "Score a atteindre :\0");

        char descript[MODES][2][20];
        strcpy(descript[0][0], "Jouez seul contre\0");
        strcpy(descript[0][1], "l'ordinateur.\0");
        strcpy(descript[1][0], "Jouez a deux avec\0");
        strcpy(descript[1][1], "un ami.\0");
        strcpy(descript[2][0], "Entrainez-vous seul.\0");

        for (int i = 0; i <= 3; i++)
        {
            if (sourisToucheBouton(tab[i + 4].p, tab[2].im) == 1)
            {
                textprintf(buffer, font, (int)texte.p.x + 10, (int)tab[10].p.y + 20, BLANC, ecriture[i]);

                val = i + 1;
                int v;

                switch (i)
                {
                case 0:
                    nbBoutons = MODES;
                    break;
                case 1:
                    nbBoutons = TAPIS;
                    break;
                case 2:
                    nbBoutons = BOULES;
                    break;
                case 3:
                    nbBoutons = SCORE;
                    break;
                }

                BITMAP *choix[nbBoutons];

                switch (i)
                {
                case 0:
                    choix[0] = load_png("images/Sous menu/1 Joueur led.png", NULL);
                    choix[1] = load_png("images/Sous menu/2 Joueurs led.png", NULL);
                    choix[2] = load_png("images/Sous menu/Libre led.png", NULL);

                    break;
                case 1:
                    choix[0] = load_png("images/Sous menu/Normal led.png", NULL);
                    choix[1] = load_png("images/Sous menu/Bleu led.png", NULL);
                    choix[2] = load_png("images/Sous menu/Rouge led.png", NULL);
                    choix[3] = load_png("images/Sous menu/Metal led.png", NULL);
                    choix[4] = load_png("images/Sous menu/Universe led.png", NULL);
                    break;
                case 2:
                    choix[0] = load_png("images/Sous menu/Normal led.png", NULL);
                    choix[1] = load_png("images/Sous menu/Vif led.png", NULL);
                    choix[2] = load_png("images/Sous menu/France led.png", NULL);
                    choix[3] = load_png("images/Sous menu/Energy led.png", NULL);
                    choix[4] = load_png("images/Sous menu/Universe led.png", NULL);
                    break;
                case 3:
                    choix[0] = load_png("images/Sous menu/20 led.png", NULL);
                    choix[1] = load_png("images/Sous menu/50 led.png", NULL);
                    choix[2] = load_png("images/Sous menu/99 led.png", NULL);
                    break;
                }
                for (int n = 0; n < nbBoutons; n++)
                    if (!(choix[n]))
                    {
                        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
                        allegro_message("Erreur ! Impossible de lire les images du menu du B. francais");
                        return -1;
                    }

                BITMAP *buf = create_bitmap(SCREEN_W, SCREEN_H);
                do
                {
                    choix2 = -1;
                    clear_bitmap(buf);
                    blit(buffer, buf, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                    point tmp = tab[i + 4].p;

                    tmp.y += tab[2].im->h + 1;

                    for (int j = 0; j < nbBoutons; j++)
                    {
                        // allegro_message("val = %d",j);
                        if (sourisSurBouton(buffer, tmp, tab[8].im, tab[9].im, j, &choix2) == 1)
                        {
                            if (i == 0)
                            {
                                textprintf(buf, font, (int)texte.p.x + 10, (int)tab[10].p.y + 40, BLANC, descript[j][0]);
                                textprintf(buf, font, (int)texte.p.x + 10, (int)tab[10].p.y + 55, BLANC, descript[j][1]);
                            }
                        }
                        tmp.y += tab[8].im->h + 1;
                    }
                    if (choix2 != -1)
                    {
                        switch (i)
                        {
                        case 0:
                            c.mode = choix2;
                            break;
                        case 1:
                            c.thBill = choix2;
                            break;
                        case 2:
                            c.styBoul = choix2;
                            break;
                        case 3:
                            c.score = choix2;
                            break;
                        }

                        sauvegarderBibliotheque(&c, "test.dat");
                        // allegro_message("%d %d %d %d",c.mode,c.thBill,c.styBoul,c.score);
                        break;
                    }

                    // ecritures
                    for (int n = 0; n < nbBoutons; n++)
                        draw_sprite(buffer, choix[n], (int)tab[4 + i].p.x + tab[2].im->w / 2 - choix[n]->w / 2, (int)tab[4].p.y + tab[2].im->h + tab[8].im->h * n + tab[8].im->h / 2 - choix[n]->h / 2 + 1);

                    if (key[KEY_ESC])
                    {
                        fadeFinMenu(buffer);
                        finAppui();

                        for (int i = 0; i < NB_SPRITES_SS_MENU; i++)
                            destroy_bitmap(tab[i].im);

                        destroy_bitmap(buffer);
                        destroy_bitmap(buf);
                        return -1;
                    }
                    blit(buf, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

                    if (mouse_x >= tab[i + 4].p.x && mouse_y >= tab[i + 4].p.y &&
                        mouse_x <= (tab[i + 4].p.x + tab[2].im->w) && mouse_y <= (tab[i + 4].p.y + tab[2].im->h + tab[8].im->h * nbBoutons + nbBoutons))
                        v = 1;
                    else
                        v = 0;
                    // allegro_message("val");
                } while (v == 1);

                destroy_bitmap(buf);
                for (int n = 0; n < nbBoutons; n++)
                    destroy_bitmap(choix[n]);
                // allegro_message("choix = %d",choix);
            }
        }

        if (key[KEY_ESC])
        {
            fadeFinMenu(buffer);
            finAppui();

            for (int i = 0; i < NB_SPRITES_SS_MENU; i++)
                destroy_bitmap(tab[i].im);

            destroy_bitmap(buffer);
            return -1;
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    fadeFinMenu(buffer);
    finAppui();
    for (int i = 0; i < NB_SPRITES_SS_MENU; i++)
        destroy_bitmap(tab[i].im);
    for (int i = 0; i < 4; i++)
        destroy_bitmap(choixRetenu[i]);

    destroy_bitmap(buffer);
    return 0;
}

int billard_francais()
{ // au début : entrainement

    //////////////////////////
    if (menuJeu() != 0)
        return 0;
    //////////////////////////

    carac c;
    if (restaurerBibliotheque(&c, "test.dat") == -1)
    {
        c.mode = 0;
        c.thBill = 0;
        c.styBoul = 0;
        c.score = 0;
    }

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    char nom_im1[40] = "images/Jeu FR/TableFR ";
    char nom_im2[40] = "images/Jeu FR/TapisFR ";
    char *p1 = strstr(nom_im1, "TableFR ");
    char *p2 = strstr(nom_im2, "TapisFR ");
    p1 += strlen("TableFR ");
    p2 += strlen("TapisFR ");

    switch (c.thBill)
    {
    case 0:
        strcat(p1, "NO");
        strcat(p2, "NO");
        break;
    case 1:
        strcat(p1, "BL");
        strcat(p2, "BL");
        break;
    case 2:
        strcat(p1, "RO");
        strcat(p2, "RO");
        break;
    case 3:
        strcat(p1, "NO");
        strcat(p2, "NO");
        break;
    case 4:
        strcat(p1, "NO");
        strcat(p2, "NO");
        break;
    }

    strcat(p1, ".png\0");
    strcat(p2, ".png\0");

    BITMAP *tab[NB_SPRITES_JEU_FR];
    imbmp table; // coin supérieur gauche du sprite
    tab[0] = table.im = load_png(nom_im1, NULL);

    imbmp tapis;
    tab[1] = tapis.im = load_png(nom_im2, NULL);

    imbmp bEffet;
    tab[2] = bEffet.im = load_png("images/Jeu global/Boule choix effet.png", NULL);

    for (int i = 0; i < NB_SPRITES_JEU_FR; i++)
    {

        if (!tab[i])
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Erreur ! Impossible de lire les images du B. francais");
            return -1;
        }
    }

    // allegro_message("%s",nom_im2);
    table.p.x = SCREEN_W / 2 - table.im->w / 2;
    table.p.y = SCREEN_H - table.im->h;

    tapis.p.x = SCREEN_W / 2 - tapis.im->w / 2;
    tapis.p.y = SCREEN_H - table.im->h / 2 - tapis.im->h / 2;

    bEffet.p.x = bEffet.p.y = table.p.y / 2;

    char nom_bB[40] = "images/Jeu FR/Boule B ";
    char nom_bR[40] = "images/Jeu FR/Boule R ";
    char nom_bJ[40] = "images/Jeu FR/Boule J ";
    p1 = strstr(nom_bB, "B ");
    p2 = strstr(nom_bR, "R ");
    char *p3 = strstr(nom_bJ, "J ");
    p1 += strlen("B ");
    p2 += strlen("R ");
    p3 += strlen("J ");

    switch (c.styBoul)
    {
    case 0:
        strcat(p1, "NO");
        strcat(p2, "NO");
        strcat(p3, "NO");
        break;
    case 1:
        strcat(p1, "VI");
        strcat(p2, "VI");
        strcat(p3, "VI");
        break;
    case 2:
        strcat(p1, "NO");
        strcat(p2, "FR");
        strcat(p3, "FR");
        break;
    case 3:
        strcat(p1, "NO");
        strcat(p2, "NO");
        strcat(p3, "NO");
        break;
    case 4:
        strcat(p1, "NO");
        strcat(p2, "NO");
        strcat(p3, "NO");
        break;
    }

    strcat(p1, ".png\0");
    strcat(p2, ".png\0");
    strcat(p3, ".png\0");

    boule B[NB_BOULES_FR];

    B[0].im = load_png(nom_bB, NULL);
    B[1].im = load_png(nom_bR, NULL);
    B[2].im = load_png(nom_bJ, NULL);

    for (int i = 0; i < NB_BOULES_FR; i++)
        B[i].v = 0;

    for (int i = 0; i < NB_BOULES_FR; i++)
    {

        if (!B[i].im)
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Erreur ! Impossible de lire les images du B. francais");
            return -1;
        }
    }

    int nb_bandes = 4;
    bande bande[nb_bandes];

    for (int i = 0; i < 4; i++)
    {
        bande[i].p1.x = tapis.p.x;
        bande[i].p2.x = tapis.p.x;
        bande[i].p1.y = tapis.p.y;
        bande[i].p2.y = tapis.p.y;
    }

    bande[0].ang = bande[2].ang = 0;
    bande[1].ang = bande[3].ang = 90;

    //+100 et -100 pour etre sur que les  boules ne s'en iront pas, en tenir compte dans coupExecuté(...)
    bande[0].p1.x += 0 - 100;
    bande[0].p1.y += 0 - 100;
    bande[0].p2.x += tapis.im->w + 100;
    bande[0].p2.y += 0 - 100;
    bande[1].p1.x += tapis.im->w + 100;
    bande[1].p1.y += 0 - 100;
    bande[1].p2.x += tapis.im->w + 100;
    bande[1].p2.y += tapis.im->h + 100;
    bande[2].p1.x += tapis.im->w + 100;
    bande[2].p1.y += tapis.im->h + 100;
    bande[2].p2.x += 0 - 100;
    bande[2].p2.y += tapis.im->h + 100;
    bande[3].p1.x += 0 - 100;
    bande[3].p1.y += tapis.im->h + 100;
    bande[3].p2.x += 0 - 100;
    bande[3].p2.y += 0 - 100;

    /*   __________
        |     0    |        ___
        |3        1|    /\     |
        |_____2____|    |______|      */

    // les centres des boules
    B[0].p.x = tapis.p.x + tapis.im->w / 4; // blanche
    B[0].p.y = tapis.p.y + tapis.im->h * 2 / 3;
    B[1].p.x = tapis.p.x + tapis.im->w * 3 / 4; // rouge
    B[1].p.y = tapis.p.y + tapis.im->h / 2;
    B[2].p.x = tapis.p.x + tapis.im->w / 4; // jaune
    B[2].p.y = tapis.p.y + tapis.im->h / 2;
    // allegro_message("%f",351.6+B[2].im->w/2);
    /////////////////

    int choix_quitter = 0;

    for (int i = 0; i < NB_BOULES_FR; i++)
    {
        B[i].ang = 0;
        B[i].v = 0;
    }

    int scoreFinal;
    switch (c.score)
    {
    case 0:
        scoreFinal = 20;
        break;
    case 1:
        scoreFinal = 50;
        break;
    case 2:
        scoreFinal = 99;
        break;
    }

    int nbjoueurs;
    int ia;
    switch (c.mode)
    {
    case 0:
        nbjoueurs = 2;
        ia = 1;
        break;
    case 1:
        nbjoueurs = 2;
        ia = 0;
        break;
    case 2:
        nbjoueurs = 1;
        ia = 0;
        break;
    }

    // initialisation de tous les éléments du buffer
    coupExecute(tapis, table, NB_BOULES_FR, B, bande, nb_bandes, &buffer, bEffet);
    BITMAP *buf = create_bitmap(500, 100);

    int nbpts[nbjoueurs];
    for (int i = 0; i < nbjoueurs; i++)
        nbpts[i] = 0;
    int nbtouch;
    point effet;
    int max = 0;
    int cptCaramb = 0;
    int i = 0;
    int cptJoueur = 0; // le joueur en train de jouer
    boule tmp;         // chgmt de boule

    while (nbtouch != -1 && nbpts[cptJoueur] != scoreFinal)
    {

        clear_bitmap(buf);

        switch (c.mode)
        {
        case 0:
            textprintf(buf, font, 0, 0, BLANC, "Mode 1 joueur.");
            break;
        case 1:
            textprintf(buf, font, 0, 0, BLANC, "Mode 2 joueurs.");
            break;
        case 2:
            textprintf(buf, font, 0, 0, BLANC, "Mode libre.");
            break;
        }
        textprintf(buf, font, 0, 20, BLANC, "Joueur %d :", cptJoueur + 1);
        textprintf(buf, font, 0, 30, BLANC, "Score %d / %d points.", nbpts[cptJoueur], scoreFinal);
        textprintf(buf, font, 0, 40, BLANC, "Nombre de coups total : %d.", i);

        blit(buf, buffer, 0, 0, 250, 20, buf->w, buf->h);

        do
        {

            B[0].ang = rotationQueue(buffer, B[0], bEffet, table);
            if (B[0].ang == -1)
            {
                for (int i = 0; i < NB_SPRITES_JEU_FR; i++)
                    destroy_bitmap(tab[i]);
                for (int i = 0; i < NB_BOULES_FR; i++)
                    destroy_bitmap(B[i].im);
                destroy_bitmap(buffer);
                return 0;
            }

            B[0].v = translationQueue(buffer, B[0], bEffet, table);

            if (B[0].v == -1)
            {

                for (int i = 0; i < NB_SPRITES_JEU_FR; i++)
                    destroy_bitmap(tab[i]);
                for (int i = 0; i < NB_BOULES_FR; i++)
                    destroy_bitmap(B[i].im);
                destroy_bitmap(buffer);
                return 0;
            }

        } while (B[0].v == 0);

        nbtouch = coupExecute(tapis, table, NB_BOULES_FR, B, bande, nb_bandes, &buffer, bEffet);

        if (nbtouch == NB_BOULES_FR - 1)
        {
            nbpts[cptJoueur] += 1;
            cptCaramb += 1;
        }
        else
        {
            cptCaramb = 0;
            cptJoueur += 1;

            tmp = B[0]; // chgmt de boule à taper
            B[0] = B[2];
            B[2] = tmp;
        }

        if (cptCaramb > max)
            max = cptCaramb;

        i++;

        if (cptJoueur == nbjoueurs)
            cptJoueur = 0;

    } // boucle tant que le nb de points pour 1 joueur n'atteint pas celui indiqué dans le sous menu

    // message pour celui qui a gagné
    // fin de jeu temporaire
    switch (c.mode)
    {

    case 0:
        allegro_message("Mode 1 joueur.");
        if (cptJoueur == 0)
            allegro_message("Vous avez gagne !");
        else
            allegro_message("L'ordinateur a gagne !");

        break;
    case 1:
        allegro_message("Mode 2 joueurs.");
        allegro_message("Le joueur %d a gagne !", cptJoueur + 1);
        break;
    case 2:
        if (i != 0)
        {
            allegro_message("Mode libre.");
            int precision = 100 * ((int)scoreFinal / i);
            allegro_message("Vous avez %d pour 100 de precision.\nPlus grand nombre de carambolages : %d.", precision, max);
        }
        break;
    }

    for (int i = 0; i < NB_SPRITES_JEU_FR; i++)
        destroy_bitmap(tab[i]);
    for (int i = 0; i < NB_BOULES_FR; i++)
        destroy_bitmap(B[i].im);
    destroy_bitmap(buffer);
    destroy_bitmap(buf);

    finAppui();

    return 0;
}
