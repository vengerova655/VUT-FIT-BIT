# Implementační dokumentace k 2. úloze do IPP 2021/2022
jméno a příjmení: Veronika Vengerová \
login: xvenge01 
### 1. Interpret.py
Interpret bol implementovaný v jazyku Python 3.8. Skript vykonáva analýzu a interpretáciu XML súboru s programom (do formy XML preložený program jazyka 
IPPcode22).
Interpret číta zdrojový kód a súbor na čítanie zo vstupu pre inštrukciu READ z užívateľom zadaného súboru, alebo zo štandardného vstupu. 
Výstupom je samotný výsledok vykonania interpretovaného programu.
##### Interpret je rozdelený do 3 súborov.

- frames/Frames.py, v ktorom sa nachádza implementácie tried Frame a FrameStack na reprezentáciu rámcov a zásobníka rámcov.
- instruction/Instructions.py, v ktorom je implementácia tried Instruction, InstructionList a InstructionController.
- interpret.py v ktorom je hlavné spracovanie argumentov pomocou ArgumentParser z knižnice argparse, spracovanie xml súboru a hlavná while loop na realizáciu inštrukcií.

V interpret.py dochádza k parsovaniu xml súboru na koreň a deti pomocou xml.etree.ElementTree knižnice.
Z detí koreňa získaného stromu sa vytvorí inštanciu triedy ***Instruction*** reprezentujúcu inštrukciu a uložia sa do nej jej argumenty. 

Vytvára sa taktiež inštancia triedy ***InstructionList***, v ktorej sú uložené všetky inštancie triedy Instruction. 
Inštancia triedy InstructionList nám reprezentuje celý vstupný program.
Sú v nej dopredu uložené inštrukcie a labels. K ich konkrétnemu spracovaniu ešte nedochádza.
InstructionList má okrem slovníku inštrukcií aj parameter curr, ktorý ukazuje, na poradie inštrukcie, ktorú momentálne interpret spracováva.

Ďalej sa vytvorí inštancia triedy ***InstructionController***, ktorá riadi samotné vykonávanie inštrukcií v inštancií InstructionList.
Trieda InstructionController obsahuje všetky nutné zásobníky (zásobník volania, dátový zásobník, zásobník rámcov) a rámce (GF, TF, LF).
Trieda funguje ako 1 velký switch, kde po spustení ***realize_instruction()*** sa spustí konkrétna instrukcia podľa potreby.
K spracovávaniu dochádza až kým nedôjde program na koniec, alebo nepríde k chybe.


### 2. Test.php
Skript hladá súbory buď v aktuálnom adresári alebo v užívateľom zadanom adresári a následne ich odtestuje. 
Výstupom je HTML stránka, ktorá je smerovaná na štandardný výstup.
Test sa skladá z 3 súborov.

- test.php, čo je hlavný súbor na generovanie html výstupu.
- helper.php, čo je súbor v ktorom je implementované všetky triedy a funkcie na vykonanie testov a na získanie pomocných výsledkov html výstupu.
- styles.css, čo je súbor v ktorom sa nastavuje výzor html výstupu.

V programe helper.php sú implementované 4 hlavné triedy ***Tester***, 
abstraktná trieda ***Result***, ***Testing*** a ***ResultList*** a 3 podtriedy triedy Result.
Inštancia triedy Tester môźe byť iba jedna nakoľko sa jedná o celkovú konfiguráciu skriptu, ktorá sa mení na základe požiadavkov užívateľa.
Jedná sa o singleton. 

Result je abstraktná trieda, ktorá predstavuje výsledok testu, skladá sa z cesty,mena testu a pomocných hodnôt na následné zobrazenie a nastavenie html výstupu.
Testovanie sa však rozlišuje v 3 rôznych prípadoch. 
Ak chce užívateľ odtestovať iba parser, ak chce užívateľ odtestovať iba interpret, 
alebo ak chce užívateľ odtestovať oba naraz.
Výsledný formát výsledku je rovnaký, no testovanie iné preto sa vytvorili 3 podtriedy ***ParseResult***, ***InterpretResult*** a ***BothResult***, triedy Result. 

O to, aká podtrieda sa vytvorí sa rozhoduje v triede Testing, ktorá sa stará o riadenie testu. 
Testing načíta .src súbory, vytvorí na základe užívateľom zadaných parametrov podtriedu Result, 
uloží ju v inštancii triedy ResultList na základe ktorej sa neskôr vypĺňa html a ak užívateľ nevyžiada aby sa pomocné súbory nezmazali, zmaže všetky vytvorené pomocné súbory.

