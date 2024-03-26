#ifndef PRAKT_4_LETTER_H
#define PRAKT_4_LETTER_H
#include <iostream>

using std::string;


class Letter {
public:
    Letter(std::string, int);
    std::string getContent();
    int getReceiverID();

private :
    std::string content;
    int receiverID;
};


#endif //PRAKT_4_LETTER_H
