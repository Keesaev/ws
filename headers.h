#ifndef HEADERS_H
#define HEADERS_H

#include <QObject>
#include <pcap/pcap.h>

class Headers : public QObject
{
    Q_OBJECT
public:
    explicit Headers(QObject *parent = nullptr);

    struct Header_ethernet // Длина всегда 14
    {
        // 6 байт
        u_char ether_dhost[6];  // Мак-адрес получателя
        // 6 байт
        u_char ether_shost[6];  // Мак-адрес отправителя
        // 2 байта
        u_short ether_type;     // Тип протокола уровня выше (0800 = ip)

        /* Тут инкапсулированный пакет */

        // 4 байта
        u_char ether_fcs[4];    // Контрольная сумма
    };
    struct Header_ip{
        // 1 байт
        // 4 бита версия и еще 4 бита длина интернет-заголовка
        u_char ip_vhl;  // Версия (IPv4 или IPv6)
        // 1 байт
        u_char ip_tos;  // Type of service (тип обслуживания)
        // 2 байта
        u_short ip_len; // Длина пакета
        // 2 байта
        u_short ip_id;  // Идентификатор пакета
        // 2 байта
        // 3 бита флаги (не смотрим)
        u_short ip_off; // 13 бит - смещение фрагмента
        // 1 байт
        u_char ip_ttl;  // Time to live (число хопов)
        // 1 байт
        u_char ip_p;    // Тип протокола (TCP, UDP, ICMP)
        // 2 байта
        u_short ip_sum; // Контрольная сумма
        // по 4 байта адреса источника и назначения
        struct in_addr ip_src, ip_dst;
        // Далее рандомное число байт для флагов

        /*
        #define IP_RF 0x8000
        #define IP_DF 0x4000
        #define IP_MF 0x2000
        #define IP_OFFMASK 0x1fff
        */
    };

    typedef u_int tcp_seq;

    struct Header_tcp{
        // 2 байта
        u_short th_sport;   // Порт отправителя
        // 2 байта
        u_short th_dport;   // Порт получателя
        // 4 байта
        tcp_seq th_seq;     // Sequence number (порядковый номер)
        // 4 байта
        tcp_seq th_ack;     // Acknowledgment number (номер подтверждения)
        // 1 байт
        u_char th_offx2;    // 4 бита Data offset (длина заголовка) от [20 до 60 байт]
        // и 4 бита хз чего

        // 1 байт
        u_char th_flags;    // Флаги
        // 2 байта
        u_short th_win;     // Размер окна
        // 2 байта
        u_short th_sum;     // Контрольная сумма
        // 2 байта
        u_short th_urp;     // Urgent point (указатель важности)

        /*
        #define TH_FIN 0x01
        #define TH_SYN 0x02
        #define TH_RST 0x04
        #define TH_PUSH 0x08
        #define TH_ACK 0x10
        #define TH_URG 0x20
        #define TH_ECE 0x40
        #define TH_CWR 0x80
        #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        */
    };

    /*
     *
     */
    static u_int th_off(const Header_tcp *tcp);

    /* Этот метод нужен для получения длины заголовка IP,
     *  так как он занимает половину переменной длиной 8 бит*/

    static u_int ip_hl(const Header_ip *ip);

    static u_int ip_v(const Header_ip *ip);

signals:

};

#endif // HEADERS_H
