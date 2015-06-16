#!/usr/bin/python3
# -*- coding: utf-8 -*-
#CST:xbayer05

import sys
import re
import os
from collections import namedtuple

def prints_msg(number_msg, other_msg):
    """ Vytiskne chybove hlaseni
    @param number_msg: cislo chyby
    @param other_msg: pripojeni dodatecne zpravy k dane chybe """

    if number_msg==0:
        sys.stdout.write("Skript vypisuje statistiky: \n"
            "   -klicovych slov\n"
            "   -jednoduchych operatoru\n"
            "   -identifikatoru\n"
            "   -vyskytu zadaneho retezce\n"
            "   -komentaru\n\n"
            "Pripustne argumenty: \n"
            "--help.................zobrazi napovedu a ukonci skript\n"
            "--input=fileordir......vstupni soubor nebo adresar\n"
            "--nosubdir.............prohledavani provadeno pouze v zadanem adresari\n"
            "--output=filename......vystupni soubor\n"
            "-k.....................pocet vyskytu klicovych slov\n"
            "-o.....................pocet vyskytu jednoduchych operatoru\n"
            "-i.....................pocet vyskytu identifikatoru\n"
            "-w=pattern.............pocet neprekryvajicich se vyskytu zadaneho retezce\n"
            "-c.....................pocet znaku komentaru\n"
            "-p.....................vypisovani souboru bez absolutni cesty\n"
            "parametry -k, -o, -i, -w a -c nelze mezi sebou kombinovat\n")
    elif number_msg==1:
        sys.stderr.write("Nepovolena kombinace parametru. " + other_msg + "\n")
    elif number_msg==2:
        sys.stderr.write("Chyba pri cteni ze vstupniho souboru. " + other_msg + "\n")
    elif number_msg==3:
        sys.stderr.write("Chyba pri otevreni vystupniho souboru. " + other_msg + "\n")
    elif number_msg==4:
        sys.stderr.write("Chybny format vstupniho souboru. " + other_msg + "\n") 
    elif number_msg==21:
        sys.stderr.write("Nektery z analyzovanych souboru nejde cist. " + other_msg + "\n")
    else:
        sys.stderr.write(other_msg)
    exit(number_msg)


""" Globalni promenne """
arg_help = 0
arg_in = 0
flag_in = ""
arg_nosub = 0
arg_out = 0
flag_out = ""

no_reply = 0
arg_k = 0
arg_o = 0
arg_i = 0
arg_w = 0
flag_w = ""
arg_c = 0
arg_p = 0

DEBUG = 0

keys = (
        "\\bauto\\b", "\\bbreak\\b", "\\bcase\\b", "\\bchar\\b", "\\bconst\\b", "\\bcontinue\\b", 
        "\\bdefault\\b", "\\bdo\\b", "\\bdouble\\b", "\\belse\\b", "\\benum\\b", "\\bextern\\b", "\\bfloat\\b", 
        "\\bfor\\b", "\\bgoto\\b", "\\bif\\b", "\\binline\\b", "\\bint\\b", "\\blong\\b", "\\bregister\\b", 
        "\\brestrict\\b", "\\breturn\\b", "\\bshort\\b", "\\bsigned\\b", "\\bsizeof\\b", "\\bstatic\\b", 
        "\\bstruct\\b", "\\bswitch\\b", "\\btypedef\\b", "\\bunion\\b", "\\bunsigned\\b", "\\bvoid", 
        "\\bvolatile\\b", "\\bwhile\\b", "\\b_Bool\\b", "\\b_Complex\\b", "\\b_Imaginary\\b")


""" Ziskani vstupnich argumentu """
# argumenty se nesmi opakovat
if len(sys.argv) > len(set(sys.argv)):
    prints_msg(1, "")

for i in range(1, len(sys.argv)):
    # --help
    if sys.argv[i] == "--help":
        arg_help = 1;
    # --input
    elif re.search("^--input=.+$", sys.argv[i]):
        arg_in = 1;
        flag_in = re.sub("^--input=","",sys.argv[i])
    # --nosubdir
    elif sys.argv[i] == "--nosubdir":
        arg_nosub = 1;
    # --output
    elif re.search("^--output=.+$", sys.argv[i]):
        arg_out = 1;
        flag_out=re.sub("^--output=","",sys.argv[i])
    # -k
    elif sys.argv[i] == "-k":
        arg_k = 1;
        no_reply+= 1;
    # -o
    elif sys.argv[i] == "-o":
        arg_o = 1;
        no_reply+= 1;
    # -i
    elif sys.argv[i] == "-i":
        arg_i = 1;
        no_reply+= 1;
    # -w
    elif re.search("^-w=.+$", sys.argv[i]):
        arg_w = 1;
        no_reply+= 1;
        flag_w = re.sub("^-w=","",sys.argv[i])
    # -c
    elif sys.argv[i] == "-c":
        arg_c = 1;
        no_reply+= 1;
    # -p
    elif sys.argv[i] == "-p":
        arg_p = 1;
    # cokoliv jineho chyba 1
    else:
        prints_msg(1, "")


