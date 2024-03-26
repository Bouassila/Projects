#include "Letter.h"
Letter::Letter(std::string content, int receiverID) : content(content), receiverID(receiverID){}

std::string Letter::getContent() {
    return content;
}

int Letter::getReceiverID() {
    return receiverID;
}