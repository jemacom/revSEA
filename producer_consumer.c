#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define in 10
#define out 100

int inputArray[in] , outputArray[out];
int inC=0 , ouC=0;
int no=0;

pthread_t pro_thread , con_thread[3] , dis_thread;
pthread_mutex_t Lock;
sem_t full,empty;

void merge(int item){
    if(no==0){
        outputArray[0]=item;
    }else{
        int low=0 , high=no-1;
        int mid,i;

        while(low <= high){
            mid=(low+high)/2;
            if(item==outputArray[mid]){
                high = mid+1;
            }
            else if(item<outputArray[mid]){
                high=mid-1;
            } 
            else{
                low=mid+1;
            }
        }
        
        if (item < outputArray[mid]){
                for (i=no-1; i >= mid; i--){
                    outputArray[i+1]=outputArray[i];
                }
                outputArray[mid+1]=item;
         }
        else
        {
            for (i=no-1; i>mid; i--){
                outputArray[i+1] = outputArray[i];
            }
            outputArray[mid+1]=item;
        }
}
no++;
}

void * dis_func(void* var){
    int i;
    while(1){
        printf("\n-------------------------------------------------------\n");
        for (i=0; i<no; i++){
            printf("%d|", outputArray[i]);
        }
        sleep(2);
    }
    return 0;
}

void* pro_func(void* var){
    int item;
    while(1){
        item = (rand()%100)+1;
        sem_wait(&empty);
        pthread_mutex_lock(&Lock);
        if (inC < in){
            inputArray[inC]=item;
            inC++;
        }
        pthread_mutex_unlock(&Lock);
        sem_post(&full);
        sleep(1);
    }
    return 0;
}

void * con_func(void * var){
    int item;
    while(1){
        sem_wait(&full);
        pthread_mutex_lock(&Lock);

        if(inC > 0){
            item = inputArray[inC-1];
            inC--;
            merge(item);
        }

        pthread_mutex_unlock(&Lock);
        sem_post(&full);
        sleep(1);
    }
    return 0;
}


int main(){
    pthread_mutex_init(&Lock , NULL);
    sem_init(&full , 0 , 0);
    sem_init(&empty , 0 , 10);

    pthread_create(&pro_thread, NULL , pro_func , NULL);
    pthread_create(&con_thread[0], NULL , con_func , NULL);
    pthread_create(&con_thread[1], NULL , con_func , NULL);
    pthread_create(&con_thread[2], NULL , con_func , NULL);
    pthread_create(&dis_thread, NULL , dis_func , NULL);

    pthread_exit(NULL);

    return 1;
}

