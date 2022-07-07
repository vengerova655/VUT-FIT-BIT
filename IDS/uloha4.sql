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
DROP VIEW  zrnoView CASCADE CONSTRAINTS;
DROP SEQUENCE  zrno_seq;
DROP MATERIALIZED VIEW kava_popularity;

-----creating new datatypes-----------
CREATE OR REPLACE TYPE hodiny IS VARRAY(7) OF DATE;
----creating tables-------------------
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
    hodnotenie           FLOAT(3) DEFAULT NULL,
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


-------------INSERT-----------------------
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
INSERT INTO kava
VALUES (4, 'Latte Biele', 2, 'Jemná', 'Vynikajúca');
INSERT INTO kava
VALUES (5, 'Freddo Dark', 5, 'Horká', 'Silná');

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
INSERT INTO pozozstava_z
VALUES (26360641, 5);

INSERT INTO dodatkove_sluzby
VALUES (25596641, 'N', 'Y', 'Y');
INSERT INTO dodatkove_sluzby
VALUES (05159822, 'Y', 'N', 'N');
INSERT INTO dodatkove_sluzby
VALUES (26360641, 'N', 'Y', 'N');

INSERT INTO zamestnanec
VALUES (1, 25596641, 'Konrád', 'Kováč', 'konkova@dzimail.sk', 421901587654, 'Y');
INSERT INTO zamestnanec
VALUES (4, 25596641, 'Konrádisko', 'Kováč', 'konkova@dzimail.sk', 421901587654, 'N');
INSERT INTO zamestnanec
VALUES (2, null, 'Marianna', 'Horník', 'marianna.hornik@vonpozeraj.sk', 421909420696, 'N');
INSERT INTO zamestnanec
VALUES (3, 25596641, 'Bronislava', 'Zima', 'bronazima@coldmail.com', 421907247777, 'Y');

INSERT INTO pracuje
VALUES (25596641, 2);
INSERT INTO pracuje
VALUES (25596641, 4);
INSERT INTO pracuje
VALUES (25596641, 1);
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
INSERT INTO uzivatel
VALUES (5, 'Bonifác', 'Záhradník', 'bono@gardener.com', 'Latte', 2, 'bonoTheMacka', 4);


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


CREATE VIEW zrnoView AS
    SELECT * FROM zrno;
CREATE SEQUENCE zrno_seq START WITH 1000 INCREMENT BY 1;

---------------------triggers----------------
--do hodnotenia da priemer (zaokruhleny nadol) zo zvysnych hodnoteni v recenzii
CREATE OR REPLACE TRIGGER recenziaTrigger
    BEFORE INSERT ON recenzia_kaviaren
    FOR EACH ROW
    BEGIN
        IF :NEW.hodnotenie  IS NULL THEN
            :NEW.hodnotenie := (:NEW.atmosfera + :NEW.pristup)/2;
        END IF;
    END;

CREATE OR REPLACE TRIGGER newtestTrigger
INSTEAD OF INSERT ON zrnoView
DECLARE
    newId NUMBER;

BEGIN
    IF :NEW.ID_zrna IS NULL THEN
        newId := zrno_seq.nextval;
    ELSE
        newId := :NEW.ID_zrna;
    END IF;
    INSERT INTO zrno(ID_zrna, oblast, odroda)
    VALUES (newId, :NEW.oblast, :NEW.odroda);
end;

/*Demonštrácia triggeru*/
INSERT INTO zrnoView VALUES (NULL, 'Test s primary key NULL', 'Test s primary key NULL');


/*Demonštrácia triggeru*/
INSERT INTO recenzia_kaviaren (ID_recenzia_kaviaren, datum, recenzia_mes, atmosfera, pristup, ID_kavia, ID_uzivatela)
VALUES (7, TO_DATE('13-12-21 02:56:12 AM','dd-mm-yy hh:mi:ss AM'), 'Jediny nedostatok -> prilis prijemna atmosfera', 5, 1, 25596641, 1);

