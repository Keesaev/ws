#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <QObject>
#include <string>
#include <vector>
#include <pcap/pcap.h>

using namespace std;

class PacketData : public QObject
{
    Q_OBJECT
public:
    explicit PacketData(QObject *parent = nullptr);

    timeval timestamp;
    string sourceIp, destIp, protocol, length;
    vector<pair<string, string>> dataLinkFullData;
    vector<pair<string, string>> networkFullData;
    vector<pair<string, string>> transportFullData;

};

#endif // PACKETDATA_H
