#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <pcap/pcap.h>

class Transport : public QObject
{
    Q_OBJECT
public:
    explicit Transport(QObject *parent = nullptr);

    typedef unsigned char bit8;
    typedef unsigned short bit16;
    typedef unsigned int tcp_seq;

    struct TcpHeader{
        bit16 th_sport;   // 16 bits:  Порт отправителя
        bit16 th_dport;   // 16 bits: Порт получателя
        tcp_seq th_seq;            // 32 bits: Sequence number (порядковый номер)
        tcp_seq th_ack;            // 32 bits: Acknowledgment number (номер подтверждения)
        bit8 th_offx2;    // 8 bits: 4 бита Data offset (длина заголовка) от [20 до 60 байт]
        // и 4 бита хз чего
        bit8 th_flags;    // 8 bits: Флаги
        bit16 th_win;     // 16 bits: Размер окна
        bit16 th_sum;     // 16 bits: Контрольная сумма
        bit16 th_urp;     // 16 bits: Urgent point (указатель важности)

        static int getOffset(const TcpHeader *tcp){
            return (((tcp)->th_offx2 & 0xf0) >> 4);
        }
    };

    struct UdpHeader{
        bit16 udp_sport;        // 16 bits: Source port
        bit16 udp_dport;        // 16 bits: Destination port
        bit16 udp_len;          // 16 bits: Длина сообщения
        bit16 udp_chksum;       // 16 bits: Контрольная сумма
    };

signals:

};

#endif // TRANSPORT_H
