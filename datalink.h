#ifndef DATALINK_H
#define DATALINK_H

#include <QObject>
#include <string>
#include <pcap/pcap.h>
#include <sstream>

#include <vector>
#include <string>

// Временные инклюды
#include <iostream>

using namespace std;

class DataLink : public QObject
{
    Q_OBJECT
public:
    explicit DataLink(QObject *parent = nullptr);

    void deserializeHeader(const u_char *bytes);
    vector<string> getHeaderData();

private:

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

    EthernetHeader ethernetHeader;

    static std::string getMac(const bit8 addr[]){
        std::string s = "";
        for(int i = 0; i < 6; i++){
            s += byteToHexString(addr[i]) + ":";
        }
        s[s.length() - 1] = ' ';
        return s;
    }
    static std::string byteToHexString(unsigned char a);
    static std::string getSingleHexRegister(int b);
signals:

};

#endif // DATALINK_H
