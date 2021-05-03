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

bool Icmp::isHeaderEmpty() {
    //  TODO
    return false;
}

vector<string> Icmp::getHeaderData() {
    //  TODO
    cout << "ICMP HEADER:\n";
    cout << "icmp_type: " << static_cast<int>(icmpHeader.icmp_type) << "\n";
    cout << "icmp_code: " << static_cast<int>(icmpHeader.icmp_code) << "\n";
    cout << "icmp_sum: " << ntohs(icmpHeader.icmp_sum) << "\n";
    cout << "icmp_rest: " << ntohl(icmpHeader.icmp_rest) << "\n";

    vector<string> v;
    return v;
}
