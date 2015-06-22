#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int fun_arg1 (int argc, char *argv[]){
    int arg1;

    if (argc >= 2){
        if ((argv[1][0] >= 'A' && argv[1][0] <= 'Z') || (argv[1][0] >= '0' && argv[1][0] <= '9') ||
            (argv[1][0] == '-') || (argv[1][0] == '_') || (argv[1][0] >= 'a' && argv[1][0] <= 'z')){
            arg1 = argv[1][0];
            return arg1;
        }else{
            if(argv[1][0] == ':'){
                arg1 = argv[1][0];
                return arg1;
            }
            if(argv[1][0] == '^'){
                arg1 = argv[1][0];
                return arg1;
            }
            if(argv[1][0] == '.'){
                arg1 = argv[1][0];
                return arg1;
            }
        return 0;
        }
    }else{
    return 0;
    }
}
/* overi jestli je zadan alespon jeden argument obsahujicipovoleny znak
-v pripade kladneho vyhodnoceni vraci do funkce main arg1 s hodnotou prvniho argumentu se kterym dale pracuje
-v pripade zaporneho vyhodnoceni ukoncuje funkci*/

int preved_retezec (int argc, char *argv[]){
    int c;
    int arg2 = 0;
    int i = 0;
    if (argv[2][0] < 0)
        return 0;

    while ((c = argv[2][i]) != '\0'){
        arg2 = arg2*10 + (c - '0');
        i++;
    }
    if (argc >= 3){
        return arg2;
    }else{
    return 0;
    }
}
// prevede retezec poli na cilo, ktere vyjadruje poyici hledaneho znaku

int napoveda (char *argv[]){
    if (argv[1] == '\0'){
        printf("Program Pocitani znaku v retezci \n"
               "Autor: Vaclav Bayer (c) 2013 \n\n"
               "Program, ktery ve vstupnim textu spocita slova obsahujici uzivatelem\n"
               "specifikovany znak a jejich pocet vypise. Uzivatel specifikuje dany\n"
               "znak argumentem prikazove radky. Uzivatel navic muze specifikovat pozici\n"
               "znaku v pocitanem slove dalsim argumentem prikazove radky.\n\n"
               "Program se spousti v nasledujici podobe:\n"
               "\t./proj1 --help\n"
               "\t./proj1 X [N] -d\n"
               "\t--help - vypise napovedu a program se ukonci\n\n"
               "\tX reprezentuje hledany znak v pocitanych slovech. Specialni znak \":\" \n"
               "\tnavic symbolizuje libovolne cislo 0-9, znak \"^\" symbolizuje \n"
               "\tlibovolne velke pismeno A-Z a znak \".\" symbolizuje libovolny znak.\n\n"
               "\tVolitelny argument N predstavuje cislo vyjadrujici pozici hledaneho\n"
               "\tznaku v pocitanych slovech. Platne pocitane slovo musi mit hledany\n"
               "\tznak prave na dane pozici. Cislo N je kladne - prvni znak ve slove\n"
               "\tma pozici 1.\n\n"
               "\tVolitelny argument -d zapina tzv. ladici (debug) rezim programu.\n"
               "\tV ladicim rezimu program na vystup prubezne vypisuje kazde nactene\n"
               "\tslovo, jedno slovo na radek. Pocet slov je programem teprve po\n"
               "\tskonceni pocitani.\n");
        return 0;
    }else{
        if (strcmp(argv[1],"--help") == 0){
            printf("Program Pocitani znaku v retezci \n"
               "Autor: Vaclav Bayer (c) 2013 \n\n"
               "Program, ktery ve vstupnim textu spocita slova obsahujici uzivatelem\n"
               "specifikovany znak a jejich pocet vypise. Uzivatel specifikuje dany\n"
               "znak argumentem prikazove radky. Uzivatel navic muze specifikovat pozici\n"
               "znaku v pocitanem slove dalsim argumentem prikazove radky.\n\n"
               "Program se spousti v nasledujici podobe:\n"
               "\t./proj1 --help\n"
               "\t./proj1 X [N] -d\n"
               "\t--help - vypise napovedu a program se ukonci\n\n"
               "\tX reprezentuje hledany znak v pocitanych slovech. Specialni znak \":\" \n"
               "\tnavic symbolizuje libovolne cislo 0-9, znak \"^\" symbolizuje \n"
               "\tlibovolne velke pismeno A-Z a znak \".\" symbolizuje libovolny znak.\n\n"
               "\tVolitelny argument N predstavuje cislo vyjadrujici pozici hledaneho\n"
               "\tznaku v pocitanych slovech. Platne pocitane slovo musi mit hledany\n"
               "\tznak prave na dane pozici. Cislo N je kladne - prvni znak ve slove\n"
               "\tma pozici 1.\n\n"
               "\tVolitelny argument -d zapina tzv. ladici (debug) rezim programu.\n"
               "\tV ladicim rezimu program na vystup prubezne vypisuje kazde nactene\n"
               "\tslovo, jedno slovo na radek. Pocet slov je programem teprve po\n"
               "\tskonceni pocitani.\n");
        }else{
            return 1;
        }
    }
}
// -overi jestli uyivatel zadal argument "--help", ktery vypisuje napovedu
// -stejne tak vypise napovedu jestli uzivatel nezadal zadny argument

