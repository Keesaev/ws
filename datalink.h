#ifndef DATALINK_H
#define DATALINK_H

#include <QObject>
#include <string>
#include <pcap/pcap.h>
#include <sstream>

#include <vector>
#include <string>
#include <map>

using namespace std;

class DataLink : public QObject
{
    Q_OBJECT

    typedef unsigned char bit8;
    typedef unsigned short bit16;
    const int ethernetHeaderSize = 14;
    map<int, string> hexLetters = {
        {10, "a"},
        {11, "b"},
        {12, "c"},
        {13, "d"},
        {14, "e"},
        {15, "f"},
    };

    struct EthernetHeader
    {
        bit8 ether_dhost[6];  // 48 bits: Мак-адрес получателя
        bit8 ether_shost[6];  // 48 bits: Мак-адрес отправителя
        bit16 ether_type;     // 16 bits: Тип протокола уровня выше (8 = ip)
    } ethernetHeader;

    string getMac(const bit8 addr[]);
    string byteToHexString(unsigned char a);
    string getSingleHexRegister(int b);

public:
    explicit DataLink(QObject *parent = nullptr);

    void deserializeHeader(const u_char *bytes);
    vector<pair<string, string>> getHeaderData();
    int getHeaderSize(){
        return ethernetHeaderSize;
    }
};

#endif // DATALINK_H
