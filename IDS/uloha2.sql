/*authors: xvisku01, xvenge01*/

DROP TABLE kaviaren CASCADE CONSTRAINTS;
DROP TABLE kava CASCADE CONSTRAINTS;
DROP TABLE zrno CASCADE CONSTRAINTS;
DROP TABLE pozozstava_z CASCADE CONSTRAINTS;
DROP TABLE ponuka CASCADE CONSTRAINTS;
DROP TABLE uzivatel CASCADE CONSTRAINTS;
DROP TABLE zamestnanec CASCADE CONSTRAINTS;
DROP TABLE pracuje CASCADE CONSTRAINTS;
DROP TABLE dodatkove_sluzby CASCADE CONSTRAINTS;
DROP TABLE CUPPING_AKCIA CASCADE CONSTRAINTS;
DROP TABLE RECENZIA_AKCIE CASCADE CONSTRAINTS;
DROP TABLE RECENZIA_KAVIAREN CASCADE CONSTRAINTS;

CREATE OR REPLACE TYPE hodiny IS VARRAY(7) OF DATE;

CREATE TABLE kaviaren
(
    ID_kaviarne NUMBER(8) PRIMARY KEY NOT NULL,
    nazov       VARCHAR(100)          NOT NULL,
    capacity    NUMBER DEFAULT 0,
    otvaracie   hodiny,
    zatvaracie  hodiny
);

CREATE TABLE dodatkove_sluzby
(
    ID_kaviarne          INT NOT NULL,
    cukraren             CHAR(1), /*Y-ano, N- nie*/
    wifi                 CHAR(1), /*Y-ano, N- nie*/
    bezbarierovy_pristup CHAR(1), /*Y-ano, N- nie*/
    FOREIGN KEY (ID_kaviarne) REFERENCES kaviaren (ID_kaviarne) ON DELETE CASCADE
);

CREATE TABLE kava
(
    ID_kavy         INT PRIMARY KEY NOT NULL,
    nazov           VARCHAR(100)    NOT NULL,
    stupen_horkosti NUMBER DEFAULT 0,
    aroma           VARCHAR(100),
    chut            VARCHAR(100)
);

CREATE TABLE zrno
(
    ID_zrna INT PRIMARY KEY NOT NULL,
    oblast  VARCHAR(100),
    odroda  VARCHAR(100) NOT NULL
);

CREATE TABLE pozozstava_z
(
    ID_zrna INT NOT NULL,
    ID_kavy INT NOT NULL,
    FOREIGN KEY (ID_zrna) REFERENCES zrno (ID_zrna) ON DELETE CASCADE,
    FOREIGN KEY (ID_kavy) REFERENCES kava (ID_kavy) ON DELETE CASCADE
);

CREATE TABLE ponuka
(
    ID_kaviarne INT NOT NULL,
    ID_kavy     INT NOT NULL,
    FOREIGN KEY (ID_kaviarne) REFERENCES kaviaren (ID_kaviarne) ON DELETE CASCADE,
    FOREIGN KEY (ID_kavy) REFERENCES kava (ID_kavy) ON DELETE CASCADE
);

CREATE TABLE uzivatel
(
    ID_osoby                  INT PRIMARY KEY NOT NULL,
    krstne_meno               VARCHAR(100) NOT NULL,
    priezvisko                VARCHAR(100) NOT NULL,
    mail                      VARCHAR(100) NOT NULL,
    oblubeny_sposob_prip_kavy VARCHAR(100),
    pocet_kav_denne           NUMBER,
    nick                      VARCHAR(100),
    ID_KAVA REFERENCES kava
);

CREATE TABLE zamestnanec
(
    ID_osoby    INT PRIMARY KEY NOT NULL,
    ICO         NUMBER(8),
    krstne_meno VARCHAR(100) NOT NULL,
    priezvisko  VARCHAR(100) NOT NULL,
    mail        VARCHAR(100) NOT NULL,
    telefon     VARCHAR(14),
    je_majitel  CHAR(1), /*Y-ano je majitel, N- nie je majitel*/
    CHECK (MOD(
                   MOD(11 -
                       8 * SUBSTR(ICO, 1, 1) +
                       7 * SUBSTR(ICO, 2, 1) +
                       6 * SUBSTR(ICO, 3, 1) +
                       5 * SUBSTR(ICO, 4, 1) +
                       4 * SUBSTR(ICO, 5, 1) +
                       3 * SUBSTR(ICO, 6, 1) +
                       2 * SUBSTR(ICO, 7, 1),
                       11), 10) = SUBSTR(ICO, 8, 1))

);

