/*
 * Soubor:  proj3.c
 * Datum:   2013/12/15
 * Autor:   Václav Bayer, xbayer05@stud.fit.vutbr.cz
 * Projekt: Hledání obrazců, projekt è.3 pro pøedmìt IZP
 * Popis:   Program, který v daném monochromatickém obrázku hledá nejdelší vodorovné a svislé úsečky a největší čtverce.
 * Obrázek je uložen jako bitmapa v textovém souboru ve formě obdélníkové matice nul a jedniček. Cílem programu je výpis
 * počáteční a koncové souřadnice nalezené úsečky nebo čtverce.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int rows;
    int cols;
    char *cells;
}Bitmap;

void volej_hline(Bitmap *bitmapa);
void volej_vline(Bitmap *bitmapa);
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
char getcolor(Bitmap *bitmap, int x, int y);
int rozrad(int argc, char **argv);
void alloc2d(int sirka, int vyska, Bitmap *bitmap);
void free2d (Bitmap *bitmap);

enum tecodes
{
  NOPEN,
  HELPMSG,
};

const char *ECODEMSG[] =
{
  [NOPEN] = "Soubor se nepovedlo otevrit.",
  [HELPMSG] =
  "Program Hledani obrazcu, projekt c.3 pro predmet IZP.\n"
  "Autor: Vaclav Bayer (c) 2013\n"
  "Program, ktery v danem monochromatickem obrazku hleda nejdelsi vodorovne\n"
  "a svisle usecky a nejvetsi ctverce.\n"
  "Pouziti: ./proj3 --help               zpusobi, ze vypise napovedu pouzivani\n"
  "                                      a program ukonci.\n"
  "         ./proj3 --test  soubor.txt   pouze zkontroluje, ze soubor dany druhym \n"
  "                                      argumentem programu obsahuje radnou\n"
  "                                      definici bitmapoveho obrazku. \n"
  "                                      Tiskne Valid/Invalid podle vysledku.\n"
  "         ./proj3 --hline obrazek.txt  v danem obrazku nalezne a vytiskne  \n"
  "                                      pocatecni a koncovou souradnici prvni  \n"
  "                                      nejdelsi horizontalni usecky.\n"
  "         ./proj3 --vline obrazek.txt  v danem obrazku nalezne a vytiskne  \n"
  "                                      pocatecni a koncovou souradnici prvni \n"
  "                                      nejdelsi vertikalni usecky.\n"
  "         ./proj3 --square obrazek.txt v danem obrazku nalezne a vytiskne  \n"
  "                                      pocatecni a koncovou souradnici \n"
  "                                      prvniho nejvetsiho ctverce.\n",
};

void alloc2d(int sirka, int vyska, Bitmap *bitmap) {
    bitmap->rows = sirka;
    bitmap->cols = vyska;
    bitmap->cells = (char*)malloc(sirka*vyska*sizeof(char));
}

void free2d (Bitmap *bitmap){
    free(bitmap->cells);
    bitmap->cells = NULL;
}

char getcolor(Bitmap *bitmap, int x, int y){
    return bitmap->cells[y*bitmap->cols + x];
}

Bitmap test (char **argv)
{
    FILE *f;
    int r = 0;
    int c = 0;
    int x;
    Bitmap bitmapa;

    if((f=fopen(argv[2], "r")) == NULL)
    {
        bitmapa.cols = 0;
        bitmapa.rows = 0;
        bitmapa.cells = NULL;
        fclose(f);
        return bitmapa;
    }

    if (fscanf(f, "%d%d", &r, &c) !=2)
    {
        bitmapa.cols = 0;
        bitmapa.rows = 0;
        bitmapa.cells = NULL;
        fclose(f);
        return bitmapa;
    }
    alloc2d(r,c,&bitmapa);
    for (int i = 0; i < r*c; i++)
    {
        if (fscanf(f, "%d", &x)!=1)
        {
            bitmapa.cols = 0;
            bitmapa.rows = 0;
            bitmapa.cells = NULL;
            fclose(f);
            return bitmapa;
        }
        if ((x != 1) && (x != 0))
        {
            bitmapa.cols = 0;
            bitmapa.rows = 0;
            bitmapa.cells = NULL;
            fclose(f);
            return bitmapa;
        }
        bitmapa.cells[i] = x;
    }
    if (fscanf(f, "%d", &x)==1)
    {
        bitmapa.cols = 0;
        bitmapa.rows = 0;
        bitmapa.cells = NULL;
        fclose(f);
        return bitmapa;
    }

    fclose(f);
    return bitmapa;
}

int rozrad(int argc, char **argv)
{
    switch(argc){
    default:
        printf("%s", ECODEMSG[HELPMSG]);
        break;

    case 2:
        if (strcmp(argv[2], "--help") == 0)
            printf("%s", ECODEMSG[HELPMSG]);
        else
            printf("%s", ECODEMSG[HELPMSG]);
        break;

    case 3:
        if ((strcmp(argv[1], "--test")) == 0)
        {
            Bitmap bitmapa = test(argv);
            if (bitmapa.cells == NULL)
                printf("Invalid\n");
            else
                printf("Valid\n");
        return 0;
        }
        if ((strcmp(argv[1], "--hline")) == 0)
        {
            Bitmap bitmapa = test(argv);
            if (bitmapa.cells == NULL)
                printf("Invalid\n");
            else
            {
                volej_hline(&bitmapa);
                free2d(&bitmapa);
            }
        return 0;
        }
        if ((strcmp(argv[1], "--vline")) == 0)
        {
            Bitmap bitmapa = test(argv);
            if (bitmapa.cells == NULL)
                printf("Invalid\n");
            else
            {
                volej_vline(&bitmapa);
                free2d(&bitmapa);
            }
        return 0;
        }
        /*if ((strcmp(argv[1], "--square")) == 0)
        {
            Bitmap bitmapa = test(argc,argv);
            if (bitmapa.cells == NULL)
                printf("Invalid");
            else
            {
                volej_square(bitmapa);
                free2d(&bitmapa);
            }
        return 0;
        }*/
        if (((strcmp(argv[1], "--test")) != 0) && ((strcmp(argv[1], "--vline")) != 0) && ((strcmp(argv[1], "--hline")) != 0)
            && ((strcmp(argv[1], "--square")) != 0))
            printf("%s", ECODEMSG[HELPMSG]);
        break;

    }
