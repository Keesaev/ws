#include "udp.h"

Udp::Udp()
{

}

void Udp::deserializeHeader(const u_char *bytes, int offset) {
    //
}

bool Udp::isHeaderEmpty() {
    //
    return false;
}

vector<string> Udp::getHeaderData() {
    //
    std::cout << "udp_sport: " << udpHeader.udp_sport << "\n";
    vector<string> v;
    return v;
}
