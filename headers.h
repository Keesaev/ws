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
        unsigned char ether_dhost[6];  // Мак-адрес получателя
        // 6 байт
        unsigned char ether_shost[6];  // Мак-адрес отправителя
        // 2 байта
        unsigned short ether_type;     // Тип протокола уровня выше (0800 = ip)

        /* Тут инкапсулированный пакет */

        // 4 байта Контрольная сумма

        static std::string getMac(const unsigned char addr[]){
            std::string s = "";
            for(int i = 0; i < 6; i++){
                s += Headers::byteToHexString(addr[i]) + ":";
            }
            s[s.length() - 1] = ' ';
            return s;
        }
    };
    struct Header_ip{
        // 1 байт
        // 4 бита версия и еще 4 бита длина интернет-заголовка
        unsigned char ip_vhl;  // Version + Header Length
        // 1 байт
        unsigned char ip_tos;  // Type of service (тип обслуживания)
        // 2 байта
        unsigned short ip_len; // Длина пакета
        // 2 байта
        unsigned short ip_id;  // Идентификатор пакета
        // 2 байта
        // 3 бита флаги (не смотрим)
        unsigned short ip_off; // 13 бит - смещение фрагмента
        // 1 байт
        unsigned char ip_ttl;  // Time to live (число хопов)
        // 1 байт
        unsigned char ip_p;    // Тип протокола (TCP, UDP, ICMP)
        // 2 байта
        unsigned short ip_sum; // Контрольная сумма
        // по 4 байта адреса источника и назначения
        struct in_addr ip_src, ip_dst;
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
        // 2 байта
        unsigned short th_sport;   // Порт отправителя
        // 2 байта
        unsigned short th_dport;   // Порт получателя
        // 4 байта
        tcp_seq th_seq;     // Sequence number (порядковый номер)
        // 4 байта
        tcp_seq th_ack;     // Acknowledgment number (номер подтверждения)
        // 1 байт
        unsigned char th_offx2;    // 4 бита Data offset (длина заголовка) от [20 до 60 байт]
        // и 4 бита хз чего

        // 1 байт
        unsigned char th_flags;    // Флаги
        // 2 байта
        unsigned short th_win;     // Размер окна
        // 2 байта
        unsigned short th_sum;     // Контрольная сумма
        // 2 байта
        unsigned short th_urp;     // Urgent point (указатель важности)

        static int getOffset(const Header_tcp *tcp){
            return (((tcp)->th_offx2 & 0xf0) >> 4);
        }
    };

    static std::string byteToHexString(unsigned char a);

private:
    static std::string getSingleHexRegister(int b);
signals:

};

#endif // HEADERS_H
