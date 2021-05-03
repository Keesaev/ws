#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <basetransport.h>
#include <vector>
#include <sstream>
#include <string>
#include <pcap/pcap.h>

#include <iostream>

using namespace std;

class Tcp : public BaseTransport
{
    Q_OBJECT

    typedef unsigned char bit8;
    typedef unsigned short bit16;
    typedef unsigned int tcp_seq;

    struct TcpHeader{
        bit16 tcp_sport;   // 16 bits:  Порт отправителя
        bit16 tcp_dport;   // 16 bits: Порт получателя
        tcp_seq tcp_seqn;  // 32 bits: Sequence number (порядковый номер)
        tcp_seq tcp_ack;   // 32 bits: Acknowledgment number (номер подтверждения)
        bit8 tcp_offx2;    // 8 bits: 4 бита Data offset (длина заголовка) от [20 до 60 байт]
        // и 4 бита хз чего
        bit8 tcp_flags;    // 8 bits: Флаги
        bit16 tcp_win;     // 16 bits: Размер окна
        bit16 tcp_sum;     // 16 bits: Контрольная сумма
        bit16 tcp_urp;     // 16 bits: Urgent point (указатель важности)

        static int getOffset(const TcpHeader *tcp){
            return (((tcp)->tcp_offx2 & 0xf0) >> 4);
        }
    };

    TcpHeader tcpHeader;

public:
    Tcp();

    virtual void deserializeHeader(const u_char *bytes, int offset);
    virtual bool isHeaderEmpty();
    virtual vector<string> getHeaderData();
};

#endif // TCP_H
