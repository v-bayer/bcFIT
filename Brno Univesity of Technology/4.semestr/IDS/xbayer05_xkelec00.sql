-- projekt c. 4 IDS 2014/2015
-- xkelec00, xbayer05

set serveroutput on

--------------------------------------------
--			 odstraneni tabulek 		         --
--------------------------------------------
DROP TABLE Zakaznik CASCADE CONSTRAINTS;
DROP TABLE Zakaznik_sluzba CASCADE CONSTRAINTS;
DROP TABLE Sluzba CASCADE CONSTRAINTS;
DROP TABLE Sluzba_platba CASCADE CONSTRAINTS;
DROP TABLE Platba CASCADE CONSTRAINTS;
DROP TABLE Pobyt_platba CASCADE CONSTRAINTS;
DROP TABLE Pobyt CASCADE CONSTRAINTS;
DROP TABLE Pokoj CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec CASCADE CONSTRAINTS;
DROP TABLE Pobyt_rezervace CASCADE CONSTRAINTS;
DROP TABLE Pobyt_bez_rezervace CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec_sluzba CASCADE CONSTRAINTS;
DROP TABLE Pokoj_pobyt CASCADE CONSTRAINTS;

DROP SEQUENCE SEQ;

--------------------------------------------
--       odstraneni prav       --
--------------------------------------------

--revoke all on zakaznik from xbayer05;
--revoke all on pobyt from xbayer05;

-------------------------------------------
--			 vytvoreni tabulek 			         --
-------------------------------------------
CREATE TABLE Zakaznik
(
	rodne_cislo NUMBER(10, 0) NOT NULL,
	jmeno VARCHAR(20) NOT NULL,
	prijmeni VARCHAR(30) NOT NULL,
	ulice VARCHAR (20),
	popisne_cislo INTEGER NOT NULL,
	mesto VARCHAR (20) NOT NULL,
	PSC INTEGER NOT NULL,
	PRIMARY KEY(rodne_cislo)
);

CREATE TABLE Zakaznik_sluzba
(
	ID_sluzby INTEGER NOT NULL,
	rodne_cislo NUMBER(10, 0) NOT NULL,
  PRIMARY KEY(ID_sluzby, rodne_cislo)
);

CREATE TABLE Sluzba
(
	ID_sluzby INTEGER NOT NULL,
	cislo_zamestnance INTEGER,
	typ VARCHAR(30) NOT NULL,	-- nazov sluzby
	cena REAL NOT NULL,
  PRIMARY KEY(ID_sluzby)
);

CREATE TABLE Sluzba_platba
(
	ID_sluzby INTEGER NOT NULL,
	ID_platby INTEGER NOT NULL,
  PRIMARY KEY(ID_sluzby, ID_platby)
);

CREATE TABLE Platba
(
	ID_platby INTEGER NOT NULL,
	rodne_cislo NUMBER(10, 0) NOT NULL,
	cislo_zamestnance INTEGER,
	cena REAL NOT NULL,
	typ_platby VARCHAR(30),
	datum DATE NOT NULL,
	PRIMARY KEY(ID_platby)
);

CREATE TABLE Pobyt_platba
(
	ID_pobytu INTEGER NOT NULL,
	ID_platby INTEGER NOT NULL,
  PRIMARY KEY(ID_pobytu, ID_platby)
);

CREATE TABLE Pobyt
(
	ID_pobytu INTEGER NOT NULL,
	rodne_cislo NUMBER(10, 0) NOT NULL,
	datum_od DATE NOT NULL,
	datum_do DATE NOT NULL,
	celkova_cena REAL,
	PRIMARY KEY(ID_pobytu)
);

CREATE TABLE Pokoj
(
	cislo_pokoje INTEGER NOT NULL,
	ID_pobytu INTEGER,
	--typ_pokoje VARCHAR(25) NOT NULL,
	cena REAL NOT NULL,
	PRIMARY KEY(cislo_pokoje)
);

CREATE TABLE Zamestnanec
(
	cislo_zamestnance INTEGER,
	jmeno VARCHAR(20) NOT NULL,
	prijmeni VARCHAR(30) NOT NULL,
	PRIMARY KEY(cislo_zamestnance)
);

CREATE TABLE Pobyt_rezervace
(
	ID_pobytu INTEGER,
	cislo_rezervace INTEGER NOT NULL
);

