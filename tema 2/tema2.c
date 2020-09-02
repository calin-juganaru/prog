#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 31    // numărul maxim de celule
                //  de pe o coloană

#define M 51    // numărul maxim de coloane
                //  dintr-un fagure

#define MAX 3 * N * M // dimensiunea maximă
                // a șirului de caractere
                // citit de la stdin

#define Q 2 * N  // dimensiunile maxime
#define W 3 * M  // ale matricei de caractere
                 // în care voi construi fagurele

#define newl printf("\n");  // macro pentru comoditate


// ------------------------------------------------------------------------------
// funcție pentru convertirea caracterelor citite
//  în numere de una sau două cifre;
// la final, sare peste două sau trei poziții,
//  deoarece, pe următoarea va fi un spațiu

short numar(char* str, short* i)
{
    short x = 0;

    if(isdigit(str[*i + 1]))
    {
        x = (short)(str[*i] - '0') * 10
          + (short)(str[*i + 1] - '0');
        *i += 3;
    }
    else
    {
        x = (short)(str[*i] - '0');
        *i += 2;
    }

    return x;
}


// ------------------------------------------------------------------------------
// desenarea unei celule din fagure
//  pornind din 'colțul' stânga-sus (i, j);
// dacă se află sau nu o regină aici
//  va pune 'Q', respectiv, spațiu

void desen_celula(char S[][W], short celule[M], short regina[][N], int i, int j)
{
    S[i][j] = '/';
    S[i][j + 2] = '\\';

    S[i + 1][j] = '\\';
    S[i + 1][j + 1] = '_';
    S[i + 1][j + 2] = '/';

    if(regina[(j + 1) / 2][(i + 1) / 2])
         S[i][j + 1] = 'Q';
    else S[i][j + 1] = ' ';

    if(celule[(j + 3) / 2] >= i / 2)
         S[i][j + 3] = '_';
}


// ------------------------------------------------------------------------------
// în cazul în care prima coloană este ridicată,
//  încep prin a construi coloanele impare,
//  apoi și pe cele pare

void caz_R(char fagure[][W], short celule[M], short regina[][N], short n)
{
    short i, j, x;

    // pun carectere '_' pe prima linie,
    //  deasupra fiecărei coloane impare
    for(i = 1; i <= (n + 1) / 2; ++i)
        printf(" _  ");
    newl;

    // generare fagure pe coloane
    for(i = 1; i <= n; ++i)
    {
        x = 2 * i - 1;
        for(j = ((i + 1) % 2) + 1; j <= 2 * celule[i]; j += 2)
            desen_celula(fagure, celule, regina, j, x);
    }
}


// ------------------------------------------------------------------------------
// în cazul în care prima coloană este coborâtă,
//  încep prin a construi coloanele pare,
//  apoi și pe cele impare

void caz_C(char fagure[][W], short celule[M], short regina[][N], short n)
{
    short i, j, x;

    // pun carectere '_' pe prima linie,
    //  deasupra fiecărei coloane pare
    for(i = 1; i <= n / 2; ++i)
        printf("   _");
    newl;

    // caz special pentru caracterul '_'
    //  de deasupra primei coloane
    fagure[1][2] = '_';

    // generare fagure pe coloane
    for(i = 1; i <= n; ++i)
    {
        x = 2 * i - 1;
        for(j = (i % 2) + 1; j <= 2 * celule[i]; j += 2)
            desen_celula(fagure, celule, regina, j, x);
    }
}

// ------------------------------------------------------------------------------


int main()
{
    char c;     // litera care sugerează 
                //  dacă prima coloana a fagurelui 
                //  este ridicată sau coborâtă
    
    short n,    // numărul de coloane
                //  din fagure
          lg,   // lungimea șirului de caractere 
                //  citit în buffer
          maxc; // numărul maxim de celule
                //  pe care îl are o coloană din fagure

    short i, j, x, y; // variabile auxiliare

    short celule[M];    // vector în care să rețin
                        //  numărul de celule al fiecărei coloane

    short regina[M][N]; // matrice în care să rețin
                        //  dacă se află o regină în 
                        //  celula j de pe coloana i
                        //  ( regina[i][j] = 0 / 1 )

    char buffer[MAX];      // buffer pentru citire

    char fagure[Q][W];  // matrice de caractere 
                        //  în care construiesc fagurele


    // construiesc câte un fagure pentru fiecare test de intrare,
    //  care se introduce pe câte o linie din stdin
    while(fgets(buffer, MAX, stdin) != NULL && buffer[0] != '\n')
    {
        // initializez tablourile
        //  si variabilele auxiliare
        //  pentru fiecare test

        memset(celule, 0, sizeof(celule));
        memset(regina, 0, sizeof(regina));
        memset(fagure, ' ', sizeof(fagure));
        
        x = y = maxc = 0; 
        lg = strlen(buffer);
        n = 1;

        // după ce am citit întreaga linie în buffer,
        //  transform cifrele în numere întregi, 
        //  corespunzătoare numerelor de celule
        //  de pe fiecare coloană din fagure,
        // și mă opresc la apariția caracterului 'C' sau 'R'

        for(i = 0; i < lg;)
        {
            if(isdigit(buffer[i]))
            {
                celule[n] = numar(buffer, &i);
                
                if(celule[n] > maxc)
                    maxc = celule[n];
                
                ++n;
            }

            else if(buffer[i] == 'C')
            {
                c = 'C';  break;
            }

            else if(buffer[i] == 'R')
            {
                c = 'R';  break;
            }
        }

        // pe poziția i din buffer se află caracterul 'C' sau 'R',
        //  iar pe poziția i + 1 va fi un spațiu sau nimic,
        //  deci i trebuie să sară două poziții,
        //  iar din n trebuie scăzut 1 pentru corectarea unei erori
        //  de la construirea vectorului celule[]
        i += 2;  n--;
        
        // rețin unde se află reginele
        if(isdigit(buffer[i]))
            while(i < lg)
            {
                x = numar(buffer, &i);
                y = numar(buffer, &i);
                regina[x][y] = 1;
            }

        // tratez cazurile separat
        if(c == 'R')
             caz_R(fagure, celule, regina, n);
        else caz_C(fagure, celule, regina, n);

        // afișarea fagurelui obținut
        for(i = 1; i <= 2 * maxc + 1; i++)
        {
            for(j = 1; j <= 2 * n + 1; j++)
                printf("%c", fagure[i][j]);
            newl;
        }
    }

    return 0;
}
