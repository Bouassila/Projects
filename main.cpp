#include "Logisticcenter.h"

void printThroughput() {
    double throughput = MESSAGE_SENT_COUNT / MESSAGE_RECEIVED_COUNT;
    cout << "total messages sent:\t\t" << MESSAGE_SENT_COUNT << "\n"
         << "total messages recevied:\t\t" << MESSAGE_RECEIVED_COUNT << "\n"
         << "Throughput:\t\t\t\t\t" << throughput << endl;
}

bool convertStringToBool(const std::string& str) {
    try {
        int num = std::stoi(str);
        return static_cast<bool>(num);
    } catch (const std::exception& e) {
        std::cerr << "Fehler beim Konvertieren: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cout << "Usage: " << argv[0] << " <send_rate> <deliver_amount> <receiver_amount>" << std::endl;
        return 1;
    }

    int rate = std::stoi(argv[1]);
    int deliver_amount = std::stoi(argv[2]);
    int receiver_amount = std::stoi(argv[3]);
    bool usePeriodicCheck = convertStringToBool(argv[4]);

    Logisticcenter* logisticcenter = Logisticcenter::getInstance();
    logisticcenter->configure(rate, deliver_amount, receiver_amount, usePeriodicCheck);

    //TODO Start all Threads
    for (auto receiver : logisticcenter->getReceivers()) {

    }

    for (auto deliver : logisticcenter->getDelivers()) {

    }

    // TODO: bei tastenkombination alle threads stoppen und printen UND aller 50 nachrichten zum beispiel
    printThroughput();

    return 0;
}