CREATE TABLE pracuje
(
    ID_kaviarne INT NOT NULL,
    ID_osoby    INT NOT NULL,
    FOREIGN KEY (ID_kaviarne) REFERENCES kaviaren (ID_kaviarne) ON DELETE CASCADE,
    FOREIGN KEY (ID_osoby) REFERENCES zamestnanec (ID_osoby) ON DELETE CASCADE
);

CREATE TABLE cupping_akcia
(
    ID_cupping  NUMBER PRIMARY KEY NOT NULL,
    datum       DATE   NOT NULL,
    nazov_akcie VARCHAR2(150) NOT NULL,
    kapacita    NUMBER DEFAULT 0,
    ID_kaviarne INT    NOT NULL,
    FOREIGN KEY (ID_kaviarne) REFERENCES kaviaren(ID_kaviarne) ON DELETE CASCADE
)
;

CREATE TABLE recenzia_kaviaren
(
    ID_recenzia_kaviaren NUMBER PRIMARY KEY NOT NULL,
    datum                DATE   NOT NULL,
    hodnotenie           NUMBER(1) DEFAULT 0 NOT NULL,
    recenzia_mes         VARCHAR2(250),
    atmosfera            NUMBER(1) DEFAULT 0,
    pristup              NUMBER(1) DEFAULT 0,
    ID_kavia             INT    NOT NULL,
    FOREIGN KEY (ID_kavia) REFERENCES kaviaren (ID_kaviarne) ON DELETE CASCADE,
    ID_uzivatela         INT    NOT NULL,
    FOREIGN KEY (ID_uzivatela) REFERENCES uzivatel (ID_osoby) ON DELETE CASCADE
)
;

CREATE TABLE recenzia_akcie
(
    ID_recenzia_akcia NUMBER PRIMARY KEY NOT NULL,
    datum             DATE   NOT NULL,
    hodnotenie        NUMBER(1) DEFAULT 0 NOT NULL,
    recenzia_mes      VARCHAR2(250),
    rozmanitost       NUMBER(1) DEFAULT 0,
    cenova_dost       NUMBER(1) DEFAULT 0,
    ID_cupp_akcie     INT    NOT NULL,
    FOREIGN KEY (ID_cupp_akcie) REFERENCES cupping_akcia (ID_cupping) ON DELETE CASCADE,
    ID_uzivatela      INT    NOT NULL,
    FOREIGN KEY (ID_uzivatela) REFERENCES uzivatel (ID_osoby) ON DELETE CASCADE
)
;

INSERT INTO kaviaren
VALUES (25596641, 'Kaviarnicka', 20,
        hodiny(TO_DATE('8:30:00AM','HH:MI:SSAM'), TO_DATE('8:00:00AM','HH:MI:SSAM'),
               TO_DATE('8:00:00AM','HH:MI:SSAM'), TO_DATE('9:00:00AM','HH:MI:SSAM'),
               TO_DATE('7:00:00AM','HH:MI:SSAM'), NULL, NULL),
        hodiny(TO_DATE('7:00:00PM','HH:MI:SSAM'), TO_DATE('8:00:00PM','HH:MI:SSAM'),
               TO_DATE('7:00:00PM','HH:MI:SSAM'), TO_DATE('7:00:00PM','HH:MI:SSAM'),
               TO_DATE('10:00:00PM','HH:MI:SSAM'), NULL, NULL));
INSERT INTO kaviaren
VALUES (05159822, 'Kaviareň u straky', 19,
        hodiny(TO_DATE('7:00:00AM','HH:MI:SSAM'), TO_DATE('7:00:00AM','HH:MI:SSAM'),
               TO_DATE('7:00:00AM','HH:MI:SSAM'), TO_DATE('7:00:00AM','HH:MI:SSAM'),
               TO_DATE('7:00:00AM','HH:MI:SSAM'), NULL, NULL),
        hodiny(TO_DATE('8:00:00PM','HH:MI:SSAM'), TO_DATE('8:00:00PM','HH:MI:SSAM'),
               TO_DATE('8:00:00PM','HH:MI:SSAM'), TO_DATE('8:00:00PM','HH:MI:SSAM'),
               TO_DATE('8:00:00PM','HH:MI:SSAM'), NULL, NULL));
INSERT INTO kaviaren
VALUES (26360641, 'Funiculi Funicula', 9,
        hodiny(TO_DATE('8:00:00AM','HH:MI:SSAM'), TO_DATE('8:00:00AM','HH:MI:SSAM'),
               TO_DATE('8:00:00AM','HH:MI:SSAM'), TO_DATE('8:00:00AM','HH:MI:SSAM'),
               TO_DATE('8:00:00AM','HH:MI:SSAM'), NULL, NULL),
        hodiny(TO_DATE('10:00:00PM','HH:MI:SSAM'), TO_DATE('10:00:00PM','HH:MI:SSAM'),
               TO_DATE('10:00:00PM','HH:MI:SSAM'), TO_DATE('10:00:00PM','HH:MI:SSAM'),
               TO_DATE('10:00:00PM','HH:MI:SSAM'), NULL, NULL));