CREATE TABLE Pobyt_bez_rezervace
(
	ID_pobytu INTEGER NOT NULL,
	cislo_zamestnance INTEGER NOT NULL,
	priplatek REAL NOT NULL
);

CREATE TABLE Zamestnanec_sluzba
(
  ID_sluzby INTEGER NOT NULL,
  cislo_zamestnance INTEGER NOT NULL,
  PRIMARY KEY(ID_sluzby, cislo_zamestnance)
);

CREATE TABLE Pokoj_pobyt
(
  cislo_pokoje INTEGER NOT NULL,
  ID_pobytu INTEGER NOT NULL,
  PRIMARY KEY(cislo_pokoje, ID_pobytu)
);

-------------------------------------------
--		  nastaveni cizich klicu 	 	       --
-------------------------------------------
ALTER TABLE Zakaznik_sluzba ADD CONSTRAINT FK_ZS_rodne_cislo FOREIGN KEY(rodne_cislo) REFERENCES Zakaznik(rodne_cislo);
ALTER TABLE Zakaznik_sluzba ADD CONSTRAINT FK_ZS_ID_sluzby FOREIGN KEY(ID_sluzby) REFERENCES Sluzba(ID_sluzby);
ALTER TABLE Sluzba_platba ADD CONSTRAINT FK_SP_ID_sluzby FOREIGN KEY(ID_sluzby) REFERENCES Sluzba(ID_sluzby);
ALTER TABLE Sluzba_platba ADD CONSTRAINT FK_SP_ID_platby FOREIGN KEY(ID_platby) REFERENCES Platba(ID_platby);
ALTER TABLE Pobyt_platba ADD CONSTRAINT FK_PP_ID_platby FOREIGN KEY(ID_platby) REFERENCES Platba(ID_platby);
ALTER TABLE Pobyt_platba ADD CONSTRAINT FK_PP_ID_pobytu FOREIGN KEY(ID_pobytu) REFERENCES Pobyt(ID_pobytu);
ALTER TABLE Pobyt ADD CONSTRAINT FK_POBYT_rodne_cislo FOREIGN KEY(rodne_cislo) REFERENCES Zakaznik(rodne_cislo);
ALTER TABLE Pobyt ADD CONSTRAINT chk_datum_od_do CHECK(datum_od <= datum_do);
ALTER TABLE Pobyt_rezervace ADD CONSTRAINT FK_rezervace_ID_pobytu FOREIGN KEY(ID_pobytu) REFERENCES Pobyt(ID_pobytu);
ALTER TABLE Pobyt_bez_rezervace ADD CONSTRAINT FK_bez_rezervace_ID_pobytu FOREIGN KEY(ID_pobytu) REFERENCES Pobyt(ID_pobytu);
ALTER TABLE Pobyt_bez_rezervace ADD CONSTRAINT FK_bez_rezervace_Czamestnance FOREIGN KEY(cislo_zamestnance) REFERENCES Zamestnanec(cislo_zamestnance);
ALTER TABLE Platba ADD CONSTRAINT FK_PLATBA_rodne_cislo FOREIGN KEY(rodne_cislo) REFERENCES Zakaznik(rodne_cislo);
ALTER TABLE Platba ADD CONSTRAINT FK_PLATBA_cislo_zamestnanca FOREIGN KEY(cislo_zamestnance) REFERENCES Zamestnanec(cislo_zamestnance);

-------------------------------------------
--		  trigger - rodne cislo		 	       --
-------------------------------------------
ALTER session SET nls_date_format='dd.mm.yy';

SET serveroutput ON;

CREATE OR REPLACE TRIGGER tr_rodc
    BEFORE INSERT OR UPDATE OF rodne_cislo ON Zakaznik
    FOR EACH ROW
  DECLARE
    rod_c Zakaznik.rodne_cislo%TYPE;
    den NUMBER(2);
    mesic NUMBER(2);
    rok NUMBER(2);
    datum DATE;
  BEGIN
  	-- rozdeleni na casti
    rod_c := :NEW.rodne_cislo;
    mesic := MOD( (rod_c / 1000000), 100);
    den   := MOD( (rod_c / 10000), 100);
    rok   := rod_c / 100000000;
  
  	-- kontrola delitelnosti 11ti
    IF ( MOD(rod_c, 11) <> 0 ) THEN
  	 Raise_Application_Error (-1, 'Neplatne rodne cislo: Neni delitelne 11');
    END IF;
  
    IF ( mesic > 50 ) THEN
  	 mesic := mesic - 50;
    END IF;
  
    BEGIN
      -- je platne datum?
  	  datum := den||'.'||mesic||'.'||rok;
    EXCEPTION
  	  WHEN OTHERS THEN
  	    Raise_Application_Error (-2, 'Neplatne datum v rodnem cisle');
    END;
  END tr_rodc;
  /

