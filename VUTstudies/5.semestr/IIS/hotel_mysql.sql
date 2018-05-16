SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE if exists Zakaznik;
DROP TABLE if exists Zakaznik_sluzba;
DROP TABLE if exists Sluzba;
DROP TABLE if exists Sluzba_platba;
DROP TABLE if exists Platba;
DROP TABLE if exists Pobyt_platba;
DROP TABLE if exists Pobyt;
DROP TABLE if exists Pokoj;
DROP TABLE if exists Zamestnanec;
DROP TABLE if exists Pobyt_rezervace;
DROP TABLE if exists Pobyt_bez_rezervace;
DROP TABLE if exists Zamestnanec_sluzba;
DROP TABLE if exists Pokoj_pobyt;
DROP TABLE if exists Users;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE Zakaznik
(
	username varchar(20) COLLATE utf8_general_ci NOT NULL,
	rodne_cislo bigint(10) NOT NULL,
	jmeno VARCHAR(20) NOT NULL,
	prijmeni VARCHAR(30) NOT NULL,
	ulice VARCHAR (20),
	popisne_cislo int(9),
	mesto VARCHAR (20),
	PSC int(6),
	PRIMARY KEY(rodne_cislo)
) COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Zakaznik_sluzba
(
	ID_sluzby int(2) NOT NULL AUTO_INCREMENT,
	rodne_cislo bigint(10) NOT NULL,
    PRIMARY KEY(ID_sluzby, rodne_cislo)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Sluzba
(
	ID_sluzby int(2) NOT NULL AUTO_INCREMENT,
	cislo_zamestnance int(4),
	typ VARCHAR(30) NOT NULL,	-- nazov sluzby
	cena REAL NOT NULL,
  PRIMARY KEY(ID_sluzby)
) COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';


CREATE TABLE Sluzba_platba
(
	ID_sluzby int(2) NOT NULL,
	ID_platby int(4) NOT NULL,
  PRIMARY KEY(ID_sluzby, ID_platby)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Platba
(
	ID_platby int(4) NOT NULL AUTO_INCREMENT,
	rodne_cislo bigint(10) NOT NULL,
	cislo_zamestnance int(4),
	cena REAL NOT NULL,
	typ_platby VARCHAR(30),
	datum DATE NOT NULL,
	PRIMARY KEY(ID_platby)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Pobyt_platba
(
	ID_pobytu int(4) NOT NULL,
	ID_platby int(4) NOT NULL,
  PRIMARY KEY(ID_pobytu, ID_platby)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Pobyt
(
	ID_pobytu int(4) NOT NULL AUTO_INCREMENT,
	rodne_cislo bigint(10) NOT NULL,
	datum_od DATE NOT NULL,
	datum_do DATE NOT NULL,
	celkova_cena REAL,
	PRIMARY KEY(ID_pobytu)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Pokoj
(
	cislo_pokoje int(4) NOT NULL,
	ID_pobytu int(4),
	cena REAL NOT NULL,
	popis TEXT(500),
	PRIMARY KEY(cislo_pokoje)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Zamestnanec
(
	cislo_zamestnance int(4) NOT NULL AUTO_INCREMENT,
	username VARCHAR(20) NOT NULL,
	jmeno VARCHAR(20) NOT NULL,
	prijmeni VARCHAR(30) NOT NULL,
	PRIMARY KEY(cislo_zamestnance)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Pobyt_rezervace
(
	ID_pobytu int(4),
	cislo_rezervace int(5) NOT NULL
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Pobyt_bez_rezervace
(
	ID_pobytu int(4) NOT NULL,
	cislo_zamestnance int(4) NOT NULL,
	priplatek REAL NOT NULL
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Zamestnanec_sluzba
(
  ID_sluzby int(2) NOT NULL,
  cislo_zamestnance int(4) NOT NULL,
  PRIMARY KEY(ID_sluzby, cislo_zamestnance)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Pokoj_pobyt
(
  cislo_pokoje int(4) NOT NULL,
  ID_pobytu int(4) NOT NULL,
  PRIMARY KEY(cislo_pokoje, ID_pobytu)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

CREATE TABLE Users (
  username varchar(20) COLLATE utf8_general_ci NOT NULL,
  password varchar(100) COLLATE utf8_general_ci NOT NULL,
  role varchar(20) COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (username)
)COMMENT='' ENGINE='InnoDB' COLLATE 'utf8_general_ci';

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
ALTER TABLE Zakaznik ADD CONSTRAINT FK_ZAKAZNIK_username FOREIGN KEY(username) REFERENCES Users(username);
ALTER TABLE Zamestnanec ADD CONSTRAINT FK_ZAMESTNANEC_cislo_username FOREIGN KEY(username) REFERENCES Users(username);

INSERT INTO Users VALUES('test1', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'klient');
INSERT INTO Users VALUES('test2', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'klient');
INSERT INTO Users VALUES('test3', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'klient');
INSERT INTO Users VALUES('test4', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'klient');
INSERT INTO Users VALUES('test5', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'recepce');
INSERT INTO Users VALUES('test6', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'recepce');
INSERT INTO Users VALUES('test7', '$2y$10$UwUy/6/o0.BkF1JP69P89OxAEy.Vc3mSjbsWcIU5Nktr3ngArGVRu', 'manazer');

INSERT INTO Zakaznik VALUES('test1', 9402025644,'Pepa','Vujtek','Ponava',5,'Brno',60200);
INSERT INTO Zakaznik VALUES('test2', 7112122567,'Honza','Velky','Bozetechova',2,'Brno',60200);
INSERT INTO Zakaznik VALUES('test3', 3311112563,'Lojza','Kral','Technicka',12,'Brno',61202);
INSERT INTO Zakaznik VALUES('test4', 5505251235,'Vasek','Patejdl','Brnenska',22,'Olomouc',12477);

INSERT INTO Sluzba VALUES(1,1,'masaz',200);
INSERT INTO Sluzba VALUES(2,1,'sauna',250);
INSERT INTO Sluzba VALUES(3,1,'wellnes',500);
INSERT INTO Sluzba VALUES(4,1,'parkovacie miesto',700);

/* Vyriesit STR_TO_DATE v mysql */
INSERT INTO Pobyt VALUES(1, 9402025644, '2014-2-1', '2014-2-12', 2600);
INSERT INTO Pobyt VALUES(2, 7112122567, '2014-12-1', '2014-12-12', 2300);
INSERT INTO Pobyt VALUES(3, 3311112563, '2014-12-22', '2014-12-23', 1200);
INSERT INTO Pobyt VALUES(4, 5505251235, '2013-12-5', '2013-12-12', 4500);
INSERT INTO Pobyt VALUES(5, 7112122567, '2014-12-1', '2014-12-12', 2500);

INSERT INTO Pokoj VALUES(10, 2, 350, '');
INSERT INTO Pokoj VALUES(33, 1, 500, '');
INSERT INTO Pokoj VALUES(27, 3, 400, '');
INSERT INTO Pokoj VALUES(50, 2, 1000, '');

INSERT INTO Zamestnanec VALUES(1, 'test5', 'Ivan', 'Hezky');
INSERT INTO Zamestnanec VALUES(2, 'test6', 'Eva', 'Pokorna');
INSERT INTO Zamestnanec VALUES(3, 'test7', 'Martin', 'Mrkvicka');

INSERT INTO Platba VALUES(1, 5505251235, 1, 1350, 'hotovost', '2013-12-12');
INSERT INTO Platba VALUES(2, 7112122567, 1, 550, 'kartou', '2014-12-12');
INSERT INTO Platba VALUES(3, 3311112563, NULL, 3340, 'faktura', '2014-12-23');
INSERT INTO Platba VALUES(4, 5505251235, NULL, 1350, 'hotovost', '2013-12-13');

INSERT INTO Pobyt_rezervace VALUES(3, 200124);
INSERT INTO Pobyt_rezervace VALUES(2, 200135);
INSERT INTO Pobyt_rezervace VALUES(1, 200200);
INSERT INTO Pobyt_bez_rezervace VALUES(4, 2, 500);