def check_args():
    """ Osetreni ziskanych argumentu 
        @return seznam souboru """

    # nelze --help a cokoliv jineho
    global arg_help, arg_in, flag_in, arg_nosub, no_reply
    if arg_help and len(sys.argv) != 2:
        prints_msg(1, "Napovedu nelze kombinovat s jinym argumentem.")
        
    # tisk napovedy
    if arg_help:
        prints_msg(0, "")

    # zakaz kompinovani jednoduchych argumentu
    if no_reply != 1:
        prints_msg(1, "Chybi anebo prebyva jednoduchy prepinac.")
        if DEBUG: print(no_reply)

    files = []

    # jestlize neni zadan input, prohledava se aktualni adresar
    if arg_in == 0:
        flag_in = os.path.dirname(sys.argv[0])
        flag_in = os.path.realpath(flag_in)
        if DEBUG: print(flag_in)
    # prohledavani input adresare/souboru
    else:
        flag_in = os.path.expanduser(flag_in)
        if arg_nosub == 1 and os.path.isfile(flag_in):
            prints_msg(1, "Nelze kombinovat paramety --nosub a --input=file.")

    if DEBUG: print(os.path.expanduser(flag_in))

    # jestlize se nejedna o adresar a ani soubor, tiskni chybu
    if not os.path.isdir(flag_in) and not os.path.isfile(flag_in):
        prints_msg(2, "Nejedna se o existujici adresar nebo soubor.")

    # konkretni soubor
    if os.path.isfile(flag_in):
        files.append(os.path.abspath(flag_in))
        if DEBUG: print("1.FILES: ",files)
        return files

    # konkretni adresar
    if os.path.isdir(flag_in):
        if arg_nosub == 0:
            for root, subFolders, f_files in os.walk(flag_in):
                for name in f_files:
                    if re.match('.+\.[chCH]$', name):
                        files.append(os.path.join(root, name))
        # neprohledavej podadresare
        else:
            files = [ os.path.join(flag_in, name) for name in os.listdir(flag_in) 
            if re.match('.+\.[chCH]$', name) 
            if os.path.isfile(os.path.join(flag_in, name)) ]

        if DEBUG: print("1.FILES: ",files)
        return files


def open_file(files):
    """ Postupne ziskani souboru a spusteni chteneho hledani 
        @param pole souboru
        @return slovnik souboru a jejich hodnot """

    global arg_p, arg_k, arg_o, arg_i, arg_w, arg_c, flag_w
    updateFiles = {}

    # ber soubor po souboru
    for one in files:
        # skus otevrit soubor
        try:
            openedFile = open(one, 'r', encoding='iso-8859-2')
            text = openedFile.read()
            openedFile.close()
        # nelze otevrit, ukonci s chybou
        except:
            if os.path.isfile(one):
                prints_msg(2, one)
            else:
                prints_msg(21, one)

        # vypis bez absolutni cesty
        if arg_p == 1:
            one = os.path.basename(one)
            updateFiles.update({one:0})
        # s absolutni cestou
        else:
            one = os.path.abspath(one)
            updateFiles.update({one:0})

        # pokud je soubor prazdny, vrat nulu
        if text == "":
            updateFiles[one] = 0;
        #jinak spust pozadovane hledani
        else:
            # klicova slova
            if arg_k == 1:
                updateFiles[one] = search_keys(lex_parser(text))
            # operatory
            elif arg_o == 1:
                updateFiles[one] = search_operators(lex_parser(text))
            # identifikatory
            elif arg_i == 1:
                updateFiles[one] = search_iden(lex_parser(text))
            # vzor
            elif arg_w == 1:
                updateFiles[one] = search_words(text)
            # pocet znaku komentare
            elif arg_c == 1:
                lex_parser(text)
                updateFiles[one] = count
                if DEBUG: print("POCET COM: ", count)

    if DEBUG: print(updateFiles)
    return updateFiles


