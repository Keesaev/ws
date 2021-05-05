#include "stuboutput.h"

StubOutput::StubOutput(QObject *parent) : QObject(parent)
{
    s = new Sniffer();
    QObject::connect(s, &Sniffer::packetDeserialized,
            this, &StubOutput::onPacketDeserialized);
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
    s->startLoopingCapture(100);
    std::cout << "Stopping\n";
}

void StubOutput::onPacketDeserialized(const PacketData &pd){
    cout << pd.timestamp.tv_sec << "\t" <<
            pd.sourceIp << "\t" <<
            pd.destIp << "\t" <<
            pd.protocol << "\t" <<
            pd.length << "\t\n";
    for(auto i : pd.dataLinkFullData){
        cout << i.first << ":\t\t" << i.second << "\n";
    }
    for(auto i : pd.networkFullData){
        cout << i.first << ":\t\t" << i.second << "\n";
    }
    for(auto i : pd.transportFullData){
        cout << i.first << ":\t\t" << i.second << "\n";
    }
    cout << "\n";
}
