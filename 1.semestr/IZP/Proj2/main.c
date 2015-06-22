/*
 * Soubor:  proj2.c
 * Datum:   2013/11/28
 * Autor:   Václav Bayer, xbayer05@stud.fit.vutbr.cz
 * Projekt: Iteraèní výpoèty, projekt è.2 pro pøedmìt IZP
 * Popis:   Program poèítá a vypisuje úhly obecného trojúhelníku. Trojúhelník je dán tøemi vrcholy a výpoèet je provádìn
 * pouze pomocí matematických operací +,-,*,/.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-13
#define PI 3.14159265358979323846
#define PIPUL 1.57079632679489661923


double my_acos(double x);
double my_asin(double x);

enum tecodes
{
  NAN,
  HELPMSG,
};

/* constanty vypisu nan a napovedy*/
const char *ECODEMSG[] =
{
  [NAN] = "nan\n",
  [HELPMSG] =
  "Program Iteracni vypocty, projekt c.2 pro predmet IZP.\n"
  "Autor: Vaclav Bayer (c) 2013\n"
  "Program pocita a vypisuje odmocninu, arcus sinus a uhly obecneho trojuhelniku.\n"
  "Pouziti: ./proj2 --help      zpusobi, ze vypise napovedu pouzivani\n"
  "                             a program ukonci.\n"
  "         ./proj2 --sqrt  X   vypocita a vypise druhou odmocninu z cisla X.\n"
  "         ./proj2 --asin  X   vypocita a vypise arcus sinus z cisla X.\n"
  "         ./proj2 --triangle AX AY BX BY CX CY    vypocita a vypise tri uhly.\n",
};

/* struktura pro bod */
typedef struct bod
{
  double X;
  double Y;
} souradnice;

/* prirazeni parametru, vola se pri vypoctu delky stran */
souradnice getParams(double X, double Y)
{
        souradnice result =
        {
            .X = X,
            .Y = Y,
        };
        return result;

}

/* absolutni hodnota */
double my_abs(double x)
{
	return (x < 0 ? -x : x);
}

/* overeni vstupnich hodnot jestli jsou cislo ci nikoli */
int je_cislo(char *argv)
{
    int i = 0;
    int t = 0;
    int e = 0;

    while (argv[i] != '\0')
    {
        if ((argv[i] == '-' && i == 0) || ((argv[i] == '.') && (t == 0) && (e == 0)) || ((argv[i] >= '0') && (argv[i] <= '9'))
            || ((argv[i] == 'e') && (e == 0) && (i != 0)))
        {

            if (argv[i] == 'e')
            {
                e++;
            }
            if ((argv[i] == 'e') && (argv[i + 1] == '-'))
            {
                i++;
            }
            if (argv[i] == '.')
            {
                t++;
            }
        }
        else
        {
            return 0;
        }
        i++;
    }
    return 1;
}

/* odmocnina */
double my_sqrt(double x)
{
    if (x < 0)
    {
        return -1;
    }

	double y = 1;
	double ny = 0.5 * (x/y + y);

	while (my_abs(ny - y) >= my_abs(EPS*ny)){
		y = ny;
		ny = 0.5 * (x/y + y);
	}
	return ny;
}

/* vypocet delky strany */
double delka_strany(int x1,int y1,int x2,int y2,char **argv)
{
    souradnice bodA = getParams(strtod(argv[x1], NULL), strtod(argv[y1], NULL));
    souradnice bodB = getParams(strtod(argv[x2], NULL), strtod(argv[y2], NULL));

    double ax = my_abs(bodA.X - bodA.Y);
    double ay = my_abs(bodB.X - bodB.Y);

    return my_sqrt((ax*ax)+(ay*ay));
}

/* vypocet uhlu trojuhelniku */
double uhly(char *argv[])
{
    double a = delka_strany(4,6,5,7, argv);
    double b = delka_strany(2,6,3,7, argv);
    double c = delka_strany(2,4,3,5, argv);


    if ((a+b <= c) || (a+c <= b) || (c+b <= a)){
        fprintf(stderr, "%s", ECODEMSG[NAN]);
        return 0;
    }

    double a2 = (((-(a*a)+(b*b)+(c*c))/(2*b*c)));
    double c2 = (((-(b*b)+(a*a)+(c*c))/(2*a*c)));
    double b2 = ((-(c*c)+(a*a)+(b*b))/(2*a*b));

    a = my_acos(a2);
    printf("%.10e\n", a);

    c = my_acos(c2);
    printf("%.10e\n", c);

    b = my_acos(b2);
    printf("%.10e\n", b);

    return 0;
}

