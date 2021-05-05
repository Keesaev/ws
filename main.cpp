#include <QCoreApplication>
#include <iostream>
#include <vector>
#include "sniffer.h"
#include "packetdata.h"
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Sniffer *s = new Sniffer();
    vector<pair<char*, char*>> d;
    if(s->getDevs(d))
        for(auto i : d){
            string a(i.first);
            string b;
            if(i.second != NULL)
                b = i.second;
            else
                b = "No description available";
            cout << a << "\t" << b << "\n";
        }

    s->setDev(d[0].first);
    if(s->initPcap())
        cout << "Device open success\n";
    else
        cout << "Device open failure\n";
    s->startLoopingCapture();
    std::cout << "Stopping\n";


    return a.exec();
}
