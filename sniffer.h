#ifndef SNIFFER_H
#define SNIFFER_H

#include <QObject>
#include <pcap/pcap.h>
#include <sstream>
#include <iostream>

#include <datalink.h>
#include <network.h>
#include <stuboutput.h>
#include <basetransport.h>
#include <emptytransport.h>
#include <factory.h>

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
    void captureSinglePacket();
    void startLoopingCapture();
    void stopLoopingCapture();
    void stopCapture();
private:
    void static handlePacket(u_char *user, const struct pcap_pkthdr *header,
                      const u_char *bytes);
signals:

};

#endif // SNIFFER_H