def lex_parser(text):
    """ Ocisti text od komentaru, maker a retezcu
        @param vstupni text
        @return ocisteny text """

    global count
    prevChar = ""       # predchozi znak
    line_com = 0        # jednoradkovy komentar     0-neaktivni, 1-aktivni
    is_com = 0          # komentare                 0-neaktivni, 1-aktivni
    string1 = 0         # retezec ""                0-neaktivni, 1-aktivni
    string2 = 0         # retezec ''                0-neaktivni, 1-aktivni
    tmp_text = ""       # ocisteny text
    count = 0           # pocet znaku komentare     celkovy pocet
    macro = 0           # macro                     0-neaktivni, 1-aktivni
    wait = 0            # cekej na rozhodnuti jestli se jedna o komentar //, /*, /\ nebo validni znak /
    wait_star = 0       # cekej na rozhodnuti jestli se jedna o konec komentare */ nebo validni znak /
    two_line_com = 0    # dvouradkovy komentar      0-neaktivni, 1-aktivni(nejasnost je li komentar validni) = /\, 2-aktivni(validni kom.) /\,dalsi radek /
    two_tmp_text = ""   # docasny text ktery se vrati kdyz se nejedna o validni dvouradkovy komentar

    for char in text:
        #if DEBUG: print("Macro je: ", macro, " Count: ", count, " Wait: ", wait, " Char: ", char)

        # zacina macro
        if not string1 and not string2 and not is_com and char == '#':
            if not macro:
                macro = 1

        # ukonceni macra
        if not prevChar == '\\' and char == '\n' or char == '\r\n' and macro:
            macro = 0

        # rozpoznani stringu ""
        if not macro and not string2:
            if char == "\"":
                if string1:
                    # ukonceni
                    string1 = 0
                elif not is_com and not string1:
                    # start
                    string1 = 1

        # rozpoznani stringu ''
        if not macro and not string1:
            if char == "\'":
                if string2:
                    # ukonceni
                    string2 = 0
                elif not is_com and not string2:
                    #start
                    string2 = 1


        # prvni lomitko se nezapocitava do textu dokud
        # neni jiste ze to neni komentar
        if prevChar == '/' and  char != '/' and char != '*' and char != '\\' and not is_com:
            if wait == 1:
                if wait_star == 0:
                    tmp_text += '/'
                    wait = 0
                else:
                    wait = 0

        if prevChar == '*' and char == '/' and wait == 0 and not is_com and not string1 and not string2:
            if wait_star == 0:
                tmp_text += '/'
            else:
                wait_star = 0


        # plati jen pro komentare (u rozsireni i pro macra)
        if not string1 and not string2:
            if is_com:
                count += 1
                #print(char)
                # blokovy komentar
                if not line_com and not two_line_com:
                    if prevChar == '*' and char == '/':
                        is_com = 0
                        wait_star = 1
                        wait = 0
                # jedno-radkovy komentar
                if line_com:
                    if not prevChar == '\\' and char == '\n' or char == '\r\n':
                        line_com = 0
                        is_com = 0
                        wait = 0
                # dvou-radkovy komentar
                if two_line_com:
                    two_tmp_text += char
                    if two_line_com == 1:
                        if char == '\n' or char == '\r\n':
                            two_line_com += 1
                    # prechod na druhy radek
                    elif two_line_com == 2:
                        # je komentar, pokracuj
                        if prevChar == '\n' or prevChar == '\r\n' and char == '/':
                            two_line_com += 1
                        # neni komentar, vrat text zpatky
                        elif prevChar == '\n' or prevChar == '\r\n' and char != '/':
                            tmp_text += "/\\" + two_tmp_text
                            count -= (len(two_tmp_text)+2)
                            is_com = 0
                            two_line_com = 0
                            two_tmp_text = ""
                            wait = 0
                    # konec komentare
                    elif two_line_com > 2:
                        if not prevChar == '\\' and char == '\n' or char == '\r\n':
                            is_com = 0
                            two_line_com = 0
                            two_tmp_text = ""
                            wait = 0
            elif not is_com:
                # zacatek komentare //
                if prevChar == '/' and char == '/':
                    line_com = 1
                    is_com = 1
                    count += 2
                # zacatek komentare /*
                elif prevChar == '/' and  char == '*':
                    line_com = 0
                    is_com = 1
                    count += 2
                # zacatek komentare /\
                elif prevChar == '/' and char == '\\' and not wait_star:
                    line_com = 0
                    is_com = 1
                    two_line_com = 1
                    count += 2

        # ocisteny text od stringu a komentaru
        if not string1 and not string2 and not macro and not is_com:
            if char == '/':
                wait = 1
            if wait == 0 and two_tmp_text == "":
                tmp_text += char

            
        prevChar = char
    if DEBUG: print(count)
    return tmp_text


