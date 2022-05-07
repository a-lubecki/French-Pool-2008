#include <stdio.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int main()
{

    float val = 0;
    do
    {
        printf("valeur de 0 a 255 : ");
        cin >> val;
        val /= 255;
        printf("valeur de 0 a 1 : %f\n", val);

    } while (val != -1);

    system("PAUSE");
    return (EXIT_SUCCESS);
}
