#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <basetransport.h>
#include <vector>
#include <string>
#include <sstream>

#include <iostream>

using namespace std;

class Udp : public BaseTransport
{
    Q_OBJECT

    typedef unsigned short bit16;

    struct UdpHeader{
        bit16 udp_sport;        // 16 bits: Source port
        bit16 udp_dport;        // 16 bits: Destination port
        bit16 udp_len;          // 16 bits: Длина сообщения
        bit16 udp_chksum;       // 16 bits: Контрольная сумма
    };

    UdpHeader udpHeader;

public:
    Udp();

    virtual void deserializeHeader(const u_char *bytes, int offset);
    virtual bool isHeaderEmpty();
    virtual vector<string> getHeaderData();
};

#endif // UDP_H