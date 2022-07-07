# IPK projekt 1

Server komunikujúci prostredníctvom HTTP protokolu, ktorý počúva na užívatelom určenom porte a na základe požiadovkou podáva informácie o systéme.
Užívateľ môže zistiť záťaž systému, hostname, a informácií o CPU.

## Server
Server sa spúšťa s 1 povinným argumentom - číslo portu v rozsahu 0-65535. Server na zadanom porte (ak naň bolo možné pripojiť sa) neustále počúva, až kým ho neeliminujeme (Ctrl+C ak nebeží na pozadí, ak beží na pozadí musíme ho eliminovať ručne pomocou príkazu kill).
Server je implementovaný tak, aby obsluhoval iba 1 klienta v 1 okamih. Ide o blokujúci vstup/výstup. 

## Príklad využitia

Server spustíme príkazom:
```
$ ./hinfosvc 12345
```
Ktorým sa server spustí a začne načúvať na porte 12345. Následne sa klienti môžu naň pripojiť pomnocou wget, curl alebo prostredníctvom internetového prehliadača.

```
$ curl http://servername:12345/load
$ curl http://servername:12345/cpu-name
$ curl http://servername:12345/hostname
```
## Autor
- xvenge01 (Veronika Vengerová)

## Zdroje
* https://tools.ietf.org/html/rfc7231
* https://developer.mozilla.org/en-US/docs/Web/HTTP
* https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
* Prednášky z predmetu IPK FIT VUT, rok 2022
* Hands-on Network Programming with C,ISBN 978-1-78934-986-3
