#include "icmp.h"

Icmp::Icmp()
{

}

void Icmp::deserializeHeader(const u_char *bytes, int offset) {
    string str(reinterpret_cast<const char*>(bytes), offset + icmpSize);
    str = str.substr(offset, icmpSize);
    stringstream stream(str);

    stream >> icmpHeader.icmp_type;
    stream >> icmpHeader.icmp_code;
    stream.read(reinterpret_cast<char*>(&icmpHeader.icmp_sum), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&icmpHeader.icmp_rest), sizeof(uint32_t));
}

bool Icmp::isHeaderInvalid() {
    //  TODO
    return false;
}

vector<pair<string, string>> Icmp::getHeaderData() {

    vector<pair<string, string>> v = {
        make_pair("Protocol", "ICMP"),
        make_pair("Type", to_string(static_cast<int>(icmpHeader.icmp_type))),
        make_pair("Code", to_string(static_cast<int>(icmpHeader.icmp_code))),
        make_pair("Checksum", to_string(ntohs(icmpHeader.icmp_sum))),
        make_pair("Options:", to_string(ntohs(icmpHeader.icmp_rest)))
    };

    return v;
}
