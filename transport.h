#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <pcap/pcap.h>
#include <string>
#include <sstream>

#include <iostream>

class Transport : public QObject
{
    Q_OBJECT
public:
    explicit Transport(QObject *parent = nullptr);

    typedef unsigned char bit8;
    typedef unsigned short bit16;
    typedef unsigned int tcp_seq;

    struct BaseTransportHeader{

    };

    struct TcpHeader : BaseTransportHeader{
        bit16 tcp_sport;   // 16 bits:  Порт отправителя
        bit16 tcp_dport;   // 16 bits: Порт получателя
        tcp_seq tcp_seqn;            // 32 bits: Sequence number (порядковый номер)
        tcp_seq tcp_ack;            // 32 bits: Acknowledgment number (номер подтверждения)
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

    struct UdpHeader : BaseTransportHeader{
        bit16 udp_sport;        // 16 bits: Source port
        bit16 udp_dport;        // 16 bits: Destination port
        bit16 udp_len;          // 16 bits: Длина сообщения
        bit16 udp_chksum;       // 16 bits: Контрольная сумма
    };

    struct IcmpHeader : BaseTransportHeader{
        bit8 icmp_type;         // 8 bits: Тип
        bit8 icmp_code;         // 8 bits: Код
        bit16 icmp_sum;         // 16 bits: Контрольная сумма
        uint32_t icmp_rest;     // 32 bits: Зависит от типа и кода
    };

    static void fillHeader(const u_char* bytes, BaseTransportHeader *header, bit8 protocol, int offset);
    static void fillTcp(const u_char* bytes, TcpHeader *header, int offset);
    static void fillUdp(const u_char* bytes, UdpHeader *header, int offset);
    static void fillIcmp(const u_char* bytes, IcmpHeader *header, int offset);
signals:

};

#endif // TRANSPORT_H
