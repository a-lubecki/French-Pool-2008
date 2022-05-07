#include <allegro.h>
#include <jpgalleg.h>
#include <alpng.h>
#include <math.h>
#include "mecaniqueDeJeu.h"
#include "quit.h"

void collisionBoules(pBoule b1, pBoule b2)
{
    float diff1 = calculDAngle(b2->p, b1->p);

    float diff2 = diff1 + 180;

    recentrAngle(&diff1);
    recentrAngle(&diff2);

    float a1 = b1->ang - diff1;
    float a2 = b2->ang - diff1;

    recentrAngle(&a1);
    recentrAngle(&a2);

    a1 *= (PI / 180);
    a2 *= (PI / 180);

    float Vx1 = cos(a1) * b1->v;
    float Vy1 = sin(a1) * b1->v;
    float Vx2 = cos(a2) * b2->v;
    float Vy2 = sin(a2) * b2->v;

    float tmp = Vx1;
    Vx1 = Vx2;
    Vx2 = tmp;

    b1->ang = atan2(Vy1, Vx1);

    if (Vx1 == 0)
        b1->v = Vy1 / sin(b1->ang);
    else
        b1->v = Vx1 / cos(b1->ang);

    b2->ang = atan2(Vy2, Vx2);

    if (Vx2 == 0)
        b2->v = Vy2 / sin(b2->ang);
    else
        b2->v = Vx2 / cos(b2->ang);

    b1->ang *= (180 / PI);
    b2->ang *= (180 / PI);

    b1->ang += diff1;
    b2->ang += diff2 + 180;

    recentrAngle(&b1->ang);
    recentrAngle(&b2->ang);
}

void recentrAngle(float *angle)
{
    while (*angle < 0)
        *angle += 360;
    while (*angle >= 360)
        *angle -= 360;
}

float calculDAngle(point p1, point p2)
{ // cercle trigo
    float angle;
    float lx = p1.x - p2.x;
    float ly = p1.y - p2.y;

    angle = atan2(ly, lx);
    angle *= (360) / (2 * PI);

    return angle;
}

void diminutionVitesseProgressive(float *V, float Vs)
{

    if (*V > 1.3 * Vs)
        *V -= Vs * 0.3;

    if (*V > Vs)
        *V -= Vs * 0.04;
    else
    {
        if (*V > Vs / 2)
            *V -= Vs * 0.007;
        else
        {
            if (*V > Vs / 4)
                *V -= Vs * 0.003;
            else
                *V -= Vs * 0.0005;
        }
    }
    if (*V <= 0.0004)
        *V = 0;
}

void placementSelonAngle(pBoule boule)
{

    float angle = -boule->ang * (PI / 180) - (PI / 2);

    boule->p.x += (boule->v * sin(angle));
    boule->p.y += (boule->v * cos(angle));
}

void gestionPremiereCollision(pBoule b1, pBoule b2, float D)
{

    float angle1 = -b1->ang * (PI / 180) + (PI / 2);
    float angle2 = -b2->ang * (PI / 180) + (PI / 2);
    int i = 1;

    while (longDiffEntre2Pts(b1->p, b2->p) < D || longDiffEntre2Pts(b1->p, b2->p) > D + 0.001)
    {
        while (longDiffEntre2Pts(b1->p, b2->p) < D)
        {

            b1->p.x += (b1->v / vtot * sin(angle1)) / i;
            b1->p.y += (b1->v / vtot * cos(angle1)) / i;

            // allegro_message("%f",angle);
            b2->p.x += (b2->v / vtot * sin(angle2)) / i;
            b2->p.y += (b2->v / vtot * cos(angle2)) / i;
        }

        if (i > 1000)
            break;

        while (longDiffEntre2Pts(b1->p, b2->p) > D + 0.001)
        {

            b1->p.x -= (b1->v / vtot * sin(angle1)) / i;
            b1->p.y -= (b1->v / vtot * cos(angle1)) / i;

            // allegro_message("%f",angle);
            b2->p.x -= (b2->v / vtot * sin(angle2)) / i;
            b2->p.y -= (b2->v / vtot * cos(angle2)) / i;
        }
        i++;
    }

    // allegro_message("%f",longDiffEntre2Pts(b1->p, b2->p));
}

