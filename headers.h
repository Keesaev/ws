#ifndef HEADERS_H
#define HEADERS_H

#include <QObject>
#include <pcap/pcap.h>

class Headers : public QObject
{
    Q_OBJECT
public:

    typedef unsigned char bit8;
    typedef unsigned short bit16;

    explicit Headers(QObject *parent = nullptr);

    struct Header_ethernet // Длина всегда 14
    {
        bit8 ether_dhost[6];  // 48 bits: Мак-адрес получателя
        bit8 ether_shost[6];  // 48 bits: Мак-адрес отправителя
        bit8 ether_type;     // 16 bits: Тип протокола уровня выше (8 = ip)

        /* Тут инкапсулированный пакет */

        // 4 байта Контрольная сумма

        static std::string getMac(const bit8 addr[]){
            std::string s = "";
            for(int i = 0; i < 6; i++){
                s += Headers::byteToHexString(addr[i]) + ":";
            }
            s[s.length() - 1] = ' ';
            return s;
        }
    };
    struct Header_ip{
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

        static std::string getAddress(in_addr addr){
            char cAddr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr), cAddr, INET_ADDRSTRLEN);
            return cAddr;
        }

        /* Этот метод нужен для получения длины заголовка IP,
         *  так как он занимает половину переменной длиной 8 бит*/
        static int getHeaderLength(const Header_ip *ip){
            return (((ip)->ip_vhl) & 0x0f);
        }

        static int getVersion(const Header_ip *ip){
            return (((ip)->ip_vhl) >> 4);
        }

        static int getPriority(const Header_ip *ip){
            return (((ip)->ip_tos) >> 5);
        }

        static std::string getDTR(const Header_ip *ip){
            std::string s = "";
            for(int i = 16; i >= 4; i /= 2){
                if(((ip)->ip_tos) & i)
                    s += "1";
                else
                    s += "0";
            }
            return s;
        }
    };

    typedef unsigned int tcp_seq;

    struct Header_tcp{
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

        static int getOffset(const Header_tcp *tcp){
            return (((tcp)->th_offx2 & 0xf0) >> 4);
        }
    };

    struct Header_udp{
        bit16 udp_sport;        // 16 bits: Source port
        bit16 udp_dport;        // 16 bits: Destination port
        bit16 udp_len;          // 16 bits: Длина сообщения
        bit16 udp_chksum;       // 16 bits: Контрольная сумма
    };

    static std::string byteToHexString(unsigned char a);

private:
    static std::string getSingleHexRegister(int b);
signals:

};

#endif // HEADERS_H
