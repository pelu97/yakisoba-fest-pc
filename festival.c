#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define CLIENTS 10
#define WORKERS 1
#define CHEFS 3
#define COUNTER 10


void* client(void* arg);
void* worker(void* arg);
void* chef(void* arg);


int Counter[COUNTER];
int CounterFillSlot = 0;


void* client(void* arg){
    int id = *((int*) arg);
    printf("Cliente criado %d\n", id);

    while(TRUE){

    }
}

void* worker(void* arg){
    int id = *((int*) arg);
    printf("Worker criado %d\n", id);

    while(TRUE){

    }
}

void* chef(void* arg){
    int id = *((int*) arg);
    printf("Chef criado %d\n", id);

    while(TRUE){

    }
}



int main(){
    pthread_t Clients[CLIENTS], Workers[WORKERS], Chefs[CHEFS];
    int *id, i;

    for(i=0;i<CLIENTS;i++){
        //create clients
        id = (int) malloc(sizeof(int));
        id = i;
        pthread_create(Clients[i], NULL, client, (void*) id);
    }

    for(i=0;i<WORKERS;i++){
        //create workers
        id = (int) malloc(sizeof(int));
        id = i;
        pthread_create(Workers[i], NULL, worker, (void*) id);
    }

    for(i=0;i<CHEFS;i++){
        //create chefs
        id = (int) malloc(sizeof(int));
        id = i;
        pthread_create(Chefs[i], NULL, chef, (void*) id);
    }


    pthread_join(&Chefs[0], NULL);
    return 0;
}
