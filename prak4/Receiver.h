#ifndef PRAKT_4_RECEIVER_H
#define PRAKT_4_RECEIVER_H
#include "Letter.h"
#include <vector>
#include <unistd.h>
#include <semaphore.h>

using std::cout;
using std::endl;
using std::vector;

static int MESSAGE_RECEIVED_COUNT = 0;

class Receiver {
public:
    Receiver(int id, bool usePeriodicCheck);
    ~Receiver();
    int getID();
    void addLetterToLetterBox(Letter*);
    void printLetter(vector<Letter*> letters);
    void checkLetterBoxPeriodically();
    void takeOutLetters();
    void work();

private:
    int id;
    bool usePeriodicCheck;
    sem_t receiverSemaphore;
    pthread_mutex_t letterBoxMutex;
    vector<Letter*> letterBox;

    void deleteLetterBox();
public:
    const vector<Letter *> &getLetterBox() const;

};


#endif //PRAKT_4_RECEIVER_H
