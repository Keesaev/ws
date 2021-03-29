#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <pcap/pcap.h>
#include <sstream>
#include <istream>
#include <string>
#include <algorithm>

#include <iostream>

using namespace std;

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);

    typedef unsigned char bit8;
    typedef unsigned short bit16;

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

    static std::string getAddress(in_addr addr){
        char cAddr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr), cAddr, INET_ADDRSTRLEN);
        return cAddr;
    }

    // Читаем левые 4 бита поля ip_vhl
    static int getHeaderLength(const IpHeader *ip){
        return (((ip)->ip_vhl) & 0x0f);
    }

    // Читаем правые 4 бита поля ip_vhl
    static int getVersion(const IpHeader *ip){
        return (((ip)->ip_vhl) >> 4);
    }

    static int getPriority(const IpHeader *ip){
        return (((ip)->ip_tos) >> 5);
    }

    static std::string getDTR(const IpHeader *ip){
        std::string s = "";
        for(int i = 16; i >= 4; i /= 2){
            if(((ip)->ip_tos) & i)
                s += "1";
            else
                s += "0";
        }
        return s;
    }

    static bool fillHeader(IpHeader *header, const unsigned char* bytes);

    friend std::istream& operator>> (std::istream &stream, bit16 &data){
        stream.read(reinterpret_cast<char*>(&data), sizeof(bit16));
        return stream;
    }
private:

signals:

};

#endif // NETWORK_H
