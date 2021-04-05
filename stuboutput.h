#ifndef STUBOUTPUT_H
#define STUBOUTPUT_H

#include <QObject>
#include "datalink.h"
#include "network.h"
#include <iostream>

class StubOutput : public QObject
{
    Q_OBJECT
public:
    explicit StubOutput(QObject *parent = nullptr);

signals:

};

#endif // STUBOUTPUT_H
