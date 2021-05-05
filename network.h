#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <pcap/pcap.h>
#include <sstream>
#include <istream>
#include <string>
#include <vector>

#include <iostream>

using namespace std;

class Network : public QObject
{
    Q_OBJECT

    typedef unsigned char bit8;
    typedef unsigned short bit16;
    const int ipMaxHeaderSize = 60;
    const int ipMinHeaderSize = 20;
    const int ethernetHeaderSize = 14;

    // https://en.wikipedia.org/wiki/IPv4#Header

    struct IpHeader{
        bit8 ip_vhl;  // 8 bits:   Version (4 bits) + Header Length (4 bits)
        bit8 ip_tos;  // 8 bits:   Type of service (тип обслуживания)
        bit16 ip_len; // 16 bits:   Длина пакета
        bit16 ip_id;  // 16 bits:   Идентификатор пакета
        bit16 ip_off; // 16 bits:    Flags (3 bits) + смещение фрагмента (13 bits)
        bit8 ip_ttl;  // 8 bits:     Time to live (число хопов)
        bit8 ip_p;    // 8 bits:   Тип протокола (TCP, UDP, ICMP)
        bit16 ip_sum; // 16 bits:  Контрольная сумма
        struct in_addr ip_src, ip_dst; // 32 bits each
    };

    IpHeader ipHeader;

    std::string getAddress(in_addr addr);
    std::string getTos();   // 3 бита ToS   TODO
    std::string getFlags(); // TODO

    int getVersion(){       // Читаем правые 4 бита поля ip_vhl
        return ((ipHeader.ip_vhl) >> 4);
    }
    int getPriority(){
        return ((ipHeader.ip_tos) >> 5);
    }
    int getOffset(){        // Берем всё кроме левых 3 битов в ip_off
        return (ntohs(ipHeader.ip_off) & 8191);
    }

public:
    explicit Network(QObject *parent = nullptr);

    vector<pair<string, string>> getHeaderData();
    void deserializeHeader(const u_char* bytes);
    bool isHeaderEmpty();

    // Читаем левые 4 бита поля ip_vhl
    int getHeaderSize(){
        return (((ipHeader).ip_vhl) & 0x0f) * 4;
    }

    int getProtocol(){
        return ipHeader.ip_p;
    }

    string getSourceIp(){
        return getAddress(ipHeader.ip_src);
    }

    string getDestIp(){
        return getAddress(ipHeader.ip_dst);
    }

signals:

};

#endif // NETWORK_H
