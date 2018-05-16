	README
	autor: Vaclav Bayer, xbayer05
	IPK Projekt c.2, Komunikace server/klient HTTP protokol

Aplikace funguje jako komunikace mezi serverem a klientem, kde server
zasila klientovi na vyzadani data pomoci protokolu HTTP 1.1.

Argumenty jsou spracovavany pomoci regularnich vyrazu.
Komunikaci zahajuje klient zaslanim zpravy serveru.
Veskere potrebne informace k pripojeni si klient ziska
pomoci knihovny z URI ktery dostane na vstupu.

Server spracovava pozadavek klienta. Dojde-li k tomu, ze server
nedokaze cist ze souboru, ktey pozaduje klient, vraci chybove hlaseni
s kodem 404. Ukonceni komunikace je reseno zaslanim informace v podobe 
dodatku v hlavicce "Connection: close" ze strany serveru klientovi
(nebylo mi moc jasne jak to resit jinak). 

Spusteni serveru:
-c=[0-9], --chunk-max-size=[0-9].......urci max. velikost dat v chunku
-p=[0-9], --port=[0-9].................port ke komunikaci s klientem
-t=[0-9], --min-chunk-time=[0-9].......minimalni cas cekani pred odeslanim
                                       dalsiho chunku

Spusteni klienta:
-h..........vytiskne napovedu
URI.........format rfc3986