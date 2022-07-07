/**
*       IZP PROJEKT 3
*       Autor: VERONIKA VENGEROVA, xvenge01@stud.fit.vutbr.cz
*       Dattum: 13.12.2019
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//return hodnoty pre testy, ktore program vykonava
#define TEST_SUCCESS 0
#define TEST_FAILURE 1

//return hodnota pre nespravne vstupne policko
#define VSTUP_FAIL -1

//oznacenie stran
#define LEFT 0
#define VERTICAL 1
#define RIGHT 2


//vyber pravej/lavej cesty
#define RPATH 1
#define LPATH 0

//hodnoty hranic, ktore sa maju kontrolovat
//LEFTBORDER najnizsi bit = 1 (001)
//RIGHTBORDER = 2 (010)
//VERTICALBORDER najvyssi bit = 4 (100)
#define VERTICALBORDER 4
#define LEFTBORDER 1
#define RIGHTBORDER 2


//podla toho na akom trojuholniku sme sa bud steny posuvaju o 2, alebo 1
//dalej, v potupnosti LVR
#define POSUN2 2
#define POSUN 1

//pozicia skumanej bunky v mape
#define POZICIABUNKY (r*map->cols + c)

//funkcia vypise ako obsluhovat dany program
void help(){
    printf("Zadaj:\n");
    printf("--help pre vypisanie pomocnika ako ovladat program\n");
    printf("--lpath pre vypisanie pohybu bludiskom pomocou pravidla lavej ruky\n");
    printf("--rpath pre vypisanie pohybu bludiskom pomocou pravidla pravej ruky\n");
    printf("--test pre odtestovanie spravnosti vstupneho bludiska\n");
}


//struktura mapy
typedef struct {
        int rows;
        int cols;
        unsigned char *cells;
} Map;


//funkcia kontroluje spravnost znakov
//kontroluje, ci sa nachadzaju iba biele znaky a cisla
//kontroluje tiez spravny pocet cisel
int kontrola_znakov(FILE *f) {
    char c;
    
    //nastavenie pointera na zaciatok suboru
    fseek(f,0,SEEK_SET);


    //kontroluje, ci sa iba cisla a biele znaky nachadzaju v subore
    while((c = fgetc(f))!=EOF) {
        if(!isspace(c) && !isdigit(c)) {
            fprintf(stderr,"Invalid\n");
            return TEST_FAILURE;
        }
    }


    //nastavenie na zaciatok suboru, uz sme ho raz presli cely
    fseek(f,0,SEEK_SET);

    //nacitanie poctu riadkov a stlpcov 
    int rows, cols;
    fscanf(f,"%d %d", &rows, &cols);
   

    //pomocna hodnota na nacitavanie cisel suboru
    int tmp;
 
    //spocitame pocet cisel
    int pocetPrvkov = 0;
    while((fscanf(f,"%d", &tmp))!=EOF) {
       pocetPrvkov++;
    }

    //aby sme v dalsom kroku vedeli s fileom pracovat opat od zaciatku
    fseek(f,0,SEEK_SET);
   
    if(pocetPrvkov != (rows * cols)) {
        return TEST_FAILURE;
    }

    return TEST_SUCCESS;
}


//program nacitava cisla z nasho file do uz vytvorenej mapy
void nacitaj(Map *m, FILE *f) {
    
    //pocet riadkov a stlpcov
    int rows, cols;
    fscanf(f,"%d %d", &rows, &cols);

    //postupne sa do buniek ukladaju cisla mapy
    for(int i = 0; i < (m->rows)*(m->cols); i++) {
        int tmp;
        fscanf(f,"%d", &tmp);
        m->cells[i]=tmp;
    }
}


//vytvori a alokuje pamat pre mapu s danymi paramterami
Map* vytvor(FILE *f){
    Map* m = malloc(sizeof(Map));

    //ak sa podarilo alokovat nacitat a alokovat bunky
    if( m ) {
        fscanf(f,"%d %d",&m->rows, &m->cols);
        m->cells = malloc(m->rows * m->cols * sizeof(int));
    }

    return m;
}

// uvolni pamat buniek
int uvolni(Map *m){
    if( m->cells ){
        free(m->cells);
    }
    if( m ){
        free(m);
    }
    return 0;
}


//funkcia ziti, ci na danom mieste je konkretna hranica
//vstupnymi parametrami zadam polohu a hranicu ktoru kontrolujem
//funkcia porovnava bitovu hodnotu, vracia bity, ktore maju spolocne
//v tomto pripade bitovym and ziskame bud hranica, alebo nulovy bit (false)
bool isborder(Map *map, int r, int c, int border) {
    unsigned char *kontrolujeme = &map->cells[POZICIABUNKY];

    if( (*kontrolujeme&border) == border) {
        return true;
    }

    return false;
}

//kontrola spravnosti borderov, susedne steny musia mat spolocnu hranicu ak tam je
//lavy trojuholnik ak ma pravu hranicu tak susedny pravy musi mat lavu atd
//a takto kontrolujeme celu mapu, ale kotntrolujeme to vzdy s nasledujucou hodnotou
//for cyklus preto neprebieha cez celu mapu, ale hranicne hodnoty ako su pravy stlpec a dolny riadok
//nemame uz s cim porovnavat 
int test(Map *m){
   
    for(int i = 0; i < m->rows-1; i++) {
        for(int j = 0;j < m->cols-1; j++) {
            //ak na oboch stranach su/nie su bordery tak sedia
            //ak na jednom je hranica a na druhom nie, tak nesedi
            //ide o xor
            if((isborder(m,i,j,RIGHTBORDER))^(isborder(m,i,(j+1),LEFTBORDER))) {
                fprintf(stderr,"Invalid\n");
                return TEST_FAILURE;
            } 

            //musia sediet takisto aj vertikalne, kontrolujeme tie, ktore maju dolnu hranicu s tymi, ktoru maju hornu
            if((i+j)%2==1){
                 if((isborder(m,i,j,VERTICALBORDER))^(isborder(m,i+1,j,VERTICALBORDER))) {
                    fprintf(stderr,"Invalid\n");
                    return TEST_FAILURE;
                }
            }
       }
    }

    return TEST_SUCCESS;
}


//funkcia nam posuva ruku na stene, respektive ktorou stenou pokracujeme
//z parametrov ziskame, akou potupnostou ziskame dalsiu stenu
//modulo 3 pretoze su iba 3 hodnoty 0-3 (Lava, Vertikalna, Prava)
int Posun_Trojuholnika(int posun,int vstup) {
    return (vstup + posun)%3;
}

//vypise nam, ktora hranica ma nasledovat po tom, ako jej zadame
//ci sme na parnom neparnom riadku/stlpci respektive na hornom, dolnom trojuholniku
//a ci sa riadim pravym alebo lavym pravidlom
//na zaklade toho cez funkciu Trojuholnik ziskame ktora stena naselduje
int follow_border(int r, int c, int vstup, int leftright) {
    int out;

    //ak pravidlo pravej ruky
    if(leftright == RPATH) {
        //ak parna tak postupnost stien ako sa na ne pozerame je vzdy LEFT RIGHT VERTICAL
        if((r+c)%2==0) {
            //out sa zmeni na nasledujucu stenu
            out = Posun_Trojuholnika(POSUN2, vstup);
        }

        //ak neparna tak vzdy postupnost LVR
        else {
            out = Posun_Trojuholnika(POSUN, vstup);
        }
    }

    //ak pravidlo lavej ruky
    if(leftright == LPATH) {
        //ak neparne tak vzdy postupnost LVR
        if((r+c)%2 == 1) {
            out = Posun_Trojuholnika(POSUN2, vstup);
        }
        //ak parne tak vzdy postupnost LRV
        else {
            out = Posun_Trojuholnika(POSUN, vstup);
        }
    }
    return out;
}


//funkcia nam vrati ktorou hranicou vstupujeme do zaciatku bludiska
//kontroluje, ci su zadane valid vstupne udaje a ci tadial mozeme vstupit
int vstup_do_bludiska(Map *m, int r, int c) {
    //nastavenie hodnoty na vstup fail, ak sa nebude dat vstupit na danej suradnici ziadnou stranou
    //hodnota zostane na vstup fail
    int vstup = VSTUP_FAIL;
   
    //ci sa da vstupit dolnou hranicou
    if(r == m->rows - 1) {
        //da sa vstupit iba cez trojuholniky, ktore maju vertikalnu stenu smerom von z bludiska
        if((r+c)%2==1) {
            if(!isborder(m, r, c, VERTICALBORDER)) {
                //vstup nastavime na stenu, ktorou do bludiska vstupujeme
                vstup = VERTICAL;
            }
        }
    }

    //ci sa da vstupit hornou hranicou
    if(r == 0) {
        //mozeme vstupit iba tymi, ktore maju vert. stenu smerom von z bludiska
        if((r+c)%2==0) {
            if(!isborder(m, r, c, VERTICALBORDER)) {
                vstup = VERTICAL;
            }
        }
    }

    //ci je mozny vstup pravou stranou
    if(c == m->cols - 1) {
        if(!isborder(m, r, c, RIGHTBORDER)) {
            vstup = RIGHT;
        }
    }

    //ci je mozny lavou stenou
    if(c == 0) {
        if(!isborder(m, r, c, LEFTBORDER)) {
            vstup = LEFT;
        }
    }

    return vstup;
}


//vrati nam ktorou hranicou vstupujeme
int start_border(Map *m, int r, int c,int  rl){
    //vratenie hodnoty ktorou sme vtupili
    int vstupdoBlud = vstup_do_bludiska(m,r,c);

    //ci boli vhodne parametre zadane
    if(vstupdoBlud==VSTUP_FAIL) {
        return VSTUP_FAIL;
    }

    //vrati ktorou hranicou pokracujeme
    return follow_border(r,c,vstupdoBlud,rl);
}


int path(Map *m, int r, int c, int rl) { 
    // uzivatel zadava v rozsahu 1 - pocet riadkov/stlpcov
    //my pracujeme v rozsahu 0 - pocet riadkov/stlpcov - 1
    r--;
    c--;

    //nastavenie stenu ktorou sa ako prvy "drzim" rukou
    int stena = start_border(m,r,c,rl);
    if(stena == VSTUP_FAIL){ 
        if(m != NULL)
            uvolni(m);
        return TEST_FAILURE;
       
    }

    //na nastavenie hodnoty, ktoru kontrolujeme
    int  kontrolujemehr;
    
    //aby sme sa nedostali mimo rozmedzia ktore mame alkovane
    while((r>-1 && r<m->rows) && (c>-1 && c<m->cols))
    {
        //nastavuje hodnotu hranice ktoru kontrolujem
        if(stena == LEFT)
        {
           kontrolujemehr = LEFTBORDER;
        }
        else if(stena == RIGHT)
       {
           kontrolujemehr = RIGHTBORDER;
        }
        else if(stena == VERTICAL)
        {
             kontrolujemehr = VERTICALBORDER;
        }

        //ak je na stene, ktoru kontrolujeme hranica, posuvame sa na dalsiu
        if(isborder(m,r,c,kontrolujemehr)) {
            //pre RPATH
            if(rl==RPATH){
                if(((r+c)%2==1)) {
                    stena = Posun_Trojuholnika(POSUN, stena);
                }
                else {
                stena = Posun_Trojuholnika(POSUN2, stena);
                
                }
            }


            //pre LPATH
            else {
                if(((r+c)%2==1)) {
                    stena = Posun_Trojuholnika(POSUN2, stena);
                }
                else {
                stena = Posun_Trojuholnika(POSUN, stena);
                
                }
            }
        }

        //ak tam nie je hranica pokracujeme podla toho na ktorej hranici sme boli
        else {
            printf("%d,%d\n",r+1,c+1);
            switch(stena) {
                //kracame doprava
                case RIGHT: 
                    c++;
                    //ale stena, ktorou sme prisli v dalsom trojuholniku je Lava
                    stena = LEFT;
                    break;

                //kracame dolava
                case LEFT:
                    c--;
                    //do dalsieho trojuholniku sme vstupili ale inou stranou
                    stena = RIGHT;
                    break;


                //kracame dolu/hore podla horneho/dolneho trojuholniku
                case VERTICAL:
                    if((r+c)%2 == 0){
                        r--;
                    }
                    else {
                        r++;
                    } 
                    
            }
            //nastavenie nasledujucej hranice ktorej sa budeme drzat
            stena = follow_border(r, c, stena, rl);
        }
    }
    return TEST_SUCCESS;
}


//funkcia nam zavola funkcie potrebne pre vypisanie cesty von z bludiska lavou rukou
int rlpathmenu(char *string1, char *string2, char *string3, int rl) {
    char *p;
    
    FILE *f;
    //ci sa subor spravne otvoril
    f = fopen(string3, "r");
    if(f == NULL) {
        fprintf(stderr,"Invalid\n");
        return  TEST_FAILURE;
    }

    Map *m =vytvor(f);

    //ci boli r a c zadane v spravnom formate
    int r = strtol(string1,&p,10);
    if(*p!='\0'){
        fprintf(stderr,"Invalid\n");
        if(m != NULL)
            uvolni(m);
        fclose(f);
        return TEST_FAILURE;
    }

    int c = strtol(string2,&p,10);
    if(*p!='\0'){
        fprintf(stderr,"Invalid\n");
        if(m != NULL)
            uvolni(m);
        fclose(f);
        return TEST_FAILURE;
    }

    if(kontrola_znakov(f) == TEST_FAILURE) {
        fprintf(stderr,"Invalid\n");
        fclose(f);
        return TEST_FAILURE;
    }
    
    nacitaj(m,f);

    if(test(m) == TEST_FAILURE) {
        if (m)
            uvolni(m);
        fprintf(stderr,"Invalid\n");
        fclose(f);
        return TEST_FAILURE;
    }

    if(path(m, r, c, rl)==TEST_FAILURE) {
        if(m)
            uvolni(m);
        fprintf(stderr,"Invalid\n");
        fclose(f);
        return TEST_FAILURE;
    }

    uvolni(m);
    fclose(f);
    return TEST_SUCCESS;
}

int test_menu(char *subor) {
    FILE *f;
    f = fopen(subor, "r");
    //ci spravne otvoreny file
    if(f == NULL) {
        fprintf(stderr,"Invalid\n");
        return  TEST_FAILURE;
    }

    Map *m =vytvor(f);

   if(kontrola_znakov(f) == TEST_FAILURE) {
        fprintf(stderr,"Invalid\n");
        fclose(f);
        return TEST_FAILURE;
    }
    
    nacitaj(m,f);

    if(test(m) == TEST_FAILURE) {
        if (m)
            uvolni(m);
        fclose(f);
        return TEST_FAILURE;
    }

    uvolni(m);
    fclose(f);
    return TEST_SUCCESS;
}

int main(int argc, char **argv ){
    //kontrola vstupov
    if(argc == 1) {
        fprintf(stderr,"Invalid\n");
        return  TEST_FAILURE;
    }

    //zistenie ktora funkcia sa ma vykonat
    if(strcmp("--help",argv[1]) == TEST_SUCCESS) {
        //kontrola poctu argumentov pre danu funkciu
        if(argc != 2) { 
            fprintf(stderr,"Invalid\n");
            return  TEST_FAILURE;
        }
        help();
        return TEST_SUCCESS;
    }

    if(strcmp("--lpath",argv[1]) == TEST_SUCCESS) {
        //kontrola poctu argumentov pre danu funkciu
         if(argc != 5) {
            fprintf(stderr,"Invalid\n");
            return  TEST_FAILURE;
        }

        rlpathmenu(argv[2], argv[3], argv[4], LPATH);
        return TEST_SUCCESS;       
    }

    if(strcmp("--rpath",argv[1]) == TEST_SUCCESS) {
        //kontrola poctu argumentov pre danu funkciu
         if(argc != 5) {
            fprintf(stderr,"Invalid\n");
            return  TEST_FAILURE;
        }
        rlpathmenu(argv[2], argv[3], argv[4], RPATH);
        return TEST_SUCCESS;       
    }

    if(strcmp("--test",argv[1]) == TEST_SUCCESS) {
        //kontrola poctu argumentov pre danu funkciu
        if(argc != 3) {
            fprintf(stderr,"Invalid\n");
            return  TEST_FAILURE;
        }
        if(test_menu(argv[2]) == TEST_FAILURE){
            return TEST_FAILURE;
        }
        printf("Valid\n");
        return TEST_SUCCESS;
    }
    return TEST_SUCCESS;
}
