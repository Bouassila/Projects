#ifndef PRAKT_4_LOGISTICCENTER_H
#define PRAKT_4_LOGISTICCENTER_H
#include "Deliver.h"

static int MESSAGE_SENT_COUNT = 0;

class Logisticcenter {
private:
    static bool instanceFlag;
    static Logisticcenter* theLogisticcenter;

    bool usePeriodicCheck;
    int deliverThreads, receiverThreads, sendRate;
    vector<Deliver*> delivers;
    vector<Receiver*> receivers;

    Logisticcenter();
    ~Logisticcenter();
    static void initializeLogisticcenter();
    void initVectors();

public:
    static Logisticcenter* getInstance();
    void configure(int rate, int deliver_count, int receiver_count, bool usePeriodicCheck);
    void generateLetter();

    const vector<Deliver *> &getDelivers() const;
    const vector<Receiver *> &getReceivers() const;
};

#endif //PRAKT_4_LOGISTICCENTER_H
