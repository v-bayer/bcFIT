Spustenim tohoto programu vytvorite server typu SSH/FTP, pro sber prihlasovacich informaci uzivatelu(klientu).

Pouziti:
Povinne parametry:
	-m 		rezim "ssh" nebo "ftp"
	-a 		IP adresa
	-p 		cislo portu
	-l 		logovaci soubor
Nepovinne parametry:
	-r 		RSA klic - povinny pouze v SSH rezimu
	-c 		max. pocet klientu
	-t 		max. pocet pokusu zadani hesla - voli se jen v SSH rezimu

Priklad pouziti:
./fakesrv -m ftp -a fe80::21e:67ff:ab1a:b1ff -p 21 -l ftp.log -c 10
./fakesrv -m ssh -a 1.2.3.4 -p 22 -r ~/my_rsa.key -l ssh.log -t 5 -c 5


Skript test.sh:
Tento skript slouzi k hromadnemu spusteni serveru a navoleneho poctu klientu.
Parametry s argumenty zadavejte stejne jako pri spusteni serveru dle pravidel daneho modu + pripojte parametr "-n" s ciselnou hodnotou vyjadrujici pocet pripojujicich se klientu.

Priklad pouziti:
bash ./test.sh -m ftp -a fe80::21e:67ff:ab1a:b1ff -p 21 -l ftp.log -c 10 -n 5


Seznam souboru:
-fakesrv.cpp
-fakesrv.h
-test.sh
-Makefile
-manual.pdf
-README.txt

