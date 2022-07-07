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
    odroda  VARCHAR(100)
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
INSERT INTO kaviaren
VALUES (53703472, 'Kaviaren Stolec', 20,
        hodiny(TO_DATE('8:30:00AM','HH:MI:SSAM'), TO_DATE('8:00:00AM','HH:MI:SSAM'),
               TO_DATE('8:00:00AM','HH:MI:SSAM'), TO_DATE('9:00:00AM','HH:MI:SSAM'),
               TO_DATE('7:00:00AM','HH:MI:SSAM'), NULL, NULL),
        hodiny(TO_DATE('7:00:00PM','HH:MI:SSAM'), TO_DATE('8:00:00PM','HH:MI:SSAM'),
               TO_DATE('7:00:00PM','HH:MI:SSAM'), TO_DATE('7:00:00PM','HH:MI:SSAM'),
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
values (1, TO_DATE('8:30:00 AM','HH:MI:SS AM'), 'COFFEE MARATHON: REVIVAL', 35, 25596641);
INSERT INTO cupping_akcia
values (2, TO_DATE('9:30:00 AM','HH:MI:SS AM'), 'COFFEE CUP', 35, 25596641);
INSERT INTO cupping_akcia
values (3, TO_DATE('8:30:00 AM','HH:MI:SS AM'), 'KAVOVA RULETA', 25, 05159822);

INSERT INTO uzivatel
VALUES (1, 'Jano', 'Janovsky', 'Janovec@jano.com', 'Lungo', 5, 'jozokavopic01', 2);
INSERT INTO uzivatel
VALUES (2, 'Fero', 'Janovsky', 'fero@jano.com', 'Lungo', 2, 'ferokavopic02', 1);
INSERT INTO uzivatel
VALUES (3, 'Zuza', 'Janovska', 'zuza@random.com', 'Cappucino', 3, 'zuzakavopic03', 2);
INSERT INTO uzivatel
VALUES (4, 'Ferdinand', 'Bulo', 'fero@ordinary.com', 'Latte', 3, 'BuloFero', 3);


INSERT INTO recenzia_akcie
VALUES (1, TO_DATE('13-08-21 12:56:30 AM','dd-mm-yy hh:mi:ss AM'), 4, 'Vyborna atmosfera, po akcii som sa citil ako znovuzrodeny', 4, 4, 1, 1);
INSERT INTO recenzia_akcie
VALUES (2, TO_DATE('14-08-21 12:56:01 AM','dd-mm-yy hh:mi:ss AM'), 1, 'Nic moc', 1, 1, 3, 2);
INSERT INTO recenzia_akcie
VALUES (4, TO_DATE('15-09-21 12:56:20 AM','dd-mm-yy hh:mi:ss AM'), 4, 'Vyborne', 3, 3, 1, 3);



INSERT INTO recenzia_kaviaren
VALUES (1, TO_DATE('13-12-21 02:56:12 AM','dd-mm-yy hh:mi:ss AM'), 5, 'Jediny nedostatok -> prilis dobra kava', 5, 1, 25596641, 1);
INSERT INTO recenzia_kaviaren
VALUES (2, TO_DATE('14-03-21 02:56:10 AM','dd-mm-yy hh:mi:ss AM'), 1, 'Nie som fanusikom', 1, 1, 05159822, 2);
INSERT INTO recenzia_kaviaren
VALUES (3, TO_DATE('15-01-21 09:56:05 AM','dd-mm-yy hh:mi:ss AM'), 4, 'Pekne prostredie, prijemna atmosfera', 2, 5, 25596641, 3);
INSERT INTO recenzia_kaviaren
VALUES (4, TO_DATE('16-01-21 01:56:21 AM','dd-mm-yy hh:mi:ss AM'), 4, 'Super kaviaren, ale da zlepsit', 4, 4, 05159822, 3);
INSERT INTO recenzia_kaviaren
VALUES (5, TO_DATE('17-02-21 12:56:34 AM','dd-mm-yy hh:mi:ss AM'), 2, 'Zlata stredna cesta, uplny priemer', 3, 3, 05159822, 1);
INSERT INTO recenzia_kaviaren
VALUES (6, TO_DATE('20-04-21 10:56:12 AM','dd-mm-yy hh:mi:ss AM'), 1, 'Priserna', 1, 1, 26360641, 2);

/*
  JOIN dvoch tabuliek
  hľadá cupping akcie, ktorých hodnotenie rozmanitosti je aspoň 2
  */
SELECT
       ca.ID_cupping,
       ca.datum,
       ca.nazov_akcie,
       ca.kapacita,
       ca.ID_kaviarne,
       ra.rozmanitost
FROM cupping_akcia ca JOIN recenzia_akcie ra ON ca.ID_cupping = ra.ID_cupp_akcie WHERE ra.rozmanitost > 2;

/*
  JOIN dvoch tabuliek
  hľadá užívateľov, ktorí hodnotili cupping akcie horšie ako hodnotením 3
  */
SELECT
       u.ID_osoby,
       u.krstne_meno,
       u.priezvisko,
       u.mail,
       u.nick,
       ra.hodnotenie
FROM uzivatel u JOIN recenzia_akcie ra on u.ID_osoby = ra.ID_uzivatela WHERE ra.hodnotenie < 3;

/*
  SELECT používajúci GROUP BY
  vypisuje všetky kaviarne, ktoré majú recenziu a ich priemerné hodnotenie
  */
SELECT
       rk.ID_KAVIA,
       ROUND(AVG(rk.hodnotenie),2) priemer_hodotenie,
       ROUND(AVG(rk.atmosfera),2) priemer_atmosfera,
       ROUND(AVG(rk.pristup),2) priemer_pristup
FROM recenzia_kaviaren rk GROUP BY rk.ID_KAVIA;

/*
  SELECT používajúci IN
  vypisuje kaviarne, ktorých hodnotenie je rovné 1
  */
SELECT
       k.ID_kaviarne,
       k.nazov,
       k.capacity
FROM kaviaren k WHERE k.ID_kaviarne IN (SELECT rk.ID_kavia FROM recenzia_kaviaren rk WHERE rk.hodnotenie = 1);


/*
  JOIN troch tabuliek
  hľadá uzivatelov, ktorych celkove hodnotenie akcie je mensie ako 2, vybera aj skore ktore pridelili akcii a jej nazov
  */
SELECT u.ID_osoby,
       u.krstne_meno,
       u.nick,
       u.oblubeny_sposob_prip_kavy,
       u.priezvisko,
       ca.nazov_akcie,
       ra.hodnotenie
FROM uzivatel u JOIN recenzia_akcie ra ON  u.ID_osoby = ra.ID_uzivatela
JOIN cupping_akcia ca on ca.ID_cupping = ra.ID_cupp_akcie
WHERE ra.hodnotenie < 2;


/*
  SELECT používajúci GROUP BY (aj JOIN)
  vypisuje id kavy, kolko uzivatelov danu kavu oblubuje a jej nazov
  */
SELECT u.ID_KAVA,
       k.nazov,
       COUNT(u.ID_KAVA)
FROM uzivatel u JOIN kava k ON u.ID_KAVA = k.ID_kavy
GROUP BY u.ID_KAVA, k.nazov;


/**
  SELECT pouzivajuci EXISTS
  vybera vsetky kaviarne, ktore su hodnotene nejakymi recenziami
 */
SELECT *
FROM kaviaren k
WHERE EXISTS( SELECT *
    FROM recenzia_kaviaren rk
    WHERE k.ID_kaviarne = rk.ID_KAVIA
          );
