#ifndef STUBOUTPUT_H
#define STUBOUTPUT_H

#include "datalink.h"
#include "network.h"
#include <packetdata.h>

#include <QObject>
#include <iostream>

using namespace std;

class StubOutput : public QObject
{
    Q_OBJECT
public:
    explicit StubOutput(QObject *parent = nullptr);

public slots:
    void onPacketDeserialized(const PacketData &pd);
};

#endif // STUBOUTPUT_H
