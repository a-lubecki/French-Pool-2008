#include <allegro.h>
#include <jpgalleg.h>
#include <alpng.h>
#include "menuPrincipal.h"
#include "quit.h"

int ecranDeDebut()
{
    finAppui();

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    BITMAP *sprite_depart = load_png("images/Screen presentation/Screen depart.png", NULL); // png pour des raisons de propreté de l'image et à 100ko près du jpeg
    BITMAP *sprite_appuyer = load_png("images/Screen presentation/Appui sur une touche.png", NULL);

    if (!sprite_depart || !sprite_appuyer)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Erreur ! Impossible de lire les images de la presentation");
        return -1;
    }
    blit(sprite_depart, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    // fade in
    BITMAP *buf = create_bitmap(SCREEN_W, SCREEN_H);
    int max;
    for (int i = 0; i < 15; i++)
    {
        clear_bitmap(buf);
        max = 255 - i * 20;
        if (max > 255)
            max = 255;
        if (max < 0)
            max = 0;
        blit(buffer, buf, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        masqueTransparent(buf, 0, 0, 0, max);

        blit(buf, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    int val = 0;
    while (!(keypressed() || mouse_b))
    {
        clear_bitmap(buffer);
        blit(sprite_depart, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        // clignotement
        val++;
        if (val > 20)
            draw_sprite(buffer, sprite_appuyer, SCREEN_W / 2 - sprite_appuyer->w / 2, 380);
        if (val == 100)
            val = 0;

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    // fade out
    for (int i = 0; i < 15; i++)
    {
        clear_bitmap(buf);
        masqueTransparent(buffer, 0, 0, 0, 40);
        blit(buffer, buf, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(buf, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    destroy_bitmap(buf);

    destroy_bitmap(sprite_depart);
    destroy_bitmap(sprite_appuyer);
    destroy_bitmap(buffer);

    finAppui();
    return 0;
}

int menu_principal()
{

    finAppui();

    int choix = 0;
    ////////////////////////////
    // MENU : CHOIX MODES DE JEUX
    ////////////////////////////
    carac c;

    if (restaurerBibliotheque(&c, "test.dat") == -1)
    {
        c.mode = 0;
        c.thBill = 0;
        c.styBoul = 0;
        c.score = 0;
    }

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    imbmp tab[NB_SPRITES_MENU];

    tab[0].im = load_jpg("images/Menu principal/Fond menu.jpg", NULL);
    tab[1].im = load_png("images/Menu principal/Cyan menu.png", NULL);
    tab[2].im = load_png("images/Menu principal/Vert menu.png", NULL);
    tab[3].im = load_png("images/Menu principal/Mot Francais led.png", NULL);
    tab[4].im = load_png("images/Menu principal/Mot Americain led.png", NULL);
    tab[5].im = load_png("images/Menu principal/Mot 9ball led.png", NULL);
    tab[6].im = load_png("images/Menu principal/Mot Snooker led.png", NULL);
    tab[7].im = load_png("images/Menu principal/Mot Defis led.png", NULL);

    tab[8].im = load_png("images/Menu principal/Bouton quitter.png", NULL);
    imbmp buf_quit;
    buf_quit.im = create_bitmap(tab[8].im->w / 3, tab[8].im->h);

    tab[9].im = load_png("images/Menu principal/Mot Options led.png", NULL);
    tab[10].im = load_png("images/Menu principal/Mot Credits led.png", NULL);
    tab[11].im = load_png("images/Menu principal/Mot Quitter led.png", NULL);

    for (int i = 0; i < NB_SPRITES_MENU; i++)
    {
        if (!tab[i].im)
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Erreur ! Impossible de lire les images du menu");
            return -1;
        }
    }

    textprintf(tab[0].im, font, 50, 720, VERT, "2008 - BILLARD UNIVERSE made by Aurelien Lubecki - a.lubecki@laposte.fr");

    tab[8].p.x = 830;
    tab[8].p.y = 620;

    // le coin supérieur gauche du bouton
    tab[3].p.x = tab[6].p.x = tab[9].p.x = (SCREEN_W / 5 - tab[1].im->w / 2);
    tab[4].p.x = tab[7].p.x = tab[10].p.x = (SCREEN_W / 2 - tab[1].im->w / 2);
    tab[5].p.x = (SCREEN_W * 4 / 5 - tab[1].im->w / 2);
    tab[3].p.y = tab[4].p.y = tab[5].p.y = 50;
    tab[6].p.y = tab[7].p.y = 150;
    tab[9].p.y = tab[10].p.y = 600;

    imbmp ex, texte;

    ex.p.x = SCREEN_W / 15;
    texte.p.x = SCREEN_W * 3 / 5 + 25;
    ex.p.y = texte.p.y = 260;

    char nom_im[40] = "images/Menu principal/ex jeu ";
    char *p = strstr(nom_im, "ex jeu ");
    p += strlen("ex jeu ");
    int ls = strlen(nom_im);

    int val = 0;

    int prem = 0;
    do
    {

        clear_bitmap(buffer);

        draw_sprite(buffer, tab[0].im, 0, 0);

        // deux cadres verts transparents
        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        set_multiply_blender(0, 0, 0, 170);
        rectfill(buffer, (int)ex.p.x, (int)ex.p.y, SCREEN_W * 3 / 5 - 25, 570, makecol(0, 50, 0)); // couleur du calque transparent
        rectfill(buffer, SCREEN_W * 3 / 5 + 25, (int)texte.p.y, SCREEN_W * 14 / 15, 550, makecol(0, 50, 0));

        val = 0;
        for (int i = 0; i <= 4; i++)
            if (sourisSurBouton(buffer, tab[i + 3].p, tab[1].im, tab[2].im, i + 1, &choix) == 1)
                val = i + 1;

        switch (val)
        {
        case 1:
            strcat(p, "FR");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "La boule blanche doit toucher");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 35, BLANC, "les deux autres a chaque coup.");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 65, BLANC, "Essayez de faire le plus de series");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 80, BLANC, "possibles pour gagner.");
            break;
        case 2:
            strcat(p, "AM");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Indisponible pour le moment.");
            break;
        case 3:
            strcat(p, "9B");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Indisponible pour le moment.");
            break;
        case 4:
            strcat(p, "SN");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Indisponible pour le moment.");
            break;
        case 5:
            strcat(p, "DF");
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Indisponible pour le moment.");
            break;
        }
        if (nom_im[ls] != 0)
        {
            strcat(p, ".jpg\0");
            ex.im = load_jpg(nom_im, NULL); // jpg de 522x311
            if (!ex.im)
            {
                set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
                allegro_message("Erreur ! Impossible de lire les images du menu");
                return -1;
            }
            draw_sprite(buffer, ex.im, (int)ex.p.x, (int)ex.p.y);
            destroy_bitmap(ex.im);
            nom_im[ls] = 0;
        }

        if (sourisSurBouton(buffer, tab[9].p, tab[1].im, tab[2].im, 10, &choix) == 1)
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Acceder aux options.");
        if (sourisSurBouton(buffer, tab[10].p, tab[1].im, tab[2].im, 11, &choix) == 1)
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Voir les credits.");

        // affiche mots : +14 sur y ou +2 si accent
        for (int i = 3; i <= 10; i++)
        {
            if (i == 8)
                continue;
            draw_sprite(buffer, tab[i].im, (int)tab[i].p.x + tab[1].im->w / 2 - tab[i].im->w / 2, (int)tab[i].p.y + tab[1].im->h / 2 - tab[i].im->h / 2);
        }

        draw_sprite(buffer, tab[11].im, (int)tab[8].p.x - tab[11].im->w / 2, (int)tab[8].p.y + tab[11].im->h); // mot quitter

        point mouse;
        mouse.x = mouse_x;
        mouse.y = mouse_y;

        float lx_quit = tab[8].im->w / 3;

        // bouton rond QUITTER
        if (longDiffEntre2Pts(mouse, tab[8].p) <= lx_quit / 2)
        {
            textprintf(buffer, font, (int)texte.p.x + 20, (int)texte.p.y + 20, BLANC, "Quitter BILLARD UNIVERSE.");
            if (mouse_b & 1)
            {
                blit(tab[8].im, buf_quit.im, (int)lx_quit * 2, 0, 0, 0, buf_quit.im->w, buf_quit.im->h);
                draw_sprite(buffer, buf_quit.im, (int)tab[8].p.x - buf_quit.im->w / 2, (int)tab[8].p.y - buf_quit.im->h / 2);
                choix = choixQuitter(buffer);
            }
            else
                blit(tab[8].im, buf_quit.im, (int)lx_quit * 1, 0, 0, 0, buf_quit.im->w, buf_quit.im->h);
        }

        else
            blit(tab[8].im, buf_quit.im, (int)lx_quit * 0, 0, 0, 0, buf_quit.im->w, buf_quit.im->h);
        if (choix != -1)
            draw_sprite(buffer, buf_quit.im, (int)tab[8].p.x - buf_quit.im->w / 2, (int)tab[8].p.y - buf_quit.im->h / 2);

        if (key[KEY_ESC])
            choix = choixQuitter(buffer);

        if (prem == 0)
        {
            // fade in
            BITMAP *buf = create_bitmap(SCREEN_W, SCREEN_H);
            int max;
            for (int i = 0; i < 5; i++)
            {
                clear_bitmap(buf);
                max = 255 - i * 60;
                if (max > 255)
                    max = 255;
                if (max < 0)
                    max = 0;
                blit(buffer, buf, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                masqueTransparent(buf, 0, 0, 0, max);
                blit(buf, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            }
            destroy_bitmap(buf);
            prem = 1;
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    } while (choix == 0);

    fadeFinMenu(buffer);

    finAppui();

    destroy_bitmap(buf_quit.im);
    destroy_bitmap(buffer);
    for (int i = 0; i < NB_SPRITES_MENU; i++)
        destroy_bitmap(tab[i].im);

    return choix;
}
