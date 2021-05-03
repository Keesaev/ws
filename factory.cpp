#include "factory.h"

Factory::Factory(QObject *parent) : QObject(parent)
{

}

BaseTransport* Factory::makeTransport(int protocol){
    BaseTransport* transport;
    switch (protocol) {
    case 1:
        transport = new Icmp();
        break;
    case 6:
        std::cout << "TCP HEADER:\n";
        //transport = new Tcp();
        transport = new EmptyTransport();
        break;
    case 17:
        transport = new Udp();
        break;
    default:
        transport = new EmptyTransport();
        break;
    }
    return transport;
}
