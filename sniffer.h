#ifndef SNIFFER_H
#define SNIFFER_H

#include <QObject>
#include <pcap/pcap.h>
#include <sstream>
#include <iostream>

#include <datalink.h>
#include <network.h>
#include <basetransport.h>
#include <unknowntransport.h>
#include <factory.h>
#include <packetdata.h>

#include <iostream>

using namespace std;

class Sniffer : public QObject
{
    Q_OBJECT
    char *m_dev;
    pcap_t *m_handle;

public:
    explicit Sniffer(QObject *parent = nullptr);
    bool getDevs(std::vector<std::pair<char*, char*>> &d);
    void setDev(char *d);
    bool initPcap();
    void startLoopingCapture(int c);
    void stopLoopingCapture();
    void stopCapture();
signals:
    void packetDeserialized(const PacketData &pd);
private:
    void captureSinglePacket();
};

#endif // SNIFFER_H
