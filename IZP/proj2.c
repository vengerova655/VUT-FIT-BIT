/*
*       IZP PROJEKT 2
*       Autor: VERONIKA VENGEROVA, xvenge01@stud.fit.vutbr.cz
*       Dattum: 1.12.2019
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>


#define Io (1e-12)
#define Ut 0.0258563


//funkcia na vypocet z vyjadrenej Shockleyho rovnice
double get_Value (double Up, double Uo, double R) {
        return ((Io) * ((exp(Up/Ut)) - 1.0) - (Uo - Up) / R);
}


//funkcia pomocou bisekcie hlada pracovny bod diody
//nastavime najmensiu a najvacsiu dosiahnutelnu hodnotu
//a UP budeme ziskat ako strednu hodnotu pomocou bisekcie
//az kym sa nedostaneme do rozmedzia odchylky
double diode (double Uo, double R, double EPS) {
        double Up;                      //napatie na diode
        double Kvalue;                  //hodnota z kirchofovych zakonov
        double max = Uo, min = 0.0;             //maximalna a minimalna hodnota

        Up = (min + max) / 2;           //nastavenie na stred intervalu
        double UpPrevious;
        Kvalue = get_Value(Up, Uo, R);

        do
        {
                UpPrevious = Up;                //ulozenie hodnoty pred zmenou na neskorsie porovnanie

                //pri vypocte nadobuda hodnota s minimom zap. hodnoty
                //vieme skontrolovat ci potrebujeme zvysit, alebo znizit minimum
                //pri vypocitani hodnoty s minimom, dostaneme zapornu hodnotu
                //vieme teda, ze ked kladnu vynasobime so zapornou dostaneme zapornu hodnotu
                //vieme ktoru hranicu posuvat
                if(Kvalue * get_Value(min, Uo, R) < 0.0) {
                        max = Up;               //posunutie maximalnej hranice
                }
                else{
                        min = Up;               //posunutie minimalnej hranice
                }
                Up = (min + max) / 2;
                Kvalue = get_Value(Up, Uo, R);
        }while(fabs(UpPrevious - Up) > EPS);//aby sa nam cely program nezacyklil
        return Up;
}

//funkcia na vypocet prudu prechadzajucim diodou
double Pocitanie_Ip(double Up){
        return (Io*(exp(Up/Ut)-1.0));
}

//funkcia na skontrolovanie pointera pri premene str na double
bool check_pointer(char *p){
        if(*p != '\0'){
                fprintf(stderr,"error: invalid arguments\n");
                return false;
        }
        return true;
}

//funkcia na skontrolovania spravnosti vstupov
//skontroluje, ci boli zadane cisla
//skontroluje aj ci sa nezadal zaporny odpor
//alebo zaporna odchylka, alebo zaporne napatie
//Shockleyho rovnica nerata so zapornymi hodnotami Uo
int kontrola_vstupov(char *str1, char *str2, char *str3){
        char *p;
        double double1 = strtod(str1, &p);      //skontrolovat prvu hodnotu double
        bool check;
        check = check_pointer(p);

        double double2 = strtod(str2, &p);      //skontrolovat druhu hodnotu double
        if(check) {
                check = check_pointer(p);
        }

        double double3 = strtod(str3, &p);      //skontrolovat 3 hodnotu double
        if(check){
                check = check_pointer(p);
        }
        if(!check){
                return 1;
        }

        if(double1 < 0 || double2 < 0 || double3 < 0) {
                fprintf(stderr,"error: invalid arguments\n");
                return 1;
        }
        return 0;

}

int main(int argc, char **argv){
        //kontrola poctu parametrov
        if(argc!=4){
                fprintf(stderr, "error: invalid arguments\n");
                return 1;
        }

        //skontrolujeme spravnost vstupov
        int kontrola = kontrola_vstupov(argv[1], argv[2], argv[3]);
        if(kontrola == 1)
                return 1;

        //hodnoty uz skontrolovane nacitame
        char *p;
        double Uo = strtod(argv[1],&p);         //pociatocne napatie
        double R = strtod(argv[2],&p);          //odpor
        double EPS = strtod(argv[3],&p);        //odchylka


        double Up = diode(Uo, R, EPS);          //vysledne napatie
        double Ip = Pocitanie_Ip(Up);           //vysledny prud na diode

        //vypis
        printf("Up=%g V\n", Up);
        printf("Ip=%g A\n", Ip);
        return 0;
}
