#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

bool Network::fillHeader(IpHeader *header, const unsigned char *bytes){
    return true;
}
