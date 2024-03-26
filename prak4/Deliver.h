#ifndef PRAKT_4_DELIVER_H
#define PRAKT_4_DELIVER_H
#include "Receiver.h"
#include <pthread.h>
#include <random>
#include <algorithm>

static const int MAX_LETTERS = 5;

class Deliver {
private:
    vector<Letter*> letters;
    vector<Receiver*> receivers;
    pthread_mutex_t lettersMutex;
    sem_t deliverySemaphore;
    int receiverCount;
    bool enougCapacity;
    bool usePeriodicCheck;

    void deliveryTimeSimulation() const;
    void deliverToLetterBox();

    void deleteLetters();
    void deleteReceivers();

public:
    bool isEnougCapacity() const;
    Deliver(int n, vector<Receiver*> receivers, bool usePeriodicCheck);
    ~Deliver();

    void work();
    void addOneLetter(Letter* newLetter);
    void notifyDeliveryComplete(); // TODO ist nicht implementiert, brauchst du die Verrell?
};


#endif //PRAKT_4_DELIVER_H
