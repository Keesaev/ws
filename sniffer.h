#ifndef SNIFFER_H
#define SNIFFER_H

#include <QObject>
#include <pcap/pcap.h>
#include <QDebug>
#include <vector>

class Sniffer : public QObject
{
    Q_OBJECT
    char *m_dev;
public:
    explicit Sniffer(QObject *parent = nullptr);
    bool getDevs(std::vector<std::pair<char*, char*>> &d);
    void setDev(char *d);
    void initPcap();
    void startCapture();
    void stopCapture();
signals:

};

#endif // SNIFFER_H
