#include "Logisticcenter.h"

bool Logisticcenter::instanceFlag = false;
Logisticcenter* Logisticcenter::theLogisticcenter = nullptr;


Logisticcenter::Logisticcenter(){}

Logisticcenter::~Logisticcenter(){
    for (auto receiver : receivers)
        delete receiver;

    receivers.clear();

    for (auto deliver : delivers)
        delete deliver;

    delivers.clear();

    instanceFlag = false;
    delete theLogisticcenter;
}

Logisticcenter* Logisticcenter::getInstance() {
    if (!instanceFlag)
        initializeLogisticcenter();

    return theLogisticcenter;
}

void Logisticcenter::initializeLogisticcenter() {
    theLogisticcenter = new Logisticcenter();
    instanceFlag = true;
}

void Logisticcenter::configure(int rate, int deliver_count, int receiver_count, bool usePeriodicCheck) {
    this->sendRate = rate;
    this->deliverThreads = deliver_count;
    this->receiverThreads = receiver_count;
    this->usePeriodicCheck = usePeriodicCheck;
    initVectors();
}

void Logisticcenter::initVectors(){
    for(size_t n = 0; n < receiverThreads; n++)
        receivers.push_back(new Receiver(n + 1, usePeriodicCheck));

    for(size_t n = 0; n < deliverThreads; n++)
        delivers.push_back(new Deliver(receivers.size(), receivers, usePeriodicCheck));
}

void Logisticcenter::generateLetter() {
    while(true)
        for (auto deliver : delivers)
            if(deliver->isEnougCapacity()) {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::string possibleCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            std::uniform_int_distribution<> charDistribution(0, possibleCharacters.size() - 1);

            std::string content;
            for (int i = 0; i < 10; i++)
                content += possibleCharacters[charDistribution(gen)];

            std::uniform_int_distribution<> receiverIDDistribution(1, receiverThreads);
            int receiverID = receiverIDDistribution(gen);

            Letter* newLetter = new Letter(content, receiverID);
            deliver->addOneLetter(newLetter);
            MESSAGE_SENT_COUNT++;
            usleep(sendRate);
            }
}

const vector<Deliver *> &Logisticcenter::getDelivers() const {
    return delivers;
}

const vector<Receiver *> &Logisticcenter::getReceivers() const {
    return receivers;
}

// TODO Durchsatz