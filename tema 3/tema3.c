#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20  //  numărul maxim de caractere pentru un string citit

// ----------------------- STRUCTURI ----------------------------

//  pereche (nume oraș, număr de cadouri luate de-acolo)
typedef struct
{
    char* nume;
    int numar;
} pereche;

//  vector alocat dinamic pentru a reține cadourile
//      și numărul de apariții pentru fiecare cadou
//      colectat de Moșul dintr-un anumit oraș
typedef struct
{
  pereche *v;
  unsigned end, dim;
} vector;

//  rețin orașele prin care trece Moșul într-un arbore binar
//      de căutare, pentru a le păstra în ordine lexicografică
//      după nume, și pentru a insera un nou oraș sau un cadou
//      într-un oraș în timp aproximativ logaritmic
struct oras
{
	char* nume;
	vector* cadouri;

	struct oras* st;
	struct oras* dr;
};

//  am nevoie și de o matrice de regiuni în care introduc
//      informațiile citite și în care apoi mă 'plimb',
//      pentru a trece prin regiunile indicate
typedef struct
{
    char* nume_oras;
    char* nume_cadou;
    int nr_cadouri;
    char directie;
} regiune;


//  --------------------- FUNCȚII DE PRELUCRARE ----------------------
//  --------------------- A STRUCTURILOR DE DATE ---------------------

//  inițializarea unui vector de 'perechi' alocat dinamic
void init(vector* vect)
{
    vect->dim = 2;  //  inițial poate conține maxim două elemente,
                    //      deci setez dimensiunea inițială la 2
    vect->end = 0;  //  end = următoarea poziție de după ultima
                    //      pe care a fost inserat un element 
    vect->v = (pereche *)malloc(vect->dim * sizeof(pereche));
}


//  funcție pentru inserarea unui element de tip 'pereche',
//      adică a unui tip de cadou și a numărului de cadouri
//      de acest tip, pe care Moșul le va aduna dintr-un oraș
void insert(vector* vect, char* nume)
{
    unsigned i;
    
    //  vectorii alocați dinamic se vor redimensiona singuri
    //      atunci când se umplu, dublându-și dimensiunea
    if(vect->dim == vect->end)
    {
        vect->dim <<= 1;
        vect->v = (pereche *)realloc(vect->v, vect->dim * sizeof(pereche));
    }

    //  întâi caut să văd dacă tipul de cadou pe care vreau să-l inserez
    //      există deja în vector; atunci va trebui doar să incrementez
    //      numărul de cadouri de acel tip colectate din orașul curent
    for(i = 0; i < vect->end; ++i)
        if(strcmp(nume, vect->v[i].nume) == 0)
        {
            vect->v[i].numar++;
            return;
        }

    //  în cazul în care tipul de cadoul nu există în vector,
    //      acesta trebuie inserat, cu numărul inițial setat la 1
    vect->v[vect->end].numar = 1;
    vect->v[vect->end++].nume = strdup(nume);
}


//  funcție pentru crearea unui nou nod pentru arborele de orașe
struct oras* oras_nou(char* nume_oras, char* nume_cadou, int ia_cadou)
{
	struct oras* aux = (struct oras *)malloc(sizeof(struct oras));

	aux->nume = strdup(nume_oras);
	aux->st = aux->dr = NULL;

    aux->cadouri = malloc(sizeof(vector));
    init(aux->cadouri);

    //  există posibilitatea ca în regiunea curentă 
    //      să nu (mai) existe cadouri;
    //      atunci trebuie să știu dacă Moșul va putea
    //      lua un cadou, pentru a-l insera în vector 
    if(ia_cadou) insert(aux->cadouri, nume_cadou);

    // la final, întorc un pointer la nodul proaspăt creat
	return aux;
}


//  funcție pentru inserarea unui nou oraș (plus un cadou) în arbore
//      sau a unui cadou într-un oraș existent
struct oras* inserare_oras(struct oras* oras, char* nume_oras, char* nume_cadou, int ia_cadou)
{
	//  dacă orașul nu există în arbore 
    //      și aceasta este poziția potrivită
    //      creez un nou nod pentru acesta
    //      și întorc un pointer la el 
    if(!oras) return oras_nou(nume_oras, nume_cadou, ia_cadou);

    //  vreau să compar lexicografix numele a două orașe
    //      (cel curent și cel pe care vreau să-l inserez)
    int aux = strcmp(nume_oras, oras->nume);

    //  dacă orașul există în arbore, inserez doar cadoul în acesta
    if(aux == 0 && ia_cadou == 1)
        insert(oras->cadouri, nume_cadou);
	
    //  altfel, parcurg arborele binar de căutare
    //      pentru a găsi locul potrivit în care să inserez noul oraș
    else if(aux < 0)
		oras->st = inserare_oras(oras->st, nume_oras, nume_cadou, ia_cadou);
	else if(aux > 0)
		oras->dr = inserare_oras(oras->dr, nume_oras, nume_cadou, ia_cadou);

    //  la final, întorc un pointer la orașul curent
	return oras;
}


// ---------------------- ALTE FUNCȚII ------------------------
//  ---------------------- (AUXILIARE) ------------------------


//  deplasarea Moșului la următorul oraș
//      conform indicațiilor din regiunea curentă
void deplasare(int* x, int* y, char directie)
{
         if(directie == 'U') { --*x; return; }
    else if(directie == 'D') { ++*x; return; }
    else if(directie == 'L') { --*y; return; }
    else if(directie == 'R') { ++*y; return; }
}


