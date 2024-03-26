#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <queue>

using namespace std;

// Define the maximum capacity of the storage
const int MAX_CAPACITY = 5;

// Structure for the letter
struct Letter {
    string content;
    int receiverID;
};

// Global variables
sem_t logisticsSemaphore;   // Semaphore to control logistics center
sem_t receiverSemaphore;    // Semaphore for notifying receiver
pthread_mutex_t storageMutex; // Mutex to protect the storage
pthread_mutex_t postBoxMutex; //Mutex to protect the postbox
vector<Letter> storage;       // storage for storing letters
vector<queue<Letter>> postBoxes; // Post boxes for receivers

// Logistikzentrum thread
void* logisticsCenter(void* arg) {
    while (true) {
        usleep(rand() % 2000000); // Sleep for random interval

        // Generate a new letter
        Letter newLetter;
        newLetter.content = "I Love BS";
        newLetter.receiverID = rand() % *((int*)arg); // Generate random receiver ID

        // Acquire logisticsSemaphore
        sem_wait(&logisticsSemaphore);

        // Check if the Storage is full
        if (storage.size() >= MAX_CAPACITY) {
            cout << "Storage is full, starting delivery" << endl;

            // Release receiverSemaphore to start delivery
            for (int i = 0; i < MAX_CAPACITY; i++) {
                sem_post(&receiverSemaphore);
            }

            // Wait for the Storage to be empty
            while (!storage.empty()) {
                usleep(100000);
            }

            cout << "Delivery completed. Storage is empty." << endl;
        }

        // Load the letter into the Storage
        pthread_mutex_lock(&storageMutex);
        storage.push_back(newLetter);
        pthread_mutex_unlock(&storageMutex);

        cout << "New letter loaded into the Storage for receiver ID: " << newLetter.receiverID << endl;

        // Release logisticsSemaphore
        sem_post(&logisticsSemaphore);
    }

    pthread_exit(NULL);
}

// Zusteller thread
void* deliveryPerson(void* arg) {
    int deliveryPersonID = *((int*)arg);

    while (true) {
        // Acquire receiverSemaphore
        sem_wait(&receiverSemaphore);

        // Check if the storage is empty
        pthread_mutex_lock(&storageMutex);
        if (storage.empty()) {
            cout << "Truck is empty. Returning to the logistics center." << endl;
            pthread_mutex_unlock(&storageMutex);
            continue;
        }

        // Deliver the letter to the receiver
        Letter letter = storage.back();
        storage.pop_back();
        pthread_mutex_unlock(&storageMutex);

        // Simulate travel time
        usleep(rand() % 1000000);

        cout << "Delivery Person " << deliveryPersonID << " delivered letter with receiver ID: "
             << letter.receiverID << ", Content: " << letter.content << endl;

        // Push the letter into the receiver's post box
        pthread_mutex_lock(&storageMutex);
        postBoxes[letter.receiverID].push(letter);
        pthread_mutex_unlock(&storageMutex);
    }

    pthread_exit(NULL);
}

//Empfänger Thread
void* receiver(void* arg) {
    int receiverID = *((int*)arg);
    while (true) {
        // Sleep for periodic check
        usleep(rand() % 3000000);

        // Check the receiver's post box
        queue<Letter>& postBox = postBoxes[receiverID];

        pthread_mutex_lock(&postBoxMutex);
        if (!postBox.empty()) {
            Letter letter = postBox.front();
            postBox.pop();
            cout << "Receiver " << receiverID << " received letter with receiver ID: "
                 << letter.receiverID << ", Content: " << letter.content << endl;
        }
        pthread_mutex_unlock(&postBoxMutex);
    }
}


int mainBRUH(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: ./brieflogistics <r> <m> <n>" << endl;
        return 1;
    }

    int r = atoi(argv[1]); // Rate of letter generation
    int m = atoi(argv[2]); // Number of delivery persons
    int n = atoi(argv[3]); // Number of receivers

    srand(time(NULL)); // Initialize random seed

    // Initialize semaphores and mutex
    sem_init(&logisticsSemaphore, 0, 1);
    sem_init(&receiverSemaphore, 0, 0);
    pthread_mutex_init(&storageMutex, NULL);

// Create logistics center thread
    pthread_t logisticsThread;
    pthread_create(&logisticsThread, NULL, logisticsCenter, (void *) &n);

// Create delivery person threads
    vector<pthread_t> deliveryThreads(m);
    vector<int> deliveryPersonIDs(m);

    for (int i = 0; i < m; i++) {
        deliveryPersonIDs[i] = i + 1;
        pthread_create(&deliveryThreads[i], NULL, deliveryPerson, (void *) &deliveryPersonIDs[i]);
    }

    // Create receiver threads
    vector<pthread_t> receiverThreads(n);
    vector<int> receiverIDs(n);
    postBoxes.resize(n);

    for (int i = 0; i < n; i++) {
        receiverIDs[i] = i + 1;
        pthread_create(&receiverThreads[i], NULL, receiver, (void*)&receiverIDs[i]);
    }

// Wait for all threads to finish
    pthread_join(logisticsThread, NULL);

    for (int i = 0; i < m; i++) {
        pthread_join(deliveryThreads[i], NULL);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(receiverThreads[i], NULL);
    }

// Destroy semaphores and mutex
    sem_destroy(&logisticsSemaphore);
    sem_destroy(&receiverSemaphore);
    pthread_mutex_destroy(&storageMutex);

    return 0;
}