-------------------------------------------
--		trigger - autoinkrement 		       --
-------------------------------------------
CREATE SEQUENCE SEQ MINVALUE 1 START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

CREATE OR REPLACE TRIGGER AUTONUMBER
BEFORE INSERT ON Zamestnanec
FOR EACH ROW
BEGIN
  SELECT SEQ.NEXTVAL
  INTO :NEW.cislo_zamestnance FROM DUAL;
END;
/

-------------------------------------------
--		naplneni tabulek hodnotami 		     --
-------------------------------------------
INSERT INTO Zakaznik VALUES(9402025644,'Pepa','Vujtek','Ponava',5,'Brno',60200);
INSERT INTO Zakaznik VALUES(7112122567,'Honza','Velky','Bozetechova',2,'Brno',60200);
INSERT INTO Zakaznik VALUES(3311112563,'Lojza','Kral','Technicka',12,'Brno',61202);
INSERT INTO Zakaznik VALUES(5505251235,'Vasek','Patejdl','Brnenska',22,'Olomouc',124777);

INSERT INTO Sluzba VALUES(1,1,'masaz',200);
INSERT INTO Sluzba VALUES(2,1,'sauna',250);
INSERT INTO Sluzba VALUES(3,1,'wellnes',500);
INSERT INTO Sluzba VALUES(4,1,'parkovacie miesto',700);

INSERT INTO Pobyt VALUES(1, 9402025644, TO_DATE('1-2-2014', 'dd-mm-yyyy'), TO_DATE('12-2-2014', 'dd-mm-yyyy'), 2600);
INSERT INTO Pobyt VALUES(2, 7112122567, TO_DATE('1-12-2014', 'dd-mm-yyyy'), TO_DATE('12-12-2014', 'dd-mm-yyyy'), 2300);
INSERT INTO Pobyt VALUES(3, 3311112563, TO_DATE('22-12-2014', 'dd-mm-yyyy'), TO_DATE('23-12-2014', 'dd-mm-yyyy'), 1200);
INSERT INTO Pobyt VALUES(4, 5505251235, TO_DATE('5-12-2013', 'dd-mm-yyyy'), TO_DATE('12-12-2013', 'dd-mm-yyyy'), 4500);

INSERT INTO Pokoj VALUES(10, 2, 350);
INSERT INTO Pokoj VALUES(33, 1, 500);
INSERT INTO Pokoj VALUES(27, 3, 400);
INSERT INTO Pokoj VALUES(50, 2, 1000);

INSERT INTO Zamestnanec VALUES(null, 'Ivan', 'Hezky');
INSERT INTO Zamestnanec VALUES(null, 'Eva', 'Pokorna');
INSERT INTO Zamestnanec VALUES(null, 'Martin', 'Mrkvicka');

INSERT INTO Platba VALUES(1, 5505251235, 1, 1350, 'hotovost', TO_DATE('12-12-2013', 'dd-mm-yyyy'));
INSERT INTO Platba VALUES(2, 7112122567, 1, 550, 'kartou', TO_DATE('12-12-2014', 'dd-mm-yyyy'));
INSERT INTO Platba VALUES(3, 3311112563, NULL, 3340, 'faktura', TO_DATE('23-12-2014', 'dd-mm-yyyy'));
INSERT INTO Platba VALUES(4, 5505251235, NULL, 1350, 'hotovost', TO_DATE('13-12-2013', 'dd-mm-yyyy'));

INSERT INTO Pobyt_rezervace VALUES(3, 200124);
INSERT INTO Pobyt_rezervace VALUES(2, 200135);
INSERT INTO Pobyt_rezervace VALUES(1, 200200);

