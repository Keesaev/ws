#ifndef FACTORY_H
#define FACTORY_H

#include <QObject>
#include <basetransport.h>
#include <icmp.h>
#include <tcp.h>
#include <udp.h>
#include <emptytransport.h>

class Factory : public QObject
{
    Q_OBJECT
public:
    explicit Factory(QObject *parent = nullptr);

    static BaseTransport* makeTransport(int protocol);

signals:

};

#endif // FACTORY_H