------------------processess-----------------
CREATE OR REPLACE PROCEDURE odstran_vsetky_kavy_obashujuce_zrno_aj_so_zrnom (zrno_ID zrno.ID_zrna%TYPE) AS
num_found NUMBER := 0;
CURSOR cursor_zrno IS
    SELECT ID_zrna FROM zrno
        WHERE ID_zrna = zrno_ID;
BEGIN
    OPEN cursor_zrno;
    FETCH cursor_zrno INTO num_found;
    CLOSE cursor_zrno;

    IF num_found > 0 THEN
       DELETE FROM kava
            WHERE ID_kavy IN
              (SELECT ID_kavy FROM pozozstava_z WHERE ID_zrna = zrno_ID);
        DELETE FROM zrno
            WHERE ID_zrna = zrno_ID;
    END IF;
end;
ALTER PROCEDURE ODSTRAN_VSETKY_KAVY_OBASHUJUCE_ZRNO_AJ_SO_ZRNOM compile;

/*Demonštrácia procedúry*/
CALL odstran_vsetky_kavy_obashujuce_zrno_aj_so_zrnom(10);



CREATE OR REPLACE PROCEDURE kaviaren_workers (kaviaren_name IN VARCHAR)
AS
    num_zam NUMBER DEFAULT 0;
    num_without_maj NUMBER DEFAULT 0;
    kaviaren_id kaviaren.ID_kaviarne%TYPE;
    kav_work_id kaviaren.ID_kaviarne%TYPE;
    uziv_work_id uzivatel.ID_osoby%TYPE;
    is_maj_res char(1);
    cursor cursor_kav IS SELECT  ID_kaviarne, ID_osoby FROM pracuje;
BEGIN
    SELECT ID_kaviarne INTO kaviaren_id
    FROM kaviaren
        WHERE kaviaren_name=nazov;
      OPEN cursor_kav;

    LOOP
        FETCH cursor_kav INTO kav_work_id,uziv_work_id;
        EXIT WHEN cursor_kav%NOTFOUND;
        IF kaviaren_id=kav_work_id THEN
            num_zam := num_zam +1;
        end if;

        SELECT je_majitel INTO is_maj_res
            FROM zamestnanec
                WHERE ID_osoby=uziv_work_id;

        if NOT is_maj_res='Y' THEN
            num_without_maj := num_without_maj +1;
        end if;
    end loop;
    if cursor_kav%ISOPEN THEN
        CLOSE cursor_kav;
    end if;
    DBMS_OUTPUT.PUT_LINE('V kaviarni '|| kaviaren_name || ' pracuje '|| num_zam|| ' zamestnancov (' || num_without_maj ||' bez majitela)');

EXCEPTION
    WHEN NO_DATA_FOUND THEN
		DBMS_OUTPUT.put_line(
			'Kaviarna ' ||kaviaren_name || ' nezadana v databaze.'
		);
    WHEN OTHERS THEN
    DBMS_OUTPUT.put_line(
			'MISTAKE OCCURED'
		);

end;
ALTER PROCEDURE  kaviaren_workers compile;
/*Demonštrácia procedúry*/
CALL kaviaren_workers('Kaviarnicka');

-----------Materialized view-----------
CREATE MATERIALIZED VIEW kava_popularity AS
   SELECT u.ID_KAVA,
       k.nazov,
       COUNT(u.ID_KAVA)
FROM uzivatel u JOIN kava k ON u.ID_KAVA = k.ID_kavy
GROUP BY u.ID_KAVA, k.nazov;
-- vybranie vsetkych riadkov z materialized view pred zmenou
SELECT * FROM kava_popularity;

UPDATE kava k SET k.nazov='Lungo' WHERE k.ID_kavy=2;
-- hodnota v KAVA tabulke je zmenena, ale v materialized view nie je
SELECT * FROM kava_popularity;

