#	Projekt IMS - Simulator P/T Petriho siti
#	Autori:	
#		- Jakub Keleceni, xkelec00
#		- Vaclav Bayer, xbayer05
#	December 2015, FIT VUT v Brne
#   -------------------------------------------------
#   -------------------------------------------------
#   subor: Makefile
#   pouzitie:
#	   - "make/make all": prelozenie projektu - vytvorenie suborov
#						  "simulator_kravin", "simulator_ucebna"		
#	   - "make run": spustenie vytvorenych programov s implicitnymi
#					 parametrami + vygenerovanie vystupnych suborov
#					 "vystup_ucebna.txt", "vystup_kravin.txt"
#	   - "make clean": vymazanie vsetkych vytvorenych suborov =>
#					  => spustitelne subory, vystupne subory(statistiky)
# 

CC = g++
OBJ = model hrana miesto prechod kalendar
PROG1 = ucebna 
PROG2 = kravin
PROG3 = vlek
FLAGS = -std=c++11 -Wall -pedantic -Wextra -g

all: $(OBJ) $(PROG1) $(PROG2) $(PROG3)
	$(CC) $(FLAGS) $(OBJ) $(PROG1) -o simulator_ucebna
	$(CC) $(FLAGS) $(OBJ) $(PROG2) -o simulator_kravin
	$(CC) $(FLAGS) $(OBJ) $(PROG3) -o simulator_vlek

model:
	$(CC) $(FLAGS) model.cpp -o model -c

hrana:
	$(CC) $(FLAGS) hrana.cpp -o hrana -c

miesto:
	$(CC) $(FLAGS) miesto.cpp -o miesto -c

prechod:
	$(CC) $(FLAGS) prechod.cpp -o prechod -c

kalendar:
	$(CC) $(FLAGS) kalendar.cpp -o kalendar -c

kravin:
	$(CC) $(FLAGS) kravin.cpp -o kravin -c

ucebna:
	$(CC) $(FLAGS) ucebna.cpp -o ucebna -c	

vlek:
	$(CC) $(FLAGS) vlek.cpp -o vlek -c	
run:
	./simulator_ucebna > vystup_ucebna.txt
	./simulator_kravin > vystup_kravin.txt
	./simulator_vlek > vystup_vlek.txt

clean:
	rm -f $(OBJ) $(PROG1) $(PROG2) $(PROG3) simulator_kravin simulator_ucebna simulator_vlek vystup_kravin.txt vystup_ucebna.txt vystup_vlek.txt