int max_arg (int argc){
    if (argc > 5){
        printf("Vypisuji napovedu");
        return 0;
    }else{
    return 1;
    }
}
// vypise napovedu jestli uzivatel zada vetsi nez povoleny pocet argumentu
//overi jestli neni zadan vetsi nez povoleny pocet argumentu

int fun_debug_pozice (int arg1, int pozice){
    int pocet = 0;
    int pocet2 = 0;
    int c;

    while((c = getchar()) != '\n'){
        if (c != ' ' && pocet2 < 80){
            putchar(c);
            pocet2++;
            if (pozice == pocet2 && c == arg1)
                pocet++;
        }
        if (c == ' '){
            printf("\n");
            pocet2 = 0;
        }
    }
    printf("\n%d\n", pocet);
    return 0;
}
// -pocita znaky na pozici, kterou zadal uzivatel
// -spousti spolu s debug modem

int fun_pozice (int arg1, int pozice){
    int pocet = 0;
    int pocet2 = 0;
    int c;

    while((c = getchar()) != '\n'){
        if (c != ' ' && pocet2 < 80){
            pocet2++;
            if (pozice == pocet2 && c == arg1)                                  //cast s pozici
                pocet++;
        }
        if (c == ' '){
            pocet2 = 0;
        }
    }
    printf("%d\n", pocet);
    return 0;
}
// -pocita znaky na pozici, kterou zadal uzivatel

int fun_debug(int arg1){
    int pocet = 0;
    int pocet2 = 0;
    int pocet3 = 0;
    int c;

    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
            if (c == arg1){
                pocet++;
                pocet2++;
            }
            putchar(c);
            pocet2++;
        }else{
            printf("\n");
            pocet2 = 0;
                if (pocet >= 1){
                pocet3++;
                pocet = 0;
                }
        }
    }
    if (pocet >= 1){
        pocet3++;
        pocet = 0;
    }
    printf("%d\n", pocet3);
    return 0;
}
// -pocita v kolika slovech se nachazi hledany znak
// -spousti spolu s debug modem

int pocitej (int arg1){
    int pocet = 0;
    int pocet2 = 0;
    int pocet3 = 0;
    int c;

    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
            if (c == arg1){
                pocet++;
                pocet2++;
            }
        }else{
            pocet2 = 0;
                if (pocet >= 1){
                pocet3++;
                pocet = 0;
                }
        }
    }
    if (pocet >= 1){
        pocet3++;
        pocet = 0;
    }

    printf("%d\n", pocet3);
    return 0;
}
// -pocita v kolika slovech se nachazi hledany znak

