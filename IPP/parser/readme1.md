## Implementační dokumentace k 1. úloze do IPP 2021/2022
### Jméno a příjmení: Veronika Vengerová
### Login: xvenge01

### parse.php

V skripte parse.php je implementovaný parser pre jazyk IPPode22. 

Na začiatku bola uložená gramatika jazyka IPPcode22 v podobe 2-rozmerného pola *IPP22gram*. To pozostáva z inštrukcií a typu prislúchajúcim argumentom.
Napríklad:

```
 "MOVE"=> array("var", "symb"),
```

Zdrojový kód je čítaný zo štandardného vstupu pomocou funkcie *stream\_get\_contents()*. 
Tá ukladá vstup do 1 reťazca, ktorý sa následne rozdelí podľa riadkov pomocou funkcie explode().

Z takto spracovaného zdrojového kôdu sa následne vymažú komentáre a prázdne riadky.

Ďalej sa kód bez komentárov rozdelí do 2D pola (poradie riadku => pole operačného kódu a operandov, v ktorom každý operand a operačný kôd je predstavený zvlášť).

Následne sa zistí, či zdrojový kód obsahoval správnu hlavičku *.IPPcode22*.

Pole sa ďalej spracováva porovnávaním inštrukcií s polom *IPP22gram*. 

Ak bola inštrukcia správna, z pola *IPP22gram* sa zistí očakávaný typ a počet argumentov.

Ak bol správny počet argumentov, tak sa pomocou regulárnych výrazov zistí, či boli argumenty typovo vhodne zadané.

Na generovanie XML bol využitý *DOMDocument()*, ktorý vytvorí nový DOM objekt.

Doň sa následne postupne pridávali inštrukcie podľa zadaného spracovvaného zdrojového kôdu.
