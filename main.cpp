#include <QCoreApplication>
#include <iostream>
#include <vector>
#include "stuboutput.h"
#include "packetdata.h"
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    StubOutput *s = new StubOutput();


    return a.exec();
}
