#include "Deliver.h"

Deliver::Deliver(int n, vector<Receiver*> receivers, bool usePeriodicCheck) : receiverCount(n), enougCapacity(true), usePeriodicCheck(usePeriodicCheck){
    for(auto receiver : receivers)
        if(this->receivers.size() < receiverCount)
            this->receivers.push_back(receiver);

    pthread_mutex_init(&lettersMutex, NULL);
    sem_init(&deliverySemaphore, 0, 0);
}

Deliver::~Deliver(){
    pthread_mutex_destroy(&lettersMutex);

    deleteReceivers();
    deleteLetters();
}

void Deliver::addOneLetter(Letter* newLetter) {
    pthread_mutex_lock(&lettersMutex); //lock the critical section
    if (letters.size() < MAX_LETTERS - 1)
        letters.push_back(newLetter);

    else {
        letters.push_back(newLetter);
        enougCapacity = false;
        sem_post(&deliverySemaphore);
        pthread_mutex_unlock(&lettersMutex); //unlock the critical section
    }
}

void Deliver::work() {
    while(true) {
        sem_wait(&deliverySemaphore);   // Wait for signal from the deliverySemaphore
        while(!enougCapacity) {
            pthread_mutex_lock(&lettersMutex); // Lock the critical section
            deliverToLetterBox();
        }
    }
}

void Deliver::deliverToLetterBox() {
    for (auto letter: letters)
        for (size_t i = 0; i < receivers.size(); ++i)
            if (letter->getReceiverID() == receivers[i]->getID()) {
                deliveryTimeSimulation();
                receivers[i]->addLetterToLetterBox(letter);
                receivers[i]->work();
                if(!usePeriodicCheck)
                    while(!receivers[i]->getLetterBox().empty())
                        usleep(300000);
            }

    deleteLetters();
    enougCapacity = true;
    pthread_mutex_unlock(&lettersMutex); // Unlock the critical section
}

void Deliver::deliveryTimeSimulation() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(500000,2000000);
    usleep(dist(gen));
}

void Deliver::deleteLetters() {
    for(auto letter : letters)
        delete letter;

    letters.clear();
}

void Deliver::deleteReceivers() {
    for (auto receiver : receivers)
        delete receiver;

    receivers.clear();
}

bool Deliver::isEnougCapacity() const {
    return enougCapacity;
}
