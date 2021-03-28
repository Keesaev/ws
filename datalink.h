#ifndef DATALINK_H
#define DATALINK_H

#include <QObject>
#include <string>
#include <pcap/pcap.h>
#include <sstream>

// Временные инклюды
#include <iostream>

class DataLink : public QObject
{
    Q_OBJECT
public:
    explicit DataLink(QObject *parent = nullptr);

    typedef unsigned char bit8;
    typedef unsigned short bit16;

    struct EthernetHeader // Длина всегда 14
    {
        bit8 ether_dhost[6];  // 48 bits: Мак-адрес получателя
        bit8 ether_shost[6];  // 48 bits: Мак-адрес отправителя
        bit16 ether_type;     // 16 bits: Тип протокола уровня выше (8 = ip)

        /* Тут инкапсулированный пакет */

        // 4 байта Контрольная сумма
    };

    static std::string getMac(const bit8 addr[]){
        std::string s = "";
        for(int i = 0; i < 6; i++){
            s += byteToHexString(addr[i]) + ":";
        }
        s[s.length() - 1] = ' ';
        return s;
    }

    static void fillHeader(EthernetHeader *header, const u_char* bytes);

    static std::string byteToHexString(unsigned char a);

private:
    static std::string getSingleHexRegister(int b);
signals:

};

#endif // DATALINK_H