/* arcus cosinus*/
double my_acos(double x)
{

    double s = my_asin(x);
    if ((x > -0.5) && (x < 0))
    {
        s = (PIPUL - s);
        return s;
    }
    else
    {
        s = (PIPUL - s);
        return s;
    }
}

/* arcus sinus */
double my_asin(double x)
{
    if ((x < -1) || (x > 1))
    {
        fprintf(stderr, "%s", ECODEMSG[NAN]);
        return -2;
    }
    double pravex = 0;


    if (x < 0)
    {
        pravex = x;
        x = my_abs(x);
    }


    double stareI = 1;
    double stareX = x;
    double s = x;
    double t = 1;
    double i = 0;

    while (my_abs(t) > my_abs(EPS * t))
    {
        t = (stareI * ((i + 1) / (i + 2))) * ((stareX * (x * x)) / (i + 3));

        stareI *= ((i + 1) / (i + 2));
        stareX *= x * x;
        s += t;
        i += 2;
    }
    if (pravex < 0)
    {
        return -s;
    }
    return s;

}


int main(int argc, char *argv[])
{
    switch(argc){               /* overeni vstupnich argumentu pomoci switch a dale overeni hodnot x pomoci funkce je_cislo */
        default:
            printf("%s", ECODEMSG[HELPMSG]);       // defaultne vraci napovedu

        case 1:
            printf("%s", ECODEMSG[HELPMSG]);        // 0 argumentu = napoveda
        break;

        case 2:
            if ((strcmp(argv[1], "--help")) == 0)           // 1.argument = --help -> vypis napovedu
            {
                printf("%s", ECODEMSG[HELPMSG]);
            }
            else{
                printf("%s", ECODEMSG[HELPMSG]);
            }
        break;

        case 3:
            if ((strcmp(argv[1], "--sqrt")) == 0){          // 1.argument = --sqrt -> over x
                if (je_cislo(argv[2]))                      // x vyhovuje
                {
                    double x = strtod(argv[2], NULL);
                    if(x == 0)                              // x = 0
                    {
                        printf("%.10e", x);
                        return 0;
                    }
                    if (my_sqrt(x) == -1)                           // x je mensi jak -1 nebo vetsi jak 1, vypis nan
                    {
                        fprintf(stderr, "%s", ECODEMSG[NAN]);
                        return 0;
                    }
                    else
                    printf("%.10e\n", my_sqrt(x));              // odmocni x
                }
                else
                {
                    fprintf(stderr, "%s", ECODEMSG[NAN]);
                }
            }
            else if((strcmp(argv[1], "--asin")) == 0)               // 1.argument = --asin -> over x
            {
                if (je_cislo(argv[2]))                      // x vyhovuje, vypocitej asin x
                {
                    if (((strtod(argv[2], NULL) < -0.5) && (strtod(argv[2], NULL) > -1))
                        || ((strtod(argv[2], NULL) > 0.5) && (strtod(argv[2], NULL) < 1)))
                        printf("%.10e\n", (my_acos(my_sqrt(1 - strtod(argv[2], NULL) * strtod(argv[2], NULL)))));
                    else
                        if (my_asin(strtod(argv[2], NULL)) == -2)
                            return 0;
                        else
                        printf("%.10e\n", my_asin(strtod(argv[2], NULL)));
                }
                else                    // x nevyhovuje, vypis nan
                {
                    fprintf(stderr, "%s", ECODEMSG[NAN]);
                }
            }
        break;

        case 8:
            if ((strcmp(argv[1], "--triangle")) == 0){          // 1.argument = --triangle -> over souradnice bodu
                if (je_cislo(argv[2]) && je_cislo(argv[3]) && je_cislo(argv[4]) && je_cislo(argv[5]) &&
                    je_cislo(argv[6]) && je_cislo(argv[7]))             // souradnice vyhovuji, vypocitej uhly v trojuhelniku
                uhly(argv);
                else                                // souradnice neodpovidaji pozadovanemu vstupu
                    fprintf(stderr, "%s", ECODEMSG[NAN]);
            }
            else                // chybny argument -> vypis napovedu
            {
                printf("%s", ECODEMSG[HELPMSG]);
            }
        break;


    }

	return 0;
}
