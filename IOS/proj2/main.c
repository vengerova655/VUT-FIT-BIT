/*
 * IOS PROJEKT 2
 * Autor: VERONIKA VENGEROVA, xvenge01@stud.fit.vutbr.cz
 * Datum: 06.05.2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

FILE *pfile;
#define MAX_VAL 2000
#define MIN_VAL 0
#define EXPECTED_NUM_ARG 6
int allocShared(int **pointer) {
    *pointer = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    return *pointer != NULL;
}

void deallocShared(int *pointer) {
    munmap(pointer,sizeof(int));
}


//delays process for random miliseconds
void newsleep(int max_time)
{
    max_time *= 10;
    if (max_time != 0)
    {
        usleep(rand()%max_time);
    }
}

typedef sem_t Semaphore;

int sem_signal(Semaphore *sem)
{
    return sem_post(sem);
}

//current number of immigrants in building whose registration not confirmed yet
int *NE = NULL;

//current number of immigrants in building
int *NB = NULL;


//current number of immigrants registered, but not confirmed yet
int *NC = NULL;

//serial number of action
int *A = NULL;

//judge in building or not
int *judge = NULL;

//no change during output
Semaphore *mutex_output = NULL;

//all immigrants in building registered
Semaphore *allSignedIn = NULL;

//stops/allows to enter/leave
Semaphore *noJudge = NULL;

//NE and NC protected by mutex
Semaphore *mutex = NULL;

//after all registrations confirmed can leave/enter
Semaphore *all_confirmed = NULL;


//functions initiliazes all semaphores and mmaps
int init()
{
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    //really random numbers
    srandom((unsigned) time(NULL));


    pfile = fopen("proj2.out","w");
    if (pfile == NULL) {
        fprintf(stderr,"ERROR -> file cannot be open\n");
    }

    setbuf(pfile, NULL);

    int allok = 1;
    allok &= allocShared(&NE);
    allok &= allocShared(&judge);
    allok &= allocShared(&NC);
    allok &= allocShared(&NB);
    allok &= allocShared(&A);

    if (allok != 1)
        return 0;
    //sets initial values
    *A = 0;
    *NE = 0;
    *NB = 0;
    *NC = 0;
    *judge = 0;

    //opens semaphores
    if((mutex_output = sem_open("/xvenge01.ios.proj2.mutex_output", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
    if((noJudge = sem_open("/xvenge01.ios.proj2.noJudge", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
    if((mutex = sem_open("/xvenge01.ios.proj2.mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
    if((all_confirmed = sem_open("/xvenge01.ios.proj2.all_confirmed", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) return -1;
    if((allSignedIn = sem_open("/xvenge01.ios.proj2.allSignedIn", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;

    return 1;
}

//function acts as a judge
void proces_judge(int JG, int JT, int PI)
{
    //number of already processed immigrants
    unsigned already_processed = 0;

    //only 1 judge -> has to process all immigrants
    while(PI - already_processed > 0) {
        newsleep(JG);

        //output
        sem_wait(mutex_output);
        (*A)++;
        fprintf(pfile,"%d\t: JUDGE\t: wants to enter\n", *A);
        sem_signal(mutex_output);

        sem_wait(noJudge);
        sem_wait(mutex);

        //output
        sem_wait(mutex_output);
        (*A)++;
        fprintf(pfile,"%d\t: JUDGE\t: enters\t\t: %d\t: %d\t: %d\n", *A, *NE, *NC, *NB);
        sem_signal(mutex_output);


        *judge = 1;     //judge in building
        if (*NE != *NC) {
            //output
            sem_wait(mutex_output);
            (*A)++;
            fprintf(pfile,"%d\t: JUDGE\t: waits for imm\t: %d\t: %d\t: %d\n", *A, *NE, *NC, *NB);
            sem_signal(mutex_output);

            sem_signal(mutex);
            sem_wait(allSignedIn);
        }

        //output
        sem_wait(mutex_output);
        (*A)++;
        fprintf(pfile,"%d\t: JUDGE\t: starts confirmation\t: %d\t: %d\t: %d\n", *A, *NE, *NC, *NB);
        sem_signal(mutex_output);

        newsleep(JT);

        for (int i = 0; i < *NC; ++i) {
            sem_signal(all_confirmed);
            ++already_processed;
        }

        //everyone from these is confirmed
        *NE = *NC = 0;

        //output
        sem_wait(mutex_output);
        (*A)++;
        fprintf(pfile,"%d\t: JUDGE\t: ends confirmation\t: %d\t: %d\t: %d\n", *A, *NE, *NC, *NB);
        sem_signal(mutex_output);

        newsleep(JT);

        //output
        sem_wait(mutex_output);
        (*A)++;
        fprintf(pfile,"%d\t: JUDGE\t: leaves\t\t: %d\t: %d\t: %d\n", *A, *NE, *NC, *NB);
        sem_signal(mutex_output);

        *judge = 0;     //judge leaves

        sem_signal(mutex);
        sem_signal(noJudge);
    }
    //output
    sem_wait(mutex_output);
    ++(*A);
    fprintf(pfile,"%d\t: JUDGE\t: finishes\n",*A);
    sem_signal(mutex_output);
    exit(0);
}

//funciton processess immigrants
void proces_immigrant(int id, int IT) {
    //output
    sem_wait(mutex_output);
    (*A)++;
    fprintf(pfile,"%d\t: IMM %d\t: starts\n",*A, id);
    sem_signal(mutex_output);

    //Judge cannot be in the building for imm to enter
    sem_wait(noJudge);
    ++(*NE);
    ++(*NB);

    //output
    sem_wait(mutex_output);
    (*A)++;
    fprintf(pfile,"%d\t: IMM %d\t: enters\t\t: %d\t: %d\t: %d\n",*A, id, *NE, *NC, *NB);
    sem_signal(mutex_output);

    sem_signal(noJudge);


    sem_wait(mutex);
    ++(*NC);
    //output
    sem_wait(mutex_output);
    (*A)++;
    fprintf(pfile,"%d\t: IMM %d\t: checks\t\t: %d\t: %d\t: %d\n",*A, id, *NE, *NC, *NB);
    sem_signal(mutex_output);

    //judge in building and have to just wait for judge -> all are registered
    if (*judge == 1 && *NE == *NC) {
        //output
        sem_wait(mutex_output);
        (*A)++;
        fprintf(pfile,"%d\t: JUDGE\t: waits for imm\t: %d\t: %d\t: %d\n",*A, *NE, *NC, *NB);
        sem_signal(mutex_output);

        sem_signal(allSignedIn);
    } else {
        sem_signal(mutex);
    }

    sem_wait(all_confirmed);

    //output
    sem_wait(mutex_output);
    (*A)++;
    fprintf(pfile,"%d\t: IMM %d\t: wants certificate\t: %d\t: %d\t: %d\n",*A, id, *NE, *NC, *NB);
    sem_signal(mutex_output);

    newsleep(IT);

    //output
    sem_wait(mutex_output);
    (*A)++;
    fprintf(pfile, "%d\t: IMM %d\t: got certificate\t: %d\t: %d\t: %d\n",*A, id, *NE, *NC, *NB);
    sem_signal(mutex_output);


    sem_wait(noJudge);

    --(*NB);

    //output
    sem_wait(mutex_output);
    (*A)++;
    fprintf(pfile, "%d\t: IMM %d\t: leaves\t\t: %d\t: %d\t: %d\n",*A, id, *NE, *NC, *NB);
    sem_signal(mutex_output);

    sem_signal(noJudge);
    exit(0);
}

//function unlinks and closes all semaphores and frees all mmaps, closes opened file
void cleanup()
{
    deallocShared(NC);
    deallocShared(NE);
    deallocShared(judge);
    deallocShared(NB);
    deallocShared(A);

    sem_close(mutex_output);
    sem_close(mutex);
    sem_close(all_confirmed);
    sem_close(noJudge);
    sem_close(allSignedIn);
    sem_unlink("xvenge01.ios.proj2.mutex_output");
    sem_unlink("xvenge01.ios.proj2.noJudge");
    sem_unlink("xvenge01.ios.proj2.all_confirmed");
    sem_unlink("xvenge01.ios.proj2.mutex");
    sem_unlink("xvenge01.ios.proj2.allSignedIn");

    //closes file
    if (pfile != NULL){
        fclose(pfile);
    }

}

//helps to generate immigrant proccesses
void generate_immigrants(int PI, int IG, int IT)
{
    for (int i = 1; i <= PI; i++)
    {
        //creates new proces
        newsleep(IG);
        pid_t igID = fork();

        if (igID == 0) {
            //child
            proces_immigrant(i,IT);
        } else if (igID < 0) {
            fprintf(stderr, "ERROR-cannot create child proces\n");
            exit(1);
        }

        newsleep(IT);
    }

    //has to wait for all children to be done
    for (int i = 1; i <= PI; i++) {
        wait(NULL);
    }

    exit(0);
}

int main(int argc, char **argv) {

    //checks if all neccessary input
    if ( argc != EXPECTED_NUM_ARG ) {
        fprintf( stderr, "ERROR -> invalid input\n" );
        return 1;
    }
    int PI, IG, JG, IT, JT;
    //checks if input numbers are integers
    char *endp;
    for( int i = 1; i < argc; i++) {
        char *str = argv[i];
        int value = strtol(str, &endp, 10);
        if (*endp != '\0'){
            fprintf( stderr, "ERROR -> invalid input\n" );
            return 1;
        }

        if (i == 1){
            PI = value;

        } else if (i == 2) {
            IG = value;
        } else if (i == 3) {
            JG = value;
        } else if (i == 4) {
            IT = value;
        } else {
            JT = value;
        }
    }

    if ( PI < 1){
        fprintf( stderr, "ERROR -> invalid PI\n" );
        return 1;
    }

    if(IG < MIN_VAL || IG > MAX_VAL){
        fprintf( stderr, "ERROR -> invalid IG\n" );
        return 1;
    }

    if(JG < MIN_VAL || JG > MAX_VAL){
        fprintf( stderr, "ERROR -> invalid JG\n" );
        return 1;
    }

    if(IT < MIN_VAL || IT > MAX_VAL){
        fprintf( stderr, "ERROR -> invalid IT\n" );
        return 1;
    }

    if(JT < MIN_VAL || JT > MAX_VAL){
        fprintf( stderr, "ERROR -> invalid JT\n" );
        return 1;
    }

    if (!init())
    {
        fprintf( stderr, "ERROR -> fault in inicialization\n" );
        return 1;
    }

    pid_t  genID = fork();
    if (genID == 0){
        generate_immigrants(PI, IG, IT);
    } else if (genID < 0) {
        fprintf(stderr, "ERROR-cannot create child proces\n");
        exit(1);
    }


    //creates new function
    pid_t judgeID = fork();
    if (judgeID == 0) {
        proces_judge(JG, JT, PI);
    } else if (judgeID == -1) {
        fprintf(stderr, "ERROR-cannot create child proces\n");
        exit(1);
    }

    waitpid(judgeID, NULL, 0);
    waitpid(genID, NULL, 0);

    cleanup();
    return 0;
}