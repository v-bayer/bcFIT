/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: scaner.c 
*Autor: Tomas Furch, xfurch01
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "str.h"
#include "scaner.h"

extern int lineNum;
FILE *source;

void setSourceFile(FILE *f) {
 	source = f;
}

int isOperator(int c){
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == ';' || c == '.' || c == '(' 
		|| c == ')' || c == '=' || c == '<' || c == '>' || c == ':' || c == ',' || c == '\n') return 1;
	else return 0;
}


int getToken(string *attr) {
	int state = 0; // stav konecneho automatu
	int c; // promenna pro ulozeni nacteneho znaku
	int i; // pocitadle pro seqEscape
	char seqEscape[4]; // promenna pro ulozeni escape sequence
	int pref = 1; // prefixove 0 v escape sekvenci
	
 	
 	// vymazeme obsah atributu a v pripade identifikatoru
   	// budeme postupne do nej vkladat jeho nazev
   	strClear(attr);
	
	while (1){
		c = getc(source);
		
		switch (state){
			case 0: if (isalpha(c) || c == '_') { // idenfikator
						c = tolower(c);
						strAddChar(attr, c);
						state = 1;
                    } else if (isdigit(c)) {      // integer nebo real                 
                        strAddChar(attr, c);
                        state = 2;
                    } else if (c == ':') {        // dvojtecka nebo prirazeni
                         state = 11;
                    } else if (c == '<') {        // dvojtecka nebo prirazeni
                         state = 12;
                    } else if (c == '>') {        // dvojtecka nebo prirazeni
                         state = 13;
                    } else if (c == '{') {        // komentar 
                        state = 10;
                    } else if (c == '+') { 			// scitani
                    	return PLUS;
                    } else if (c == '-') { 			// odcitani
                    	return MINUS;
                    } else if (c == '*') { 			// nasobeni
                    	return MULTIPLY;
                    } else if (c == '/') { 			// deleni
                    	return DIVIDE;
                    } else if (c == '\'') { 		//retezcovy literal
                    	state = 14;
                    } else if (c == ';'){			// strednik
                    	return SEMICOLON;
                    } else if (c == ','){			// carka
                    	return COMMA;
                    }else if (c == '('){			// leva zavorka
                    	return LEFT_BRACKET;
                    } else if (c == ')'){			// prava zavorka
                    	return RIGHT_BRACKET;
                    } else if (c == '='){			// rovnitko
                    	return EQUAL;
                    } else if (c == '.'){			// tecka
                    	return DOT;
                    } else if (c == '\n') {			// prechod na novy radek
                    	lineNum++;
                    } else if (c == EOF) {			// konec souboru
                    	return END_OF_FILE;
                    }else if (!isspace(c)) {		// neni ani whitespace - lexikalni chyba 
                    	strAddChar(attr, c);
                    	return LEX_ERROR;
                    }
					break;

			case 1: if (isalnum(c) || c == '_') { // idenfikator nebo klicove slovo
						c = tolower(c);
						strAddChar(attr, c);
					} else if (isOperator(c)){ // pri nacteni operatoru konci identifikator a operator se vraci do vstupniho proudu
						ungetc(c, source);
						state = 0;
                        return ID;	
					} else if (isspace(c)) { // pokud white-space konci identifikator a prechazi se do pocatecniho stavu
						state = 0;
						return ID;
					} else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					}
					break;

			case 2: if (isdigit(c)) { // integer
						strAddChar(attr, c); 
					} else if (c == '.') { // pri nacteni '.' -> real (17 je kontrola po tecce)
						strAddChar(attr, c);
						state = 17;
					} else if (c == 'e' || c == 'E') {	// pri nacteni 'e', 'E' -> real
						strAddChar(attr, c);			
						state = 4;
					} else if (isspace(c)) { // pokud white space, pak konec INT a prehod do pocatecniho stavu
						state = 0;
						return INT;
					} else if (isOperator(c)) { // pokud operand, pak konec INT a prechod do pocatecniho stavu
						ungetc(c, source);
						state = 0;
						return INT;
					} else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					}
					break;

			case 3: if (isdigit(c)){ // real
                        strAddChar(attr, c);
                    } else if (c == 'e' || c == 'E') { // pokud e nebo E pak prechod do stavu 4
                    	strAddChar(attr, c);
						state = 4;
					} else if (isOperator(c)) {
						ungetc(c, source);
						state = 0;
						return REAL;
					} else if (isspace(c)){
						state = 0;
						return REAL;
					} else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					}
                    break;
                     
            case 4: if (isdigit(c)) { // pokud za E je cislo, pak prechod od 5
            			strAddChar(attr, c);
            			state = 5;
            		} else if (c == '+' || c == '-') { // pokud za E nasleduje volitelny operator + nebo -, pak prechod do 6		
            			strAddChar(attr, c);
            			state = 6;
                    } else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					}         		 
            		break;
            	
            case 5: if (isdigit(c)){ // exponent bez znamenka
            			strAddChar(attr, c);
            		} else if (isOperator(c)) { // pokud operand, je vracen zpet a prechazi se do pocatecniho stavu
						ungetc(c, source);
						state = 0;
						return REAL;
					} else if (isspace(c)){ // pokud white space, pak konec REAL a prechod do pocatecniho stavu
						state = 0;
						return REAL;
					} else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					}
            		break;
            		
            case 6: if (isdigit(c)) { // pokud za E nasleduje znamenku, musi byt dalsi znak cislice, dale stejna pravidla jako ve stavu 5
            			strAddChar(attr, c);
            			state = 5;
            		} else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					}
            		break;
            		
            case 10: if (c == '}') state = 0;
            		 if (c == EOF) {
            		 	return LEX_ERROR;
                     }
                     break; 

			case 11: if (c == '=') { // prirazeni
						state = 0;
						return ASSIGN;
					} else { // dvojtecka
						ungetc(c, source);
						state = 0;
						return COLON;
					}
					break;
					
			case 12: if (c == '=') { // mensi nebo rovno
						state = 0;
						return LESS_OR_EQUAL;
					} else if (c == '>') { 
						state = 0;
						return NOT_EQUAL;
					} else {
						ungetc(c, source);
						state = 0;
						return LESS;
					}
					break;
					
			case 13: if (c == '=') { // vetsi nebo rovno
						state = 0;
						return GREATER_OR_EQUAL;
					} else { // vetsi
						ungetc(c, source);
						state = 0;
						return GREATER;
					}
					break;
					
			case 14: if (c == '\'') {
					 	state = 15;
					 } else if (c <= 31) { // konec radku (neukonceny retezec) => lexiklani chyba 
					 	return LEX_ERROR;
					 } else {
					 	strAddChar(attr, c);
					 }
					 break;
			
			case 15: if (c == '\'') { // vlozeni apostrofu do retezce, navrat od stavu 14 
					 	strAddChar(attr, c);
					 	state = 14;
					 } else if (c == '#') { // vlozeni escape sekvence 
					 	i = 0; // nastaveni pocitadla pole seqEscape
					 	pref = 1; // nastaveni prefixu pro escape sekvence zacinajiho nulama
					 	state = 16;
					 } else { // jinak konec retezcoveho literalu
					 	ungetc(c, source);
					 	state = 0;
					 	return STRING;
					 }
					 break;
					 
			case 16: if (c == '\'') { // konec escape sekvence
						seqEscape[i] = '\0'; // pridani ukoncovaci nuly
						c = atoi(seqEscape); 
						if (c < 1 || c > 255){
							return LEX_ERROR;
						}
						strAddChar(attr, c);
					 	state = 14;
					 } else if (isdigit(c)) {
					 	if (!(c == '0' && pref == 1)){
					 		pref = 0;
					 		seqEscape[i++] = c;
					 		//printf("%s \n", seqEscape);
					 	} 
					 	if (i > 3){ //vice nez tri mistne cislo -> urcite > 255
					 		return LEX_ERROR;
					 	}
					 } else {
						strAddChar(attr, c);
						return LEX_ERROR; // jinak lexikalni chyba
					 }
					 break;
			
			case 17: if (isdigit(c)){ // po tecce v real
                        strAddChar(attr, c);
                        state = 3;
                    } else { // pokud po tecce nenasleduje cislo -> LEX_ERROR
                    	return LEX_ERROR;
					}
					break;

		} // end of switch
	} // end of while
}

/***** END OF SCANER.C *****/
