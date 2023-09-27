#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <semaphore.h>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
sem_t forks[NUM_PHILOSOPHERS];
mutex printMutex;

void Think(int i)
{
    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " is thinking." << endl;
    }
    this_thread::sleep_for(chrono::seconds(2));
}

void PickupForks(int i)
{
    int leftFork = i;
    int rightFork = (i + 1) % NUM_PHILOSOPHERS;

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " is trying to pick up fork " << leftFork << "." << endl;
    }
    sem_wait(&forks[leftFork]);

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " picked up fork " << leftFork << "." << endl;
    }

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " is trying to pick up fork " << rightFork << "." << endl;
    }
    sem_wait(&forks[rightFork]);

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " picked up fork " << rightFork << "." << endl;
    }
}

void Eat(int i)
{
    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " is eating." << endl;
    }
    this_thread::sleep_for(chrono::seconds(3));
}

void PutForks(int i)
{
    int leftFork = i;
    int rightFork = (i + 1) % NUM_PHILOSOPHERS;

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " put down fork " << leftFork << "." << endl;
    }
    sem_post(&forks[leftFork]);

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Philosopher " << i << " put down fork " << rightFork << "." << endl;
    }
    sem_post(&forks[rightFork]);
}

void PhilosopherProcess(int i)
{
    Think(i);
    PickupForks(i);
    Eat(i);
    PutForks(i);
}

int main()
{
    thread philosophers[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        sem_init(&forks[i], 0, 1); // Initialize forks to be available (semaphore value 1)
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        philosophers[i] = thread(PhilosopherProcess, i);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        philosophers[i].join();
    }

    {
        lock_guard<mutex> lock(printMutex);
        cout << "All philosophers are nourished." << endl;
    }

    return 0;
}
