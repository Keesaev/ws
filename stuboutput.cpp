#include "stuboutput.h"

StubOutput::StubOutput(QObject *parent) : QObject(parent)
{

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