int special_pocitej (int arg1){
    int pocet = 0;
    int pocet2 = 0;
    int pocet3 = 0;
    int c;

    if (arg1 == ':'){
    while((c = getchar()) != '\n'){
            if ((c >= '1' && c <= '9') && (pocet2 < 80)){
                    pocet++;
                    pocet2++;
            }else{
                pocet2 = 0;
                    if (pocet >= 1){
                    pocet3++;
                    pocet = 0;
                    }
            }
        }
        if (pocet >= 1){
            pocet3++;
            pocet = 0;
        }
    printf("%d\n", pocet3);
    return 0;
    }
    if (arg1 == '^'){
    while((c = getchar()) != '\n'){
            if ((c >= 'A' && c <= 'Z')&& (pocet2 < 80)){
                    pocet++;
                    pocet2++;
            }else{
                pocet2 = 0;
                    if (pocet >= 1){
                    pocet3++;
                    pocet = 0;
                    }
            }
        }
        if (pocet >= 1){
            pocet3++;
            pocet = 0;
        }
    printf("%d\n", pocet3);
    return 0;
    }
    if (arg1 == '.'){
    while((c = getchar()) != '\n'){
            if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                (c == '-') || (c == '_')) && (pocet2 < 80)){
                    pocet++;
                    pocet2++;
            }else{
                pocet2 = 0;
                    if (pocet >= 1){
                    pocet3++;
                    pocet = 0;
                    }
            }
        }
        if (pocet >= 1){
            pocet3++;
            pocet = 0;
        }
    printf("%d\n", pocet3);
    return 0;
    }
}
/* -pocita v kolika slovech se nachazi hledany specialni znak
- ":" = cislice
- "." = male pismeno
- "^" = velke pismeno */

int special_pocitej_debug (int arg1){
    int pocet = 0;
    int pocet2 = 0;
    int pocet3 = 0;
    int c;

    if (arg1 == ':'){
    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
        putchar(c);
        pocet2++;
            if (c >= '1' && c <= '9'){
                    pocet++;
            }else{
                pocet2 = 0;
                    if (pocet >= 1){
                    pocet3++;
                    pocet = 0;
                    }
            }
        }else{
            printf("\n");
        }
        if (pocet >= 1){
            pocet3++;
            pocet = 0;
        }
    }
    printf("\n%d\n", pocet3);
    return 0;
    }
    if (arg1 == '^'){
    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
        putchar(c);
        pocet2++;
            if (c >= 'A' && c <= 'Z'){
                    pocet++;
            }else{
                pocet2 = 0;
                    if (pocet >= 1){
                    pocet3++;
                    pocet = 0;
                    }
            }
        }else{
            printf("\n");
        }
        if (pocet >= 1){
            pocet3++;
            pocet = 0;
        }
    }
    printf("\n%d\n", pocet3);
    return 0;
    }
    if (arg1 == '.'){
    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
        putchar(c);
        pocet2++;
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                (c == '-') || (c == '_')){
                    pocet++;
            }else{
                pocet2 = 0;
                    if (pocet >= 1){
                    pocet3++;
                    pocet = 0;
                    }
            }
        }else{
            printf("\n");
        }
        if (pocet >= 1){
            pocet3++;
            pocet = 0;
        }
    }
    printf("\n%d\n", pocet3);
    return 0;
    }
}
/* -pocita v kolika slovech se nachazi hledany specialni znak
-spousti se spolu s debug modem
- ":" = cislice
- "." = male pismeno
- "^" = velke pismeno */

int special_pozice_debug (int arg1, int pozice){
    int pocet = 0;
    int pocet2 = 0;
    int c;

    if (arg1 == ':'){
    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
            putchar(c);
            pocet2++;
            if ((c >= '1' && c <= '9') && (pozice == pocet2)){
                pocet++;
            }
        }else{
            printf("\n");
            pocet2 = 0;
        }
    }
    printf("\n%d\n", pocet);
    return 0;
    }
    if (arg1 == '^'){
    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
            putchar(c);
            pocet2++;
            if ((c >= 'A' && c <= 'Z') && (pozice == pocet2)){
                    pocet++;
            }
        }else{
            printf("\n");
            pocet2 = 0;
        }
    }
    printf("\n%d\n", pocet);
    return 0;
    }
    if (arg1 == '.'){
    while((c = getchar()) != '\n'){
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
            putchar(c);
            pocet2++;
            if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                (c == '-') || (c == '_')) && pocet2 == pozice){
                    pocet++;
            }
        }else{
            printf("\n");
            pocet2 = 0;
        }
    }
    printf("\n%d\n", pocet);
    return 0;
    }
}
/* -pocita v kolika slovech se nachazi hledany specialni znak na dane pozici uzivatelem
-spusti se s polu s debug modem
- ":" = cislice
- "." = male pismeno
- "^" = velke pismeno */

