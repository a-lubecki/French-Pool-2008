#include <allegro.h>
#include <alpng.h>
#include <math.h>
#include "quit.h"
#include <iostream>
#include <stdio.h>

void fadeFinMenu(BITMAP *buffer)
{
    // fade
    BITMAP *buf = create_bitmap(SCREEN_W, SCREEN_H);
    for (int i = 0; i < 4; i++)
    {
        clear_bitmap(buf);
        masqueTransparent(buffer, 0, 0, 0, 130);
        blit(buffer, buf, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(buf, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    destroy_bitmap(buf);
}

float longDiffEntre2Pts(point p1, point p2)
{
    float lx = fabs(p2.x - p1.x);
    float ly = fabs(p2.y - p1.y);

    float diff = sqrt((lx * lx) + (ly * ly));

    return diff;
}

//à mettre avant un return qui prend en compte une touche et en début et fin de fonction obligatoirement (sauf cas spéciaux)
void finAppui()
{
    while (key[KEY_ESC])
        ;
    while (key[KEY_ENTER])
        ;
    while (key[KEY_BACKSPACE])
        ;
    while (mouse_b)
        ;
}

int sourisToucheBouton(point bouton, BITMAP *sprite_neutre)
{
    if (mouse_x >= bouton.x && mouse_y >= bouton.y && mouse_x <= (bouton.x + sprite_neutre->w) && mouse_y <= (bouton.y + sprite_neutre->h))
        return 1;
    return 0;
}

// détecte si la souris est positionnée sur le bouton (return 1)
int sourisSurBouton(BITMAP *buffer, point bouton, BITMAP *sprite_neutre, BITMAP *sprite_touche, int Vrenvoyee, int *choix)
{

    if (sourisToucheBouton(bouton, sprite_neutre) == 1)
    {

        if (mouse_b & 1)
        {
            finAppui();
            draw_sprite(buffer, sprite_touche, (int)bouton.x, (int)bouton.y);
            *choix = Vrenvoyee;
        }
        else
            draw_sprite(buffer, sprite_touche, (int)bouton.x, (int)bouton.y);

        return 1;
    }
    else
    {
        draw_sprite(buffer, sprite_neutre, (int)bouton.x, (int)bouton.y);
        return 0;
    }
}

void masqueTransparent(BITMAP *buf, int r, int g, int b, int a)
{ // transparence

    drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
    set_multiply_blender(0, 0, 0, a);                      // alpha
    rectfill(buf, 0, 0, buf->w, buf->h, makecol(r, g, b)); // couleur du calque transparent
    set_alpha_blender();
}

int choixPause(BITMAP *buf)
{

    imbmp pause;
    pause.im = load_png("images/Jeu global/Mot Pause led.png", NULL);
    if (!pause.im)
        return 0;
    pause.p.x = SCREEN_W / 2 - pause.im->w / 2;
    pause.p.y = SCREEN_H / 4;

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    for (int i = 0; i < 5; i++)
    {
        clear_bitmap(buffer);
        draw_sprite(buf, pause.im, (int)pause.p.x, (int)pause.p.y);
        masqueTransparent(buf, 50, 150, 50, 70);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    
    do
    {
        clear_bitmap(buffer);

        draw_sprite(buffer, pause.im, (int)pause.p.x, (int)pause.p.y);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        textprintf(buffer, font, 340, 550, makecol(200, 255, 200), "Touche Enter ou Echap pour revenir au jeu.");
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    } while (!(mouse_b || key[KEY_ENTER] || key[KEY_ESC] || key[KEY_P]));

    destroy_bitmap(pause.im);
    destroy_bitmap(buffer);

    while (key[KEY_P])
        ;
    finAppui();
    return 0;
}

int choixQuitter(BITMAP *buf)
{

    finAppui();
    int choix = 0;

    imbmp tab[5];

    tab[0].im = load_png("images/Choix quit/Vert quit.png", NULL);
    tab[1].im = load_png("images/Choix quit/Cyan quit.png", NULL);
    tab[2].im = load_png("images/Choix quit/Mot Oui led.png", NULL);
    tab[3].im = load_png("images/Choix quit/Mot Non led.png", NULL);
    tab[4].im = load_png("images/Choix quit/Phrase Quitter led.png", NULL);

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    for (int i = 0; i <= 4; i++)
    {
        if (!tab[i].im)
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Erreur ! Impossible de lire les images de fin de jeu");
            return -1;
        }
    }

    tab[0].p.x = SCREEN_W / 2 - 80 - tab[0].im->w;
    tab[0].p.y = SCREEN_H / 2;

    tab[1].p.x = SCREEN_W / 2 + 80;
    tab[1].p.y = SCREEN_H / 2;

    tab[2].p.x = tab[0].p.x + tab[0].im->w / 2 - tab[2].im->w / 2;
    tab[2].p.y = tab[0].p.y + tab[0].im->h / 2 - tab[2].im->h / 2;

    tab[3].p.x = tab[1].p.x + tab[1].im->w / 2 - tab[3].im->w / 2;
    tab[3].p.y = tab[1].p.y + tab[1].im->h / 2 - tab[3].im->h / 2;

    tab[4].p.x = SCREEN_W / 2 - tab[4].im->w / 2;
    tab[4].p.y = SCREEN_H / 3;

    // fade
    for (int i = 0; i < 5; i++)
    {
        clear_bitmap(buffer);
        masqueTransparent(buf, 0, 0, 0, 55);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    do
    {
        clear_bitmap(buffer);

        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (key[KEY_ESC] || (mouse_b & 2))
            break;
        if (key[KEY_ENTER])
        {
            choix = -1;
            break;
        }

        sourisSurBouton(buffer, tab[0].p, tab[1].im, tab[0].im, (-1), &choix);
        sourisSurBouton(buffer, tab[1].p, tab[1].im, tab[0].im, (-2), &choix);
        if (choix == -2)
        {
            choix = 0;
            break;
        }

        for (int i = 2; i <= 4; i++)
            draw_sprite(buffer, tab[i].im, (int)tab[i].p.x, (int)tab[i].p.y); // sprites oui, non et la phrase

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    } while (choix == 0);

    destroy_bitmap(buffer);
    for (int i = 0; i <= 4; i++)
        destroy_bitmap(tab[i].im);

    finAppui();

    return choix;
}

int sauvegarderBibliotheque(const pCarac pC, char *nomFichier)
{
    FILE *f = fopen(nomFichier, "wb");
    if (f == NULL)
    {
        return -1;
        allegro_message("Impossible de sauvegarder");
    }

    fwrite(pC, sizeof(carac), 1, f); // bloc
    // allegro_message("La bibliotheque a bien ete sauvegardee");

    fclose(f);
    return 0;
}

int restaurerBibliotheque(const pCarac pC, char *nomFichier)
{
    FILE *f = fopen(nomFichier, "rb");
    if (f == NULL)
        return -1;

    fread(pC, sizeof(carac), 1, f);

    fclose(f);
    return 0;
}

int convertCharBitmap(BITMAP *buf, char *nom, BITMAP **im_nom)
{

    //TODO
}
