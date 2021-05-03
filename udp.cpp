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

bool Udp::isHeaderEmpty() {
    // TODO
    return false;
}

vector<string> Udp::getHeaderData() {
    // TODO
    cout << "UDP HEADER:\n";
    cout << "udp_sport: " << ntohs(udpHeader.udp_sport) << "\n";
    cout << "udp_sport: " << ntohs(udpHeader.udp_dport) << "\n";
    cout << "udp_sport: " << ntohs(udpHeader.udp_len) << "\n";
    cout << "udp_sport: " << ntohs(udpHeader.udp_chksum) << "\n";
    vector<string> v;
    return v;
}
