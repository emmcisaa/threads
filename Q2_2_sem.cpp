// Filename: Q2_2_sem.cpp
// Author: Erin McIsaac

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

sem_t s1;                                               // With this semaphore, runMeFirst runs before runMeSecond
sem_t s2;                                               // With this semaphore, runMeSecond runs before runMeThird

void *runMeFirst(void *)                                // thread1 will execute this function
{
    cout << "Run me first" << endl;
    sem_post(&s1);                                      // Signals thread2 runMeSecond to print
    pthread_exit(NULL);
}

void *runMeSecond(void *)                               // thread2 will execute this function
{
    sem_wait(&s1);                                      // Waits for thread1 to signal
    cout << "I run second" << endl;
    sem_post(&s2);                                      // Signals thread3 runMeThird to print
    pthread_exit(NULL);
}

void *runMeThird(void *)                                // thread 3 executes this function
{
    sem_wait(&s2); //waits for thread 2 to signal it
    cout << "I run third" << endl;
    pthread_exit(NULL);
}

int main()
{
    sem_init(&s1, 0, 0);                                // Initial value of 0, so thread2 should wait until thread1 signals it
    sem_init(&s2, 0, 0);                                // Initial value of 0, so thread3 should wait until thread2 signals it
    pthread_t tid1, tid2, tid3;                         // Declare thread ids

    pthread_create(&tid1, NULL, runMeFirst, NULL);      // Start thread1
    pthread_create(&tid2, NULL, runMeSecond, NULL);     // Start thread2
    pthread_create(&tid3, NULL, runMeThird, NULL);      // Start thread3
    pthread_join(tid1, NULL);                           // Join all threads after their execution
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    
    return 0;
}