----------PRIVILEGES--------------
GRANT ALL on KAVA to XVISKU01;
GRANT ALL ON KAVIAREN TO XVISKU01;
GRANT ALL on RECENZIA_AKCIE to XVISKU01;
GRANT ALL ON RECENZIA_KAVIAREN to XVISKU01;
GRANT ALL on CUPPING_AKCIA to XVISKU01;
GRANT ALL ON ponuka to XVISKU01;
GRANT ALL ON dodatkove_sluzby to XVISKU01;
GRANT ALL ON uzivatel to XVISKU01;
GRANT ALL ON zrno to XVISKU01;
GRANT ALL ON zamestnanec to XVISKU01;
GRANT ALL ON pozozstava_z to XVISKU01;
GRANT ALL ON pracuje to XVISKU01;

GRANT EXECUTE ON ODSTRAN_VSETKY_KAVY_OBASHUJUCE_ZRNO_AJ_SO_ZRNOM TO XVISKU01;
GRANT EXECUTE ON KAVIAREN_WORKERS TO XVISKU01;
GRANT ALL ON XVENGE01.KAVA_POPULARITY TO XVISKU01;


----------EXPLAIN PLAN--------------
/*Explain plan bez indexu (pri sekvenčom spustení),
  ktorý vyberá kávy s jemnou arómou a počíta
  priemer káv denne u užívateľov, ktorí ich obľubujú*/
EXPLAIN PLAN FOR SELECT u.ID_KAVA,
    k.nazov,
    AVG(u.pocet_kav_denne)
FROM uzivatel u JOIN kava k ON u.ID_KAVA = k.ID_kavy WHERE aroma = 'Jemná'
GROUP BY u.ID_KAVA, k.nazov;

/*Výpis EXPLAIN PLAN-u*/
SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());


/*Index zrýchlujúci dotazy nad arómou kávy*/
CREATE INDEX aromaIndex ON kava (aroma);


/*Explain plan s indexom (pri sekvenčnom spustení),
  ktorý vyberá kávy s jemnou arómou a počíta
  priemer káv denne u užívateľov, ktorí ich obľubujú*/
EXPLAIN PLAN FOR SELECT u.ID_KAVA,
    k.nazov,
    AVG(u.pocet_kav_denne)
FROM uzivatel u JOIN kava k ON u.ID_KAVA = k.ID_kavy WHERE aroma = 'Jemná'
GROUP BY u.ID_KAVA, k.nazov;

/*Výpis EXPLAIN PLAN-u*/
SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());

----------TRANSACTIONS--------------

/*Transakcia ukazujúca potvrdenie zmien*/
COMMIT;
SET TRANSACTION NAME 'potvrdenie upravenia';
BEGIN
    /*Užívateľ si želá zmeniť počet vypitých káv za deň*/
    UPDATE uzivatel SET pocet_kav_denne = 7 WHERE ID_osoby = 1;
    END;
COMMIT;

/*Transakcia ukazujúca návrat omylom nastavených hodnôt*/
COMMIT;
SET TRANSACTION NAME 'vratenie_spat_omylom_pridanych_hodnot';
BEGIN
    /*Vývojár chce zmeniť len jednu hodnotu v tabuľke ale zabudne na WHERE*/
    UPDATE kaviaren SET capacity = 30;
    /*Vráti zmeny za pomoci ROLLBACK*/
    ROLLBACK;
END;

COMMIT;

/*Transakcia ukazujuca savepoint a moznost zvratenia zmien tabulky*/
--pred zmenou tabulky
SET TRANSACTION NAME 'demonstracia savepointu pred vymazanim';
BEGIN
    DELETE FROM zamestnanec WHERE ID_osoby=3;
    --vymazany 1 riadok tabulky
    SAVEPOINT SP2;
    --vymazany 1 riadok tabulky
    DELETE FROM zamestnanec WHERE ID_osoby=3;
    --teraz sme ale zistili, ze chceme tabulku pred poslednym vymazanim riadku->rollback
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
        ROLLBACK TO SP2;
END;
--vysledne zmeny ulozime
COMMIT;