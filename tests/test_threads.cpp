//  g++ test_threads.cpp -o ../bin/test_threads -lpthread

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#define NUM_OF_THREADS 4

void* (worker) (void*);

int main(int argc, char** argv) {

    DEBUG_MSG("creating threads...");

    pthread_t threads[NUM_OF_THREADS];

    int ret;
    for (int i =0 ; i < NUM_OF_THREADS; i++) {
        ret = pthread_create(&threads[i], nullptr, &worker, nullptr);
        if (ret != 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
    }

    char tmp;
    std::cin >> tmp;   

}


void* worker(void* args) {
    pthread_exit(nullptr);
}


