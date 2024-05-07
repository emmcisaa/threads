// Filename: Q2_1_sem.cpp
// Author: Erin McIsaac

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

sem_t s;                                                 // With this semaphore, runMeFirst runs before the function runMeSecond

void *runMeFirst(void *)                                // thread1 executes this function
{
    cout << "Run me first" << endl;
    sem_post(&s);                                       // Signals thread2 runMeSecond to print
    pthread_exit(NULL);
    }

void *runMeSecond(void *)                               // thread2 executes this function
{
    sem_wait(&s);                                       // Waits for thread1 to signal
    cout << "I run second" << endl;
    pthread_exit(NULL);
}

int main()
{
    sem_init(&s, 0, 0);                                 // Initial value of 0, so thread2 should wait until thread1 signals it
    pthread_t tid1, tid2; //declare thread ids

    pthread_create(&tid1, NULL, runMeFirst, NULL);      // Start thread1
    pthread_create(&tid2, NULL, runMeSecond, NULL);     // Start thread2
    pthread_join(tid1, NULL);                           // Join both threads after their execution
    pthread_join(tid2, NULL);
    
    return 0;
}