int special_pozice (int arg1, int pozice){
    int pocet = 0;
    int pocet2 = 0;
    int c;

    if(arg1 == ':'){
        while((c = getchar()) != '\n'){
            if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
                pocet2++;
                if (pozice == pocet2 && (c >= '1' && c <= '9') )                                  //cast s pozici
                    pocet++;
            }
            else{
                pocet2 = 0;
            }
        }
    printf("\n%d\n", pocet);
    return 0;
    }
    if(arg1 == '^'){
        while((c = getchar()) != '\n'){
            if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            (c == '-') || (c == '_')) && (pocet2 < 80)){
                pocet2++;
                if (pozice == pocet2 && (c >= 'A' && c <= 'Z') )                                  //cast s pozici
                    pocet++;
            }
            else{
                pocet2 = 0;
            }
        }
    printf("\n%d\n", pocet);
    return 0;
    }
    if(arg1 == '.'){
        while((c = getchar()) != '\n'){
            if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                (c == '-') || (c == '_')) && (pocet2 < 80)){
                pocet2++;
                if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                (c == '-') || (c == '_')) && (pozice == pocet2))
                    pocet++;
            }
            else{
                pocet2 = 0;
            }
        }
    printf("\n%d\n", pocet);
    return 0;
    }
}
/* -pocita v kolika slovech se nachazi hledany specialni znak na dane pozici uzivatelem
- ":" = cislice
- "." = male pismeno
- "^" = velke pismeno */

int main(int argc, char *argv[]){
    int arg1;
    int arg2;
    int pozice;

    arg1 = fun_arg1(argc,argv);
    if (napoveda(argv) == 1 && max_arg(argc) == 1){ //napoveda a pocet zadanych argumentu je vyhodnocen kladne
        if (argv[2] == '\0'){                       //je zadan jen jeden argument
                if ((arg1 >= 'a' && arg1 <= 'z') || (arg1 >= 'A' && arg1 <= 'Z') || (arg1 >= '0' && arg1 <= '9') ||
                (arg1 == '-') || (arg1 == '_')){        // 1.argument odpovida povolenym znakum a neni specialni znak
                    pocitej(arg1);
                    return 0;
                }
                if (arg1 == ':' || arg1 == '^' || arg1 == '.'){     // 1.argument je specialni znak
                    special_pocitej(arg1);
                    return 0;
                }
        }else{                                         //je zadano vice jak jeden argument (+pozice a debug mod)
            if (((arg2=argv[2][0]) >= '0') && ((arg2=argv[2][0]) <= '9')){      // 2.argumentem je pozice hledaneho znaku
                pozice = preved_retezec(argc,argv);
                if (argc == 4){                             //plati jestli jsou zadane vsechny 3argumenty
                    if (strcmp(argv[3], "-d") == 0){            // 3.argumentem je debug mod
                        if (arg1 == ':' || arg1 == '^' || arg1 == '.'){      // 1.argumentem je specialni znak, 2.argumentem zadana
                            special_pozice_debug(arg1, pozice);             // pozice a 3.argumentem je spusten debug mod
                            return 0;
                        }else{                                      // 1.argumentem neni specialni znak, 2.argumentem zadana
                        fun_debug_pozice(arg1, pozice);             // pozice a 3.argumentem je spusten debug mod
                        return 0;
                        }
                    }
                }
                if (arg1 == ':' || arg1 == '^' || arg1 == '.'){             // 1.argument je specialni znak a 2.argumentem
                    special_pozice(arg1, pozice);                           // je dana pozice
                    return 0;
                }else{                                      // 1.argumenem jsou  hledane znaky krom specialnich znaku
                    fun_pozice(arg1, pozice);               // 2.argumentem je dana pozice
                    return 0;
                }
            }
            if (strcmp(argv[2], "-d") == 0){
                if (arg1 == ':' || arg1 == '^' || arg1 == '.'){     // 1.argument je specialni znak a 2.argumentem se spousti debug
                    special_pocitej_debug(arg1);
                    return 0;
                }else{
                    fun_debug(arg1);         // 1.argument jsou hledane znaky krom specialnich znaku
                    return 0;               // 2.argumentem se spousti debug
                }
            }
        }
    }


    return 0;
}
