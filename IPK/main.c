#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFSIZE 1024

//author: xvenge01 (Veronika Vengerova)

void get_cpu_stat(long double *NIdle, long double  *NonIdle){
    long double a[8];
    long double user, nice, system, idle, iowait, irq, softirq,steal;
    FILE *f = fopen("/proc/stat", "r");
    fscanf(f,"%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf",&user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    fclose(f);
    *NIdle = idle + iowait;
    *NonIdle = user + nice + system + irq + softirq + steal;
}

void get_cpu_load(long double *Load){
    long double PrevIdle, PrevNonIdle;
    long double Idle, NonIdle;
    get_cpu_stat(&PrevIdle,&PrevNonIdle);
    sleep(1);
    get_cpu_stat(&Idle,&NonIdle);
    long double PrevTotal, Total;
    PrevTotal = PrevIdle + PrevNonIdle;
    Total = Idle + NonIdle;

    long double TotalDif, IdleDif;
    TotalDif = Total - PrevTotal;
    IdleDif = Idle - PrevIdle;


    *Load = (TotalDif - IdleDif) / TotalDif * 100.0;
}


void get_hostname(char *HostName){
    FILE *f;
    f = fopen("/proc/sys/kernel/hostname", "r");
    fscanf(f,"%s", HostName);
    fclose(f);
}

void get_CPU_name(char *CPUName){
    FILE *f;
    f = popen( "cat /proc/cpuinfo | grep \"model name\" | head -n 1 | awk -F \":\" {'print $2'}", "r");
    fscanf(f,"%[^\n]", CPUName);
    pclose(f);
}

int main(int argc, char**argv) {

    if(argc != 2 ){
        perror("ERROR\nusage: ./hinfosvc portNumber\n");
        exit(EXIT_FAILURE);
    }
    char*p;
    long conv = strtol(argv[1],&p,10);
    if(*p != '\0' || conv > 65535 || conv < 0){//long to short potrebujeme iba od 0-65535, ono si ustriehne uz ked sa binduje na zly
        perror("ERROR: port not number\nusage: ./hinfosvc portNumber\n");
        exit(EXIT_FAILURE);
    }

    //premenne na vysledky zataze, hostname a informaciach o cpu
    long double Load;
    char HostName[255];
    char CPUName[255];

    struct sockaddr_in serverAdress, cli;
    int sockfd;
    long portNumber = conv;
    int optval = 1;
    char buf[BUFFSIZE];

    //kolko bytov  sa precitalo/ dostal ood klienta
    int bytecount;

    //header na OK spravu na odoslanie
    char *header = "HTTP/1.1 200 OK\nContent-Type: text/plain\n";

    //premenna na celkovu spravu, ktoru odosiela server klientovi
    char message[1024];


    //vytvorenie socketu
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<=0){
        perror("ERROR in creating socket");
        exit(EXIT_FAILURE);
    }

    bzero(&serverAdress, sizeof(serverAdress));
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAdress.sin_port = htons((unsigned short )portNumber);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (const void *)&optval, sizeof(int));


    if((bind(sockfd, (struct sockaddr*)&serverAdress,sizeof(serverAdress)))!=0){
        perror("ERROR in binding");
        exit(EXIT_FAILURE);
    }

    if((listen(sockfd,1)) != 0){
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }


    int len = sizeof(cli);
    while(1) {
        int comm_socket = accept(sockfd, (struct sockaddr *) &cli, &len);

        if (comm_socket < 0) {
            perror("ERROR: accept");
            exit(EXIT_FAILURE);
        }

        bzero(buf, BUFFSIZE);

        for(;;) {
            bytecount = recv(comm_socket, buf, sizeof buf - 1, 0);

            //chyba pri ziskavani bytov
            if (bytecount <= 0) {
                break;
            }

            //ziskanie prveho riadku poziadavku
            char *token = strtok(buf, "\r\n");

            //proces spracovava iba GET poziadavky
            if (strncmp("GET /", token, 5)) {
                const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
                                   "Connection: close\r\n"
                                   "Content-Length: 11\r\n\r\nBad Request";
                send(comm_socket, c400, strlen(c400), 0);
            }
            else {
                if (token != NULL) {
                    if (strcmp(token, "GET /hostname HTTP/1.1") == 0) {
                        get_hostname(HostName);
                        snprintf(message, sizeof(message), "%sContent-Length: %d\r\n\r\n%s", header, strlen(HostName),
                                 HostName);
                        send(comm_socket, message, strlen(message), 0);
                    }
                    else if (strcmp(token, "GET /load HTTP/1.1") == 0) {
                        get_cpu_load(&Load);
                        char temp[64];
                        snprintf(temp, sizeof(temp), "%Lf", Load);
                        snprintf(message, sizeof(message), "%sContent-Length: %d\r\n\r\n%s%%", header, strlen(temp) + 1,
                                 temp);
                        send(comm_socket, message, strlen(message), 0);

                    }
                    else if (strcmp(token, "GET /cpu-name HTTP/1.1") == 0) {
                        get_CPU_name(CPUName);
                        snprintf(message, sizeof(message), "%sContent-Length: %d\r\n\r\n%s", header, strlen(CPUName),
                                 CPUName);
                        send(comm_socket, message, strlen(message), 0);

                    }
                    else {
                        const char *c404 = "HTTP/1.1 404 Not Found\r\n"
                                           "Connection: close\r\n"
                                           "Content-Length: 9\r\n\r\nNot Found";
                        send(comm_socket, c404, strlen(c404), 0);
                    }
                }
            }
        }
        //zatvorenie pripojenia
        close(comm_socket);
    }
    close(sockfd);
}
