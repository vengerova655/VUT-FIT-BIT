/*
 * IZP PROJEKT 1
 * Autor: VERONIKA VENGEROVA, xvenge01@stud.fit.vutbr.cz
 * Datum: 16.11.2019
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 102

//Funkcia v zmenenom retazci vyhlada hladany retazec
//hlada postupne od zaciatku po koniec retazca, v ktorom sa moze nachadzat hladany retazec
//hlada sa preto do dlzky retazca - dlzky hladaneho retazca a pokial sa najde hladany znak
//skontroluje sa, ci aj ostatne hladane znaky sa v retazci bezprostredne za sebou nachadzaju
//ak nie, tak sa pokracuje po prvom najdenom znaku
bool Finding_Number(char* strChanged, char *UserInputN)
{
        bool Err;//chyba pri hladani v retazci
        for (unsigned i=0;i<=strlen(strChanged)-strlen(UserInputN);i++) {
                unsigned j=0;//index hladaneho cisla
                if (strChanged[i]==UserInputN[j]) {
                        unsigned k=i+1;//chceme kontrolovat cely retazec/nasledujuce znaky
                        Err=false;//ziadna chyba pri hladani=nachadza sa

                        while (!Err && k < (i + strlen(UserInputN))) {
                                j++;
                                if (strChanged[k] != UserInputN[j])
                                        Err = true;//chyba pri hladani=nenachadza sa
                                k++;
                        }
                        if (!Err)
                                break;
                }
        }
        return !Err;
}


//funkcia vytvori novy retazec so zmenenymi prislusnymi znakmi
//porovnava hodnoty charov a ak sa ich hodnota nachadza v danom rozsahu
//nahradi ich vhodnym znakom
void Get_Changed_Line(char *strLine, char *strChanged)
{
        char c;
        unsigned i = 0;
        for (; i < strlen(strLine); i++) {
                c = strLine[i];
                if (c >= 'A' && c <= 'Z')
                        c = c + ('a' - 'A');
                else if (c >= 'a' && c <= 'c')
                        c = '2';
                else if (c >= 'd' && c <= 'f')
                        c = '3';
                else if (c >= 'g' && c <= 'i')
                        c = '4';
                else if (c >= 'j' && c <= 'l')
                        c = '5';
                else if (c >= 'm' && c <= 'o')
                        c = '6';
                else if (c >= 'p' && c <= 's')
                        c = '7';
                else if (c >= 'w' && c <= 'z')
                        c = 9;
                else if (c >= 't' && c <= 'v')
                        c = '8';
                else if (c == '+')
                        c = '0';
                strChanged[i] = c;
        }
        strChanged[i] = '\0';//ukoncenie retazca
}

//vypis retazca v jednom riadku v predpisanej podobe
void Write_Out(char* strName, char* strNumber)
{
        strName[strlen(strName) - 1] = '\0';
        printf("%s, %s", strName, strNumber);
}


//kontrola dodrzania maximalnej dlzky retazca
//zistujeme, ci dlzka retazca je vacsia ako 100 tym, ze porovnavame, ci dany
//retazec ma aspon 101 znakov a ci posledny znak pred '\0' sa rovna '\n'
//ak ano, retazec ma zodpovedajucu dlzku, ak nie, je dlhsi
bool Check_Line_Length(char * strLine)
{
        if ((strlen(strLine) >= (MAXSIZE-1)) && (strLine[MAXSIZE-2] != '\n')) {
                fprintf(stderr, "Chyba vstupu\n");
                char ch;
                while ((ch = getchar()) != '\n' &&  ch != '\0')//vyprazdnenie zvysku
                        ;
                return false;
        } else
                return true;
}


//vypis vsetkych kontaktov po zadani prazdneho argumentu
void Write_All(char* strName,char* strNumber)
{
        while (fgets(strName,MAXSIZE,stdin) && fgets(strNumber,MAXSIZE,stdin)) {
                if (Check_Line_Length(strName) && Check_Line_Length(strNumber))
                        Write_Out(strName, strNumber);
        }

}



int main(int argc, char **argv)
{
        char strName[MAXSIZE];
        char strNumber[MAXSIZE];
        char strChanged[MAXSIZE];//premenene na cisla
        bool ContainsAny = false;//obsahuje celkovo v zozname
        bool Contains;//obsahuje v konkretnom kontakte
        bool chlength;//check length

        //ked uzivatel nezada, co hlada
        if (argc == 1)
                Write_All(strName, strNumber);
        else {
                char *UserInputN = argv[1];
                //kontrola vhodnosti zadaneho hladaneho retazca
                for (unsigned i = 0; i < strlen(UserInputN); i++) {
                        if (!isdigit(UserInputN[i])) {
                                printf("Neplatny vstup\n");
                                return -1;
                        }
                }

                while (fgets(strName, MAXSIZE, stdin)) {
                        fgets(strNumber, MAXSIZE, stdin);
                        chlength = (Check_Line_Length(strName) && Check_Line_Length(strNumber));//ci maju oba riadky vhodny pocet znakov

                        if (chlength) {
                                Contains = false;
                                //hladany retazec sa moze nachadzat iba v retazci dlhsom/rovnom ako je on sam
                                if (strlen(strName) >= strlen(UserInputN)) {
                                        Get_Changed_Line(strName, strChanged);
                                        Contains = Finding_Number(strChanged, UserInputN);
                                }
                                if (!Contains && (strlen(strNumber) >= strlen(UserInputN))) {
                                        Get_Changed_Line(strNumber, strChanged);
                                        Contains = Finding_Number(strChanged, UserInputN);
                                }

                                if (Contains) {
                                        Write_Out(strName, strNumber);
                                        ContainsAny = true;
                                }
                        }
                }
                if (chlength && !ContainsAny) {
                        printf("Not found\n");
                }

        }
        return 0;
}