INSERT INTO Pobyt_bez_rezervace VALUES(4, 2, 500);

SELECT * FROM Zakaznik;
SELECT * FROM Sluzba;
SELECT * FROM Platba;
SELECT * FROM Pobyt;
SELECT * FROM Pobyt_rezervace;
SELECT * FROM Pobyt_bez_rezervace;
SELECT * FROM Zamestnanec;


--------------------------------------------
--              SELECTY                   --
--------------------------------------------
-- Vyber vsechny pobyty bez registrace, ktere vlozil zamestnanec
-- a ktere maji priplatek vetsi jak 200
SELECT P.*
FROM Zamestnanec Z,Pobyt_bez_rezervace P
WHERE Z.cislo_zamestnance=P.cislo_zamestnance AND P.priplatek>200;

-- Vyber zamestnance, kteri uskutecnili platbu po datu 12-12-2013
SELECT Z.*
FROM Platba P,Zamestnanec Z
WHERE P.cislo_zamestnance=Z.cislo_zamestnance AND P.datum>'12-12-2013';

-- Vyber zakazniky, kteri maji rezervaci a jsou z mesta 'Brno'
SELECT jmeno, prijmeni, cislo_rezervace
FROM Pobyt 
NATURAL JOIN Pobyt_rezervace 
NATURAL JOIN Zakaznik
WHERE mesto='Brno';

-- Vyber platby ktere zadal zamestnanec a vypis pocet
-- jednotlivych typu
SELECT typ_platby,COUNT(*)
FROM Platba
WHERE cislo_zamestnance IS NOT NULL
GROUP BY typ_platby;

-- Vyber platby s vetsi cenou nez 200 a vypis pocet
-- jednotlivych typu
SELECT typ_platby,COUNT(*)
FROM Platba
WHERE cena>200
GROUP BY typ_platby;

-- Vyber ID platby, cenu a rodne cislo z plateb,
-- ktere nevkladal zamestnanec
SELECT ID_platby,cena,rodne_cislo
FROM Platba
WHERE NOT EXISTS (
	SELECT jmeno
	FROM zamestnanec
	WHERE Platba.cislo_zamestnance=zamestnanec.cislo_zamestnance
);

-- Vyber zamestnance, ktery vytvoril ubytovani pro
-- klienta od 4-12-2013 do 23-12-2014
SELECT *
FROM Zamestnanec
WHERE cislo_zamestnance IN(
	SELECT cislo_zamestnance
	FROM Pobyt_bez_rezervace
	WHERE ID_pobytu IN(
		SELECT ID_pobytu
		FROM Pobyt
		WHERE datum_od BETWEEN '4-12-2013' AND '23-12-2014'
		));


--------------------------------------------
--    procedura - maximalni utrata        --
--  nalezne zakaznika, ktery v hotelu     --
--  minul dohromady nejvice penez         --
--------------------------------------------
create or replace procedure platbaMax(out_rodne_cislo out zakaznik.rodne_cislo%type) is
  cursor zakaznik_cur  is select rodne_cislo from Zakaznik;
  cursor cena_cur is select * from Platba;
  platbaRow Platba%rowtype;
  suma number;
  tmpNum number;
  maxSuma number;
  aktualny_zakaznik Zakaznik.rodne_cislo%type;
  najviacUtratil zakaznik.rodne_cislo%type;
  zakaznik_vystup zakaznik%rowtype;
begin
  open zakaznik_cur;
  maxSuma := 0;
  tmpNum := 0;
  <<outer_loop>>
  loop
    fetch zakaznik_cur into aktualny_zakaznik;
    exit when zakaznik_cur%notfound;
    suma := 0;
    open cena_cur;
    <<inner_loop>>
    loop
      fetch cena_cur into platbarow;
      exit when cena_cur%notfound;
      if aktualny_zakaznik = platbarow.rodne_cislo then
        suma := suma + platbarow.cena;
      end if;
    end loop inner_loop;
    
    if maxSuma < suma then
      maxSuma := suma;
      najviacUtratil := aktualny_zakaznik;
    end if;
    close cena_cur;
  end loop outer_loop;
  select * into zakaznik_vystup from Zakaznik where najviacUtratil = zakaznik.rodne_cislo;
  out_rodne_cislo := najviacUtratil;
  --dbms_output.put_line(zakaznik_vystup.jmeno || ' ' || zakaznik_vystup.prijmeni || ' ' || zakaznik_vystup.rodne_cislo || ' ' || maxSuma);
  close zakaznik_cur;
