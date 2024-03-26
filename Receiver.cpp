#include "Receiver.h"

Receiver::Receiver(int id, bool usePeriodicCheck): id(id), usePeriodicCheck(usePeriodicCheck) {
    pthread_mutex_init(&letterBoxMutex, NULL);
    sem_init(&receiverSemaphore, 0, 0);
}

Receiver::~Receiver(){
    deleteLetterBox();
}

void Receiver::addLetterToLetterBox(Letter* newletter) {
    pthread_mutex_lock(&letterBoxMutex); //lock the critical section
    letterBox.push_back(newletter);
    pthread_mutex_unlock(&letterBoxMutex); //unlock the critical section
    sem_post(&receiverSemaphore); // Increment the semaphore count to notify waiting receiver
}

void Receiver::work() {
    if(usePeriodicCheck)
        checkLetterBoxPeriodically();
    else
        takeOutLetters();
}

void Receiver::checkLetterBoxPeriodically() {
    while (true) {
        usleep(rand() % 2000000);

        if (!letterBox.empty())
            printLetter(letterBox);
    }
}

void Receiver::takeOutLetters() {
    while(true){
        pthread_mutex_lock(&letterBoxMutex); //lock the critical section
        sem_wait(&receiverSemaphore);
        printLetter(letterBox);
        pthread_mutex_unlock(&letterBoxMutex); //unlock the critical section
    }
}

void Receiver::printLetter(vector<Letter*> letters){
    for(auto letter : letters) {
        cout << "Receiver_" << this->id << ": " << letter->getContent() << endl;
        MESSAGE_RECEIVED_COUNT++;
    }

    deleteLetterBox();
}

void Receiver::deleteLetterBox() {
    for(auto letter : this->letterBox)
        delete letter;

    this->letterBox.clear();
}

int Receiver::getID() {
    return id;
}

const vector<Letter *> &Receiver::getLetterBox() const {
    return letterBox;
}


