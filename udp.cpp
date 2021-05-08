#include "udp.h"

Udp::Udp()
{

}

void Udp::deserializeHeader(const u_char *bytes, int offset) {
    string str(reinterpret_cast<const char*>(bytes), offset + udpHeaderSize);
    str = str.substr(offset, udpHeaderSize);
    stringstream stream(str);

    stream.read(reinterpret_cast<char*>(&udpHeader.udp_sport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&udpHeader.udp_dport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&udpHeader.udp_len), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&udpHeader.udp_chksum), sizeof(bit16));
}

bool Udp::isHeaderInvalid() {
    // TODO
    return false;
}

vector<pair<string, string>> Udp::getHeaderData() {

    vector<pair<string, string>> v = {
        {"Protocol", "UDP"},
        {"Source port", to_string(ntohs(udpHeader.udp_sport))},
        {"Destination port", to_string(ntohs(udpHeader.udp_dport))},
        {"Header length", to_string(ntohs(udpHeader.udp_len))},
        {"Checksum", to_string(ntohs(udpHeader.udp_chksum))}
    };
    return v;
}
