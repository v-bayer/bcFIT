/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: scaner.h 
*Autor: Tomas Furch, xfurch01
*/

//hlavicka pro lexikalni analyzator

#define ID      	    	100  // identifikator
#define INT   		    	101  // integer
#define REAL		    	102  // real
#define ASSIGN  	    	103  // prirazeni
#define SEMICOLON 	    	104  // strednik
#define EQUAL		    	105  // revnitko
#define COLON		    	106  // dvojtecka
#define END_OF_FILE     	108  // konec souboru
#define DOT			    	109  // tecka
#define LEFT_BRACKET    	110  // leva zavorka
#define RIGHT_BRACKET   	111  // prava zavorka
#define NOT_EQUAL			112  // nerovna se 
#define LESS				113  // mensi nez
#define GREATER				114  // vetsi nez
#define LESS_OR_EQUAL		115  // mensi nebo rovno
#define GREATER_OR_EQUAL	116  // vetsi nebo rovno
#define STRING				117  // retezec
#define COMMA				118  // carka
#define PLUS				119  // scitani
#define MINUS				120  // odcitani
#define MULTIPLY			121  // nasobeni
#define DIVIDE				122  // deleni

	
//chybove hlasky
#define LEX_ERROR     1

//hlavicka funkce simulujici lexikalni analyzator
void setSourceFile(FILE *f);
int getToken(string *attr);
int isOperator(int c);
