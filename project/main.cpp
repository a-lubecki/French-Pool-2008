#include <allegro.h>
#include <jpgalleg.h>
#include <alpng.h>
#include "menuPrincipal.h"
#include "billardFR.h"

int init();
void deinit();

int main()
{
    init();

    int choix_menu;
    int choix_quitter = 0;

    /////////////////////////////////
    choix_quitter = ecranDeDebut();
    /////////////////////////////////

    while (choix_quitter == 0)
    {

        //////////////////////////////
        choix_menu = menu_principal();
        //////////////////////////////

        switch (choix_menu)
        {
        case -1:
            choix_quitter = -1;
            break;
        case 0:
            break;
        case 1:
            choix_quitter = billard_francais();
            break;
        /*case 2:
            choix_quitter = billard_americain();
            break;
        case 3:
            choix_quitter = nine_ball();
            break;
        case 4:
            choix_quitter = snooker();
            break;
        case 5:
            choix_quitter = defis();
            break;

        case 10:
            options();
            continue;
        case 11:
            credits();
            continue;
            */
        }
    }

    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

    deinit();

    return 0;
}
END_OF_MAIN()

int init()
{
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth == 0)
        depth = 32;
    set_color_depth(depth);

    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0);

    if (res != 0)
    {
        allegro_message(allegro_error);
        exit(-1);
    }

    install_timer();
    install_keyboard();
    install_mouse();
    jpgalleg_init();
    alpng_init();

    if (install_mouse() == -1)
    {
        allegro_message("Erreur ! %s", allegro_error);
        return -1;
    }

    text_mode(-1);
    show_mouse(screen);
}

void deinit()
{
    clear_keybuf();

} /*  dimensions FR
  2,10 m ;
  2,20 m ;
  2,30 m ;
  2,40 m ;
  2,60 m ;
  2,80 m x 1,5275 m (taille "semi match") ;
  3,10 m x 1,6775 m (taille de compétition "match")

  diamètre de 61,5 mm

 dimensions ANGLAIS
  1,83m sur 0,91m

 dimensions AM
  2,54 m sur 1,27

  dimensions SNK
  3,56 m sur 1,78
  diamètre de 52,5mm

 */
