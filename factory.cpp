#include "factory.h"

Factory::Factory(QObject *parent) : QObject(parent)
{

}

BaseTransport* Factory::makeTransport(int protocol){
    BaseTransport* transport;
    switch (protocol) {
    case 1:
        cout << "ICMP\n";
        transport = new Icmp();
        break;
    case 6:
        cout << "TCP\n";
        //transport = new Tcp();
        transport = new EmptyTransport();
        break;
    case 17:
        cout << "UDP\n";
        transport = new Udp();
        break;
    default:
        transport = new EmptyTransport();
        break;
    }
    return transport;
}
