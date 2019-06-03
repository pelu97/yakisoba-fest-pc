#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define CLIENTS 5
#define WORKERS 1
#define CHEFS 3
#define COUNTER 10
//delays
#define DELAY_DELIVER_CLIENT 1
#define DELAY_DELIVER_COUNTER 1
#define DELAY_EAT 20
#define DELAY_COOK 3



int Counter[COUNTER];
int CounterFillSlot = 0;
int Line = 0;

pthread_barrier_t barrier_start;

pthread_cond_t cond_chef = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_worker = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_client = PTHREAD_COND_INITIALIZER;


pthread_mutex_t lock_counter = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_line = PTHREAD_MUTEX_INITIALIZER;


void* client(void* arg);
void* worker(void* arg);
void* chef(void* arg);
void eat_dish();
int cook_dish();
int deliver_dish_counter(int dish);
int deliver_dish_client();


void* client(void* arg){
    int id = *((int*) arg);
    printf("Cliente %d criado\n", id);
    pthread_barrier_wait(&barrier_start);
    printf("Cliente %d - START\n", id);

    while(TRUE){
        pthread_mutex_lock(&lock_line);
        Line++;

        printf("Cliente %d - Chegou na fila (%d pessoas na fila)\n", id, Line);
        pthread_cond_broadcast(&cond_worker);   //client chega na fila, acorda workers

        pthread_cond_wait(&cond_client, &lock_line);

        printf("Cliente %d - Comendo prato (%d pessoas na fila)\n", id, Line);
        pthread_mutex_unlock(&lock_line);

        eat_dish();
    }
}

void* worker(void* arg){
    int id = *((int*) arg);
    int amount;
    printf("Worker %d criado\n", id);
    pthread_barrier_wait(&barrier_start);
    printf("Worker %d - START\n", id);


    while(TRUE){

        pthread_mutex_lock(&lock_counter);
        while(CounterFillSlot == 0){
            printf("Worker %d - Balcao vazio, worker dorme\n", id);
            pthread_cond_wait(&cond_worker, &lock_counter);     //balcao vazio, atendente dorme
        }
        pthread_mutex_unlock(&lock_counter);

        pthread_mutex_lock(&lock_line);
        while(Line == 0){
            printf("Worker %d - Fila vazia, worker dorme\n", id);
            pthread_cond_wait(&cond_worker, &lock_line);       //fila vazia, atendente dorme
        }
        pthread_mutex_unlock(&lock_line);


        pthread_mutex_lock(&lock_counter);
        pthread_mutex_lock(&lock_line);

        amount = deliver_dish_client();     //entrega prato para cliente
        pthread_cond_signal(&cond_client);
        Line--;
        if(amount == 2){
            pthread_cond_signal(&cond_client);
            Line--;
        }

        printf("Worker %d - Entrega prato para cliente (%d pratos - %d restantes)\n", id, amount, CounterFillSlot);
        pthread_cond_broadcast(&cond_chef);     //tirou pratos, acorda chefs
        sleep(DELAY_DELIVER_CLIENT);

        pthread_mutex_unlock(&lock_line);
        pthread_mutex_unlock(&lock_counter);

    }
}

void* chef(void* arg){
    int id = *((int*) arg);
    int dish;
    printf("Chef %d criado\n", id);
    pthread_barrier_wait(&barrier_start);
    printf("Chef %d - START\n", id);


    while(TRUE){
        pthread_mutex_lock(&lock_counter);
        while(CounterFillSlot == COUNTER){
            printf("Chef %d - Balcao cheio, chefe dorme\n", id);
            pthread_cond_wait(&cond_chef, &lock_counter);   //balcao cheio, chefe dorme
        }
        pthread_mutex_unlock(&lock_counter);

        if(dish == -1){ //nenhum prato pronto
            dish = cook_dish();  //cozinha um prato
        }
        printf("Chef %d - Prepara prato\n", id);

        pthread_mutex_lock(&lock_counter);
        if(CounterFillSlot != COUNTER){ //checa se o balcao nao encheu enquanto cozinhava - se sim, guarda o prato
            dish = deliver_dish_counter(dish);       //coloca prato no balcao
            printf("Chef %d - Coloca prato no balcao (%d pratos no balcao)\n", id, CounterFillSlot);
            pthread_cond_broadcast(&cond_worker);   //chef colocou pratos, acorda workers
            sleep(DELAY_DELIVER_COUNTER);
        }
        pthread_mutex_unlock(&lock_counter);

    }
}


void eat_dish(){
    sleep(DELAY_EAT);
}

int cook_dish(){
    sleep(DELAY_COOK);
    return rand() % 5;
}

int deliver_dish_counter(int dish){
    Counter[CounterFillSlot] = dish;
    CounterFillSlot++;
    return -1;
}

int deliver_dish_client(){
    int amount = 1;

    Counter[CounterFillSlot-1] = -1;
    CounterFillSlot--;
    if((CounterFillSlot >= 1) && (Line >= 2)){
        Counter[CounterFillSlot-1] = -1;
        CounterFillSlot--;
        amount = 2;
    }

    return amount;
}


int main(){
    pthread_t Clients[CLIENTS], Workers[WORKERS], Chefs[CHEFS];
    int *id, i;

    pthread_barrier_init(&barrier_start, NULL, CLIENTS + WORKERS + CHEFS);

    srand(time(NULL));

    for(i=0;i<CLIENTS;i++){
        //create clients
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&Clients[i], NULL, client, (void*) id);
    }

    for(i=0;i<WORKERS;i++){
        //create workers
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&Workers[i], NULL, worker, (void*) id);
    }

    for(i=0;i<CHEFS;i++){
        //create chefs
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&Chefs[i], NULL, chef, (void*) id);
    }


    pthread_join(Chefs[0], NULL);
    return 0;
}