end;
/

--------------------------------------------
--       procedura - nejdelsi pobyt       --
--  nalezne nejdelsi pobyt zvoleneho      --
--  zakaznika.                            --
--------------------------------------------
create or replace procedure nejdelsi_pobyt(rod_c in zakaznik.rodne_cislo%type, idPobytu out number, maxDelka out number)
is
  tmpDelka number;
  --cenaPobytu number;
  pobytRow Pobyt%rowtype;
  zakaznikRow zakaznik%rowtype;
  cursor datum is select * from Pobyt;  
begin
  select * into zakaznikRow from Zakaznik where rod_c = rodne_cislo; -- vyvola vynimku ak zakaznik neexistuje
  maxDelka := 0;
  open datum;
  loop
    fetch datum into pobytRow;
    exit when datum%notfound;
    if rod_c = pobytRow.rodne_cislo then
      tmpDelka := (pobytRow.datum_do - pobytRow.datum_od);
      if tmpDelka > maxDelka then
        maxDelka := tmpDelka;
        idPobytu := pobytRow.ID_pobytu;        
        --cenaPobytu := pobytRow.celkova_cena;
      end if;
    end if;
  end loop;
  --dbms_output.put_line(zakaznikRow.prijmeni || ' ' || idPobytu || ' ' || maxDelka || ' ' || cenaPobytu);
  close datum;  

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        Raise_Application_Error(-20001, 'Neexistujici zakaznik');
end;
/

-------------------------------------------
--    zavolani explain plan bez indexu   --
-------------------------------------------
explain plan for select rodne_cislo, count(*) from Zakaznik natural join Platba where cena > 1000 group by rodne_cislo;
select plan_table_output from table(dbms_xplan.display());

-------------------------------------------
--    zavolani explain plan s indexem    --
-------------------------------------------
create index platba_cena on Platba(cena); 
explain plan for select rodne_cislo, count(*) from Zakaznik natural join Platba where cena > 1000 group by rodne_cislo;
select plan_table_output from table(dbms_xplan.display());
drop index platba_cena;

--------------------------------------------
--       nastaveni prav clenu tymu        --
--------------------------------------------
GRANT ALL ON Zakaznik to xbayer05;
GRANT ALL ON Pobyt to xbayer05;

-----------------------------------------------------------------------------
-------------- MATERIALIZOVANY POHLED VYTVARI DRUHY CLEN TYMU ---------------
/*
drop materialized view my_view;
drop synonym zakaznik;
drop synonym pobyt;

create or replace synonym zakaznik for xkelec00.zakaznik@DBgort;
create or replace synonym pobyt for xkelec00.pobyt@DBgort;

create materialized view my_view
  nologging
  cache
  build immediate 
  refresh on commit
  enable query rewrite
  as
    select rodne_cislo, prijmeni, jmeno, mesto, pobyt.celkova_cena
    from zakaznik
    natural join pobyt;

alter session set query_rewrite_enabled = false;
explain plan for select rodne_cislo, prijmeni, jmeno, mesto, pobyt.celkova_cena
from zakaznik natural join pobyt;
select plan_table_output from table(dbms_xplan.display(null, null, 'basic'));

alter session set query_rewrite_enabled = true;
explain plan for select rodne_cislo, prijmeni, jmeno, mesto, pobyt.celkova_cena
from zakaznik natural join pobyt;
select plan_table_output from table(dbms_xplan.display(null, null, 'basic'));

grant all on my_view to xkelec00
*/

-----------------------------------------------
-- priklad pouzitia procedury platbamax --
-----------------------------------------------
declare
  x zakaznik.rodne_cislo%type;
begin
  platbamax(x);
  dbms_output.put_line(x);
end;
/

-----------------------------------------------
-- priklad pouzitia procedury nejdelsi_pobyt --
-----------------------------------------------
declare
  y zakaznik.rodne_cislo%type;
  cena number;
  idPobytu number;
begin
  y := '9402025644';
  nejdelsi_pobyt(y, idPobytu, cena);
  dbms_output.put_line(idPobytu || ' ' || cena);
end;
/