void gestionPremiereCollision2(pBoule b1, pBoule b2, float D)
{
    float angle = -b1->ang * (PI / 180) + (PI / 2);
    int i = 0;
    while (longDiffEntre2Pts(b1->p, b2->p) < D)
    {
        b1->p.x += (b1->v * sin(angle));
        b1->p.y += (b1->v * cos(angle));
        i++;
    }
    // allegro_message("%d",i);
}

float distancePointDroite(point p, droite d)
{
    return fabs((d.a * p.x + d.b * p.y + d.c) / (sqrt(d.a * d.a + d.b * d.b)));
}

float distanceBouleBande(boule boule, bande bande)
{
    droite d;

    if (bande.p1.x == bande.p2.x)
    {
        d.b = 0;
        d.c = 1;
        d.a = -1 / bande.p1.x;
    }
    else if (bande.p1.y == bande.p2.y)
    {
        d.a = 0;
        d.c = -bande.p1.y;
        d.b = 1;
    }
    else
    {
        float A = (bande.p1.y - bande.p2.y) / (bande.p1.x - bande.p2.x);
        float B = bande.p1.y - A * bande.p2.y;
        d.a = -A;
        d.c = -B;
        d.b = 1;
    }
    return distancePointDroite(boule.p, d);
}

point choixEffet(BITMAP **buf, imbmp table, imbmp bEffet, point mouse)
{
    // boule C : C comme choix

    float diff = (longDiffEntre2Pts(mouse, bEffet.p));
    if (diff <= bEffet.im->w / 3)
    {
        BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
        clear_bitmap(buffer);

        blit(*buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        set_multiply_blender(0, 0, 0, 100); // alpha
        circlefill(buffer, mouse_x, mouse_y, 10, makecol(200, 200, 200));
        circle(buffer, mouse_x, mouse_y, 10, makecol(50, 150, 50));
        set_alpha_blender();

        blit(buffer, *buf, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        destroy_bitmap(buffer);
    }
}

float rotationQueue(BITMAP *buf, boule blanche, imbmp bEffet, imbmp table)
{
    finAppui();
    if (mouse_y < table.p.y)
        return 0;
    BITMAP *sprite_queue = load_png("images/Jeu global/queueBillard.png", NULL);

    if (!sprite_queue)
    {
        allegro_message("Erreur ! Impossible de lire l'image de la queue de billard");
        return -1;
    }
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    point mouse; // création de la position de la souris pour s'adapter à la fonct calculDAngleParRapportAuNord(boule b1, boule b2)

    for (int i = 0; i < blanche.im->w / 2 / 2; i++)
    {
        clear_bitmap(buffer);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        set_multiply_blender(0, 0, 0, 100); // alpha
        circlefill(buffer, mouse_x, mouse_y, i * 2, makecol(200, 200, 200));
        circle(buffer, mouse_x, mouse_y, i * 2, makecol(50, 150, 50));
        set_alpha_blender();

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    while (!(mouse_b & 1) && !key[KEY_ENTER])
    { // choix du coup, rotation

        mouse.x = mouse_x;
        mouse.y = mouse_y;

        clear_bitmap(buffer);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (mouse_y < table.p.y)
        {
            do
            {
                choixEffet(&buffer, table, bEffet, mouse);
            } while (mouse_y < table.p.y);
        }
        else
        {
            blanche.ang = calculDAngle(blanche.p, mouse);

            drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
            set_multiply_blender(0, 0, 0, 100); // alpha
            circlefill(buffer, mouse_x, mouse_y, blanche.im->w / 2, makecol(200, 200, 200));
            circle(buffer, mouse_x, mouse_y, blanche.im->w / 2, makecol(50, 150, 50));
            set_alpha_blender();

            pivot_sprite(buffer, sprite_queue, (int)blanche.p.x, (int)blanche.p.y, , sprite_queue->h / 2, ftofix(blanche.ang) * 182 / 256);

            recentrAngle(&blanche.ang);
        }

        if (key[KEY_ESC])
        {
            if (choixQuitter(buffer) == -1)
                return -1;
        }
        if (key[KEY_P])
            choixPause(buffer);

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    destroy_bitmap(sprite_queue);
    destroy_bitmap(buffer);
    finAppui();

    return blanche.ang;
}

float translationQueue(BITMAP *buf, boule blanche, imbmp bEffet, imbmp table)
{
    finAppui();
    if (mouse_y < table.p.y)
        return 0;

    float force = 0;

    BITMAP *sprite_queue = load_png("images/Jeu global/queueBillard.png", NULL);

    if (!sprite_queue)
    {
        allegro_message("Erreur ! Impossible de lire l'image de la queue de billard");
        return -1;
    }
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    int maxf;
    maxf = 250;
    point mouse;

    mouse.x = mouse_x;
    mouse.y = mouse_y;
    force = longDiffEntre2Pts(blanche.p, mouse);
    if (force > maxf)
        force = maxf;

    for (int i = 0; i < force / 40; i++)
    {
        clear_bitmap(buffer);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        set_multiply_blender(0, 0, 0, 100); // alpha
        circlefill(buffer, (int)blanche.p.x, (int)blanche.p.y, i * 40, makecol(200, 200, 200));
        circle(buffer, (int)blanche.p.x, (int)blanche.p.y, i * 40, makecol(50, 150, 50));
        set_alpha_blender();

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    while (!(mouse_b & 1) && !key[KEY_ENTER])
    { // choix du coup, translation

        mouse.x = mouse_x;
        mouse.y = mouse_y;

        clear_bitmap(buffer);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (mouse_y < table.p.y)
        {
            while (mouse_y < table.p.y)
                choixEffet(&buffer, table, bEffet, mouse);
        }
        else
        {

            force = longDiffEntre2Pts(blanche.p, mouse);
            if (force > maxf)
                force = maxf; // limitation de la force
            pivot_sprite(buffer, sprite_queue, (int)blanche.p.x, (int)blanche.p.y, -blanche.im->w / 2 - (int)force, sprite_queue->h / 2, ftofix(blanche.ang) * 182 / 256);

            drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
            set_multiply_blender(0, 0, 0, 100); // alpha
            circlefill(buffer, (int)blanche.p.x, (int)blanche.p.y, (int)force, makecol(200, 200, 200));
            circle(buffer, (int)blanche.p.x, (int)blanche.p.y, (int)force, makecol(50, 150, 50));
            set_alpha_blender();

            if (key[KEY_ESC])
            {
                if (choixQuitter(buffer) == -1)
                    return -1;
            }
            if (key[KEY_P])
                choixPause(buffer);

            if (mouse_b & 2)
            {
                for (float i = force / 40; i >= 0; i--)
                {
                    clear_bitmap(buffer);
                    blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

                    drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
                    set_multiply_blender(0, 0, 0, 100); // alpha
                    circlefill(buffer, (int)blanche.p.x, (int)blanche.p.y, (int)i * 40, makecol(200, 200, 200));
                    circle(buffer, (int)blanche.p.x, (int)blanche.p.y, (int)i * 40, makecol(50, 150, 50));
                    set_alpha_blender();

                    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                }
                return 0; // revenir au choix en rotation
            }

            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
    }

    finAppui();
    for (int i = 0; i <= 20; i++)
    { // temps de déplacement

        clear_bitmap(buffer);
        blit(buf, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        pivot_sprite(buffer, sprite_queue, (int)blanche.p.x, (int)blanche.p.y, -blanche.im->w / 2 - (int)force + ((int)force * i / 20), sprite_queue->h / 2, ftofix(blanche.ang) * 182 / 256);

        if (key[KEY_ESC])
        {
            if (choixQuitter(buffer) == -1)
                return -1;
        }
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    blanche.v = force / 20;

    destroy_bitmap(sprite_queue);
    destroy_bitmap(buffer);

    return blanche.v;
}

// vaut pour tous les billards
int coupExecute(imbmp tapis, imbmp table, int nb_boules, boule *B, bande *bande, int nb_bandes, BITMAP **buffer, imbmp bEffet)
{
    // rajouter les trous
    float Vtotale = 0;
    float D = B[0].im->w;
    float R = D / 2;
    float tmp;
    point ombre;
    int nbtouch = 0;

    int touch[nb_boules - 1];
    for (int i = 0; i < nb_boules - 1; i++)
        touch[i] = 0;

    do
    { /////////
        rest(7);
        ////////
        for (int i = 0; i < nb_boules; i++)
            placementSelonAngle(&B[i]);

        for (int i = 0; i < nb_boules; i++)
        {
            for (int j = 0; j < nb_bandes; j++)
            {
                if (distanceBouleBande(B[i], bande[j]) < R + 100)
                {
                    B[i].ang = 180 + 2 * (bande[j].ang - 90) - B[i].ang;

                    while (distanceBouleBande(B[i], bande[j]) < R + 100)
                    {
                        placementSelonAngle(&B[i]);
                        placementSelonAngle(&B[j]);
                    }
                }
            }
            if (i != nb_boules - 1)
            {
                for (int j = nb_boules - 1; j > i; j--)
                {
                    if (longDiffEntre2Pts(B[i].p, B[j].p) < D)
                    {
                        collisionBoules(&B[i], &B[j]);

                        while (longDiffEntre2Pts(B[i].p, B[j].p) < D)
                        {
                            placementSelonAngle(&B[i]);
                            placementSelonAngle(&B[j]);
                        }

                        if (i == 0)
                            touch[j - 1] = 1;
                        i = j;
                        break;
                    }
                }
            }
        }

        if (key[KEY_DOWN])
            for (int i = 0; i < nb_boules; i++)
                B[i].v = 0;
        for (int i = 0; i < nb_boules; i++)
            diminutionVitesseProgressive(&B[i].v, 15);
        Vtotale = 0;
        for (int i = 0; i < nb_boules; i++)
            Vtotale += B[i].v;

        draw_sprite(*buffer, tapis.im, (int)tapis.p.x, (int)tapis.p.y); // tapis

        point centre;
        centre.x = table.p.x + table.im->w / 2;
        centre.y = table.p.y + table.im->h / 2;

        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        set_multiply_blender(0, 0, 0, 130); // alpha

        for (int i = 0; i < nb_boules; i++)
        {
            ombre.x = B[i].p.x;
            ombre.y = B[i].p.y;

            float L = longDiffEntre2Pts(centre, B[i].p);

            float a = calculDAngle(B[i].p, centre);
            recentrAngle(&a);
            a *= (PI / 180);
            ombre.x += L * cos(a) / 80;
            ombre.y += L * sin(a) / 80;

            circlefill(*buffer, (int)ombre.x, (int)ombre.y, (int)R, makecol(150, 100, 150)); // ombres boules

            ombre.x = B[i].p.x;
            ombre.y = B[i].p.y;

            ombre.x += L * cos(a) / 60;
            ombre.y += L * sin(a) / 60;

            circlefill(*buffer, (int)ombre.x, (int)ombre.y, (int)R + 1, makecol(140, 100, 140)); // ombres boules
        }
        set_alpha_blender();

        for (int i = 0; i < nb_boules; i++)
            draw_sprite(*buffer, B[i].im, (int)B[i].p.x - (int)R, (int)B[i].p.y - (int)R); // boules

        draw_sprite(*buffer, table.im, (int)table.p.x, (int)table.p.y); // table affichée en bas au milieu

        textprintf(*buffer, font, 50, 600, BLANC, "vtot %f", Vtotale);

        draw_sprite(*buffer, bEffet.im, (int)bEffet.p.x - bEffet.im->h / 2, (int)bEffet.p.y - bEffet.im->h / 2);

        if (key[KEY_ESC])
        {
            if (choixQuitter(*buffer) == -1)
                return -1;
        }
        if (key[KEY_P])
        {
            while (key[KEY_P])
                ;
            choixPause(*buffer);
        }
        blit(*buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    } while (Vtotale > 0);

    for (int i = 0; i < nb_boules - 1; i++)
        if (touch[i] == 1)
            nbtouch += 1;

    return nbtouch;
}
