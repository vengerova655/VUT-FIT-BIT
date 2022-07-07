#include <stdio.h>
#include<unistd.h>
#include<thread>
#include<queue>
#include<mutex>
#include<vector>
#include <iostream>
#include<string.h>




std::vector<std::mutex *> zamky,zamkyo; /* pole zamku promenne velikosti */
int linescore;
char *line;
int min;
std::mutex score_mtx;
int num;
int res;
std::mutex *waiting_mtx; 
std::mutex *line_mtx;

int waiting;


//stavovy automat na najdenie hladaneho stringu v retazci-> iba 1 vyskyt v riadku
void dothis(char *str, int value,int i) {
	usleep(1);
    //riadice na pohyb riadkom
    int k = 0;
    int state = 0;
    //prechadzame cely riadok
    while(line[k]!='\0'){
        switch(state){
            case 0:
                if(line[k]==str[0]){state = 1;break;}
                state = 0;
                break;
            case 1:
                if(line[k]==str[1]){state = 2;break;}
                if(line[k]==str[0]){state = 1;break;}
                state = 0;
                break;
            case 2:
                if(line[k]==str[0]){state = 1;break;}
                if(line[k]==str[2]){state = 3;break;}
                state = 0;
                break;
            case 3:
                if(line[k]==str[0]){state = 1;break;}
                if(line[k]==str[3]){state = 4;break;}
                state = 0;
                break;
            case 4:
                if(line[k]==str[0]){state = 1;break;}
                if(line[k]==str[4]){
                    score_mtx.lock();
                    usleep(1);
                    linescore+= value;
                    score_mtx.unlock();
                    return;
                }
				state=0;
				break;
        }
        k++;
    }
}

void f(int i, char * str, int value, std::mutex *zamky){
    // int OK=0;
   
    (*zamky).lock();

	if(strlen(str)!=5){
		fprintf(stderr,"Chyba -> hladany retazec nema pozadovanu dlzku (5)\n");
		exit(1);
	}

    while(res)
    {
    
        //prejdenie riadkom ci je tam stirng a pricita k celkovej hodnote riakdu ked sa nachadza
        dothis(str,value,i);

        waiting_mtx->lock();
		if(waiting+1 == num){
			waiting = 0;
			//spracovany riadok vsektymi threadmi->novy riadok moze nacitat
			line_mtx->unlock();
		}
		else
		{
			++waiting;
		}
		waiting_mtx->unlock();

        (*zamky).lock();

    }
    
}

//nacitanie riadka zo stdin
char *read_line(int *res) {
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) {
		str=(char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		*res=1;
		return str;
	} else {
		*res=0;
		return NULL;
	}

}


int main(int argc, char **argv) {
	/*******************************
	 * Inicializace threadu a zamku
	 * *****************************/
	if((argc<4)||(argc%2==1))
	{
		fprintf(stderr,"CHYBA V POCTE ARGUMENTOV\n");
		return 1;
	}
	waiting=0;
	linescore=0;
	//inicializacia mutexov
    waiting_mtx = new std::mutex;
	line_mtx = new std::mutex;

    //pocet threadov
	num=(argc-2)/2;
    //minimalne score zadane na vypisanie riadku
    min = atoi(argv[1]);
    
	int num_zamky=num;
	std::vector <std::thread *> threads; /* pole threadu promenne velikosti */

	/* vytvorime zamky */
	zamky.resize(num_zamky); /* nastavime si velikost pole zamky */
	for(int i=0;i<num_zamky;i++){	
		std::mutex *new_zamek = new std::mutex();
		zamky[i]=new_zamek;
		/* Pokud je treba, tak vytvoreny zamek muzeme rovnou zamknout */
		(*(zamky[i])).lock();
	}
  	line_mtx->lock();	// inicializujem ho na zamknuty

	/* vytvorime thready */
	threads.resize(num); /* nastavime si velikost pole threads */
	for(int i=0;i<num;i++){	
        // usleep(1);
		std::thread *new_thread = new std::thread (f,i,argv[2+i*2],atoi(argv[3+i*2]), zamky[i]);
		threads[i]=new_thread;
	}
	/**********************************
	 * Vlastni vypocet aut
	 * *******************************/
	// int res;
	line=read_line(&res);
	while (res) {
        //kazdy riadok nove score
		score_mtx.lock();
        linescore=0;
		score_mtx.unlock();

        for(int i=0;i<num;i++) {
          zamky[i]->unlock();
        }
       
		line_mtx->lock();

        //celkova hodnota riadku je vacsia ako minimum, preto vypisat
        if(linescore>=min){
		    printf("%s\n",line);
        }
		free(line); /* uvolnim pamet */
        line=read_line(&res);
	}
	
	//thready unlockneme
    for(int i=0;i<num;i++) (*(zamky[i])).unlock();



	/**********************************
	 * Uvolneni pameti
	 * ********************************/

	/* provedeme join a uvolnime pamet threads */
	for(int i=0;i<num;i++){

		(*(threads[i])).join();
        delete threads[i];
	}
	/* uvolnime pamet zamku */
	for(int i=0;i<num_zamky;i++){
		delete zamky[i];
	}
	
}