INSERT INTO kava
VALUES (1, 'Espresso', 5, 'Silná', 'Povzbudzujúca');
INSERT INTO kava
VALUES (2, 'Latte', 1, 'Jemná', 'Dlhotrvajúca');
INSERT INTO kava
VALUES (3, 'Freddo', 3, 'Sladká', 'Ľadovo-osviežujúca');

INSERT INTO ponuka
VALUES (25596641, 2);
INSERT INTO ponuka
VALUES (05159822, 1);
INSERT INTO ponuka
VALUES (26360641, 2);

INSERT INTO zrno
VALUES (1, 'Turecko', 'Arabika');
INSERT INTO zrno
VALUES (2, 'Etiópia', 'Typica');
INSERT INTO zrno
VALUES (26360641, 'Filipíny', 'Liberika');

INSERT INTO pozozstava_z
VALUES (1, 1);
INSERT INTO pozozstava_z
VALUES (2, 2);
INSERT INTO pozozstava_z
VALUES (2, 2);

INSERT INTO dodatkove_sluzby
VALUES (25596641, 'N', 'Y', 'Y');
INSERT INTO dodatkove_sluzby
VALUES (05159822, 'Y', 'N', 'N');
INSERT INTO dodatkove_sluzby
VALUES (26360641, 'N', 'Y', 'N');

INSERT INTO zamestnanec
VALUES (1, 25596641, 'Konrád', 'Kováč', 'konkova@dzimail.sk', 421901587654, 'Y');
INSERT INTO zamestnanec
VALUES (2, null, 'Marianna', 'Horník', 'marianna.hornik@vonpozeraj.sk', 421909420696, 'N');
INSERT INTO zamestnanec
VALUES (3, 25596641, 'Bronislava', 'Zima', 'bronazima@coldmail.com', 421907247777, 'Y');

INSERT INTO pracuje
VALUES (25596641, 2);
INSERT INTO pracuje
VALUES (05159822, 1);
INSERT INTO pracuje
VALUES (26360641, 3);

INSERT INTO cupping_akcia
values (1, TO_DATE('8:30:00AM','HH:MI:SSAM'), 'COFFEE MARATHON: REVIVAL', 35, 25596641);
INSERT INTO cupping_akcia
values (2, TO_DATE('9:30:00AM','HH:MI:SSAM'), 'COFFEE CUP', 35, 25596641);
INSERT INTO cupping_akcia
values (3, TO_DATE('8:30:00AM','HH:MI:SSAM'), 'KAVOVA RULETA', 25, 05159822);

INSERT INTO uzivatel
VALUES (1, 'Jano', 'Janovsky', 'Janovec@jano.com', 'Lungo', 5, 'jozokavopic01', 2);
INSERT INTO uzivatel
VALUES (2, 'Fero', 'Janovsky', 'fero@jano.com', 'Lungo', 2, 'ferokavopic02', 1);
INSERT INTO uzivatel
VALUES (3, 'Zuza', 'Janovsky', 'zuza@random.com', 'Cappucino', 3, 'zuzakavopic03', 2);


INSERT INTO recenzia_akcie
VALUES (1, TO_DATE('13-AUG-21 12:56 A.M.','dd-mm-yyyy hh:mi A.M.'), 4, 'Vyborna atmosfera, po akcii som sa citil ako znovuzrodeny', 4, 4, 1, 1);
INSERT INTO recenzia_akcie
VALUES (2, TO_DATE('14-AUG-21 12:56 A.M.','dd-mm-yyyy hh:mi A.M.'), 1, 'Nic moc', 1, 1, 3, 2);
INSERT INTO recenzia_akcie
VALUES (3, TO_DATE('15-AUG-21 12:56 A.M.','dd-mm-yyyy hh:mi A.M.'), 4, 'Vyborne', 4, 4, 1, 3);



INSERT INTO recenzia_kaviaren
VALUES (1, TO_DATE('13-AUG-21 12:56 A.M.','dd-mm-yyyy hh:mi A.M.'), 5, 'Jediny nedostatok -> prilis dobra kava', 5, 5, 25596641, 1);
INSERT INTO recenzia_kaviaren
VALUES (2, TO_DATE('14-AUG-21 12:56 A.M.','dd-mm-yyyy hh:mi A.M.'), 1, 'Nie som fanusikom', 1, 1, 05159822, 2);
INSERT INTO recenzia_kaviaren
VALUES (3, TO_DATE('15-AUG-21 12:56 A.M.','dd-mm-yyyy hh:mi A.M.'), 4, 'Pekne prostredie, prijemna atmosfera', 5, 5, 25596641, 3);