//  funcție folosită ca parametru la qsort()
//      pentru ordonarea orașelor lexicografic
//      și a cadourilor descrescător după numărul
//      de apariții, apoi lexicografic, dacă 
//      două tipuri de cadou au același număr
int comparator(const void* a, const void* b)
{
    pereche x = *(const pereche *)a;
    pereche y = *(const pereche *)b;
    int aux = strcmp(x.nume, y.nume);

    if(x.numar == y.numar)
    {
        if(aux == 0) return 0;
        else if(aux < 0) return -1;
        else if(aux > 0) return  1;
    }
    else if(x.numar < y.numar) return 1;
    else if(x.numar > y.numar) return -1;
}


//  funcție pentru afișarea tuturor cadourilor adunate
//      în ordinea cerută în enunț
void parcurgere_cadouri(vector* vect)
{
	qsort(vect->v, vect->end, sizeof(pereche), comparator);

    unsigned i;
	for(i = 0; i < vect->end; ++i)
        printf("  %d %s\n", vect->v[i].numar, vect->v[i].nume);
}


//  funcție pentru afișarea tuturor orașelor prin care
//      a trecut Moșul, precum și a cadourilor din fiecare
void parcurgere_orase(struct oras* oras)
{
	if(oras)
	{
		parcurgere_orase(oras->st);
		if(oras->cadouri->end)
            printf("%s: \n", oras->nume);
		parcurgere_cadouri(oras->cadouri);
		parcurgere_orase(oras->dr);
	}
}


//  dealocarea arborelui de orașe și a vectorilor alocați dinamic
void dealocare(struct oras* oras)
{
    if(oras)
    {
        unsigned i;

        dealocare(oras->st);
        dealocare(oras->dr);

        for(i = 0; i < oras->cadouri->end; ++i)
            free(oras->cadouri->v[i].nume);
        
        free(oras->cadouri->v);
        free(oras->cadouri);
        free(oras->nume);
        free(oras);        
    }
}

// ------------------------- The MAIN() ------------------------------

int main()
{
	int N, M, x, y, nr_pasi;
    char* nume_oras = malloc(MAX);
    char* nume_cadou = malloc(MAX);
    int i, j, nr_cadouri_adunate = 0;

    //freopen("input/test7.in", "r", stdin);
    //freopen("output/test10.out", "w", stdout);

    scanf("%d%d", &N, &M);
    scanf("%d%d", &x, &y);
    scanf("%d", &nr_pasi);

    //  aloc o matrice de regiuni care va fi un fel de hartă
    //      prin care Moșul se va deplasa după indicațiile primite
    regiune** harta = (regiune **)malloc(N * sizeof(regiune *));
    for(i = 0; i < N; ++i)
        harta[i] = (regiune *)malloc(M * sizeof(regiune));

    //  aloc un prim nod din arbore, care va fi rădacina;
    //      acesta va fi orașul din care Moșul pornește 
	struct oras* orase = NULL;

    //  citesc datele de intrare și le stochez în 'hartă'
	for(i = 0; i < N; ++i)
    {
        for(j = 0; j < M; ++j)
        {
            scanf("%s", nume_oras);
            harta[i][j].nume_oras = strdup(nume_oras);

            scanf("%s", nume_cadou);
            harta[i][j].nume_cadou = strdup(nume_cadou);

            scanf("%d", &harta[i][j].nr_cadouri);
            scanf(" %c", &harta[i][j].directie);
        }
    }
    
    //  există posibilitatea ca Moșul să nu gasească niciun cadou
    //      în primul oraș pe care-l vizitează; în acest caz,
    //      trebuie inserat orașul, dar fără vreun cadou
    if(harta[x][y].nr_cadouri)
    {
        nr_cadouri_adunate++;
        harta[x][y].nr_cadouri--;
        orase = inserare_oras(orase, harta[x][y].nume_oras,
                                     harta[x][y].nume_cadou, 1);
    }
    else orase = inserare_oras(orase, harta[x][y].nume_oras,
                                      harta[x][y].nume_cadou, 0);

    //  după ce am inserat primul oraș, trec prin următoarele,
    //      conform indicațiilor găsite, și inserez orașele 
    //      nou vizitate, eventual și cadourile, dacă există,
    //      dar fără să depășesc numărul maxim de pași
    for(i = 0; i < nr_pasi; ++i)
    {   
        //  mă deplasez la următorul oraș indicat
        deplasare(&x, &y, harta[x][y].directie);
        
        //  în cazul în care Moșul părăsește harta
        if(x < 0 || x >= N || y < 0 || y >= M)
        {
            printf("TOO MUCH SNOW !\n");
            break;
        }
        
        //  dacă există vreun cadou în orașul curent
        //      atunci Moșul va lua unul de-aici
        if(harta[x][y].nr_cadouri > 0)
        {
            nr_cadouri_adunate++;
            harta[x][y].nr_cadouri--;
            inserare_oras(orase, harta[x][y].nume_oras, 
                                 harta[x][y].nume_cadou, 1);
        } 
    }

    //  la final, voi afișa numărul de pași efectuați,
    //      numărul total de cadouri colectate,
    //      apoi orașele și cadourile din ele
    printf("%d \n", i);
    printf("%d \n", nr_cadouri_adunate);
	parcurgere_orase(orase);

    //  aici dealoc matricea de regiuni, apoi
    //      apelez funcția pentru dealocarea arborelui
    for(i = 0; i < N; ++i)
    {
        for(j = 0; j < M; ++j)
        {
            free(harta[i][j].nume_oras);
            free(harta[i][j].nume_cadou);
        }
        free(harta[i]);
    }
    free(harta);

    dealocare(orase);

    //  cele două string-uri auxiliare
    //      sunt, totuși, alocate dinamic;
    //      deci trebuie dealocate și ele
    free(nume_oras);
    free(nume_cadou);

	return 0;
}