def search_keys(text):
    """ Vyhleda a vypise pocet nalezenych klicovych slov
        @param vstupni text
        @return pocet klicovych slov """

    global keys
    result = 0

    text = text.split(' ')
    for i in range(0, len(text)):
        for j in range(0, len(keys)):
            if re.search(keys[j], text[i]):
                find = re.findall(keys[j], text[i])
                result += len(find)
                if DEBUG: print("key: ", find)
    if DEBUG: print("KLICOVYCH SLOV JE: ", result)
    return result;

def search_operators(text):
    """ Vyhleda a vypise pocet nalezenych operatoru
        @param vstupni text
        @return pocet operatoru """
        
    operators = (
        "\+\=", "\-\=", "\+\+", "\-\-", "\*\=", "\%\=", "\/\=", "\>\=", "\<\=", "\=\=", "\<\<\=", "\>\>\=", "\!\=", 
        "\|\|", "\&\&", "\<\<", "\>\>", "\&\=", "\|\=", "\^\=", "\-\>", "\!", "\|", "\~", "\&", "\^", "\.[_a-zA-Z\s]", "\=", 
        "\+", "\-", "\*", "\%", "\/", "\<", "\>")
    result = 0
    index = 0
    if DEBUG: print(text)

    # prefiltrovani kvuli operatoru *
    text = search_operators_filter(text)
    if DEBUG: print(text)
    text = text.split(' ')
    for i in range(0, len(text)):
        for j in range(0, len(operators)):
            if re.search(operators[j], text[i]):
                find = re.findall(operators[j], text[i])
                result += len(find)
                text[i] = re.sub(operators[j],' ',text[i])
                index += 1
                if DEBUG: print(index, " op: ", find)
    if DEBUG: print("OPERATORU JE: ", result)
    return result;

def search_operators_filter(text):
    """ Prefiltrovani nepozadovanych operatoru
        @param vstupni text
        @return filtrovany text """

    to_remove = ( "\{", "\:", "\;", "\.\.\." ) # TODO '}', '[', ']'

    for pattern in to_remove:
        text = re.sub(pattern,' ',text)
    text = re.sub(r'(char|const|double|float|int|long|short|sizeof|_Bool|_Complex|,|})+(\s*\(?\s*\*+\s*\(?\w*\)?)+', r' ', text)
    return text

def search_iden(text):
    """ Vyhleda a vypise pocet nalezenych identifikatoru
        @param vstupni text
        @return pocet identifikatoru """

    global keys
    result = 0
    minus = 0
    if DEBUG: print(text)

    text = text.split(' ')
    for i in text:
        if re.search("\\b[_a-zA-Z][_0-9a-zA-Z]*\\b", i):
            find = re.findall("\\b[_a-zA-Z][_0-9a-zA-Z]*\\b", i)
            result += len(find)
            for j in range(0, len(keys)):
                for k in range(0, len(find)):
                    if re.search(keys[j], find[k]):
                        minus += 1
                        continue
            if DEBUG: print("id: ", find)
    if DEBUG: print("IDENTIFIKATORU JE: ", result)
    return result - minus

def search_words(text):
    """ Vyhleda a vypise pocet nalezenych textovych vzoru
        @param vstupni text
        @return pocet textovych vzoru """
    result = len(re.findall(flag_w, text))
    if DEBUG: print("SLOV JE: ", result)
    return result


def print_result(updateFiles):
    """ Vytiskne vysledky
        @param slovník s výsledky """

    global arg_out
    maxLength = 0
    finalString = ""

    # ziskani poctu CELKEM
    sumValue = sum(updateFiles.values())

    # jestlize neni slovnik prazdny
    if updateFiles:
        key_max = 0
        value_max = 0

        # ziskani maximalni delky souboru a hodnoty
        for key, value in updateFiles.items():
            if key_max < len(key):
                key_max = len(key)
            if value_max < len(str(value)):
                value_max = len(str(value))

        if key_max < 7: key_max = 7
        if value_max < len(str(sumValue)): value_max = len(str(sumValue))

        maxLength = key_max + value_max + 1

        # konecne serazeni a dopocitani delek
        for key in sorted(updateFiles.keys()):
            spaces = maxLength - len(key) - len(str(updateFiles[key]))
            spaces *= " "
            finalString += (key + spaces + str(updateFiles[key]) + "\n")
        finalString += ("CELKEM:" + (maxLength-7-len(str(sumValue)))* " " + str(sumValue) + "\n")
    else:
        finalString = ("CELKEM: 0" + "\n")

    # konecne vytisknuti
    if arg_out == 0:
        sys.stdout.write(finalString)
    else:
        try:
            printFile = open(flag_out, 'w', encoding='iso-8859-2')
            printFile.write(finalString)
            printFile.close()
        except:
            prints_msg(3, "")



files = check_args()
updateFiles = open_file(files)
print_result(updateFiles)
sys.exit(0)