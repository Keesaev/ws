#ifndef ICMP_H
#define ICMP_H

#include <QObject>
#include <basetransport.h>
#include <vector>
#include <string>
#include <sstream>

#include <iostream>

using namespace std;

class Icmp : public BaseTransport
{
    Q_OBJECT

    typedef unsigned char bit8;
    typedef unsigned short bit16;

    struct IcmpHeader {
        bit8 icmp_type;         // 8 bits: Тип
        bit8 icmp_code;         // 8 bits: Код
        bit16 icmp_sum;         // 16 bits: Контрольная сумма
        uint32_t icmp_rest;     // 32 bits: Зависит от типа и кода
    };

    IcmpHeader icmpHeader;

public:
    Icmp();

    void deserializeHeader(const u_char *bytes, int offset) override;
    bool isHeaderEmpty() override;
    vector<string> getHeaderData() override;
};

#endif // ICMP_H