return 0;
}

void volej_hline(Bitmap *bitmapa)
{
    int x1=0;
    int y1=0;
    int x2=0;
    int y2=0;
    int over = find_hline(bitmapa, &x1, &y1, &x2, &y2);
    if (over)
        fprintf(stderr, "Zadana matice neobsahuje zadnou horizontalni usecku.");
        else
        printf("%d %d %d %d\n", x1, y1, x2, y2);
}

int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int X;
    int Y;
    int linka = 0;
    int stara_linka = 0;

    for(int j = 0; j < bitmap->rows; j++)
    {
        for(int i = 0; i < bitmap->cols; i++)
        {
            if ((getcolor(bitmap,i,j)) == 1)
            {
                linka++;
                X = j;
                Y = i;
            }
            else
            {
                if (stara_linka < linka)
                {
                    *x1 = X;
                    *y1 = Y - linka + 1;
                    *x2 = X;
                    *y2 = Y;
                    stara_linka = linka;
                }
                linka = 0;
            }
        }
        if (stara_linka < linka)
        {
            *x1 = X;
            *y1 = Y - linka + 1;
            *x2 = X;
            *y2 = Y;
            stara_linka = linka;
        }
        linka = 0;
    }
if(stara_linka < 2)
    return 1;
else
    return 0;
}

void volej_vline(Bitmap *bitmapa)
{
    int x1=0;
    int y1=0;
    int x2=0;
    int y2=0;
    int over = find_vline(bitmapa, &x1, &y1, &x2, &y2);
    if (over)
        fprintf(stderr, "Zadana matice neobsahuje zadnou vertikalni usecku.");
        else
        printf("%d %d %d %d\n", y1, x1, y2, x2);
}

int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int X;
    int Y;
    int linka = 0;
    int stara_linka = 0;

    for(int j = 0; j < bitmap->cols; j++)
    {
        for(int i = 0; i < bitmap->rows; i++)
        {
            if ((getcolor(bitmap,j,i)) == 1)
            {
                linka++;
                X = j;
                Y = i;
            }
            else
            {
                if (stara_linka <= linka)
                {
                    *x1 = X;
                    *y1 = Y - linka + 1;
                    *x2 = X;
                    *y2 = Y;
                    stara_linka = linka;
                }
                linka = 0;
            }
        }
        if (stara_linka <= linka)
        {
            *x1 = X;
            *y1 = Y - linka + 1;
            *x2 = X;
            *y2 = Y;
            stara_linka = linka;
        }
        linka = 0;
    }
if(stara_linka < 2)
    return 1;
else
    return 0;
}


int main(int argc, char **argv)
{
    rozrad(argc, argv);
    return 0;
}


