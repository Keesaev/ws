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

// ВРЕМЕННО
private:

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
        // Далее рандомное число байт для флагов
    };

    IpHeader ipHeader;

    std::string getAddress(in_addr addr){
        char cAddr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr), cAddr, INET_ADDRSTRLEN);
        return cAddr;
    }

    // Читаем правые 4 бита поля ip_vhl
    int getVersion(){
        return ((ipHeader.ip_vhl) >> 4);
    }

    int getPriority(){
        return ((ipHeader.ip_tos) >> 5);
    }

    // 3 бита ToS
    std::string getTos(){
        std::string s = "";
        for(int i = 128; i >= 1; i /= 2){
            if((ipHeader.ip_tos) & i)
                s += "1";
            else
                s += "0";
        }
        return s;
    }

    // Берем 2 байта без левых 3 битов
    int getOffset(){
        return (ntohs(ipHeader.ip_off) & 8191);
    }

    // 3 левых вита из поля off
    std::string getFlags(){
        // ТУТ ПИЗДА --------------------------------------------------------------------------
        std::string s = "";
        for(int i = 32768; i >= 8192; i /= 2){
            if(ntohs(ipHeader.ip_off) & i)
                s += "1";
            else
                s += "0";
        }
        return s;
    }

public:
    explicit Network(QObject *parent = nullptr);

    vector<string> getHeaderData();
    void deserializeHeader(const u_char* bytes);
    bool isHeaderEmpty();

    // Читаем левые 4 бита поля ip_vhl
    int getHeaderLength(){
        return (((ipHeader).ip_vhl) & 0x0f);
    }

    int getProtocol(){
        return ipHeader.ip_p;
    }

    int getTotalLength(){
        return ipHeader.ip_len;
    }

signals:

};

#endif // NETWORK_H
