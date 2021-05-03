#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

bool Network::isHeaderEmpty(){
    unsigned int size_ip = getHeaderLength() * 4;
    if(size_ip < 20)
        return true;
    else{
        return false;
    }
}

void Network::deserializeHeader(const u_char *bytes){
    // Читаем последовательность длиной ethernetSize + ipMaxSize
    std::string str(reinterpret_cast<const char*>(bytes), 14 + ipMaxSize);
    str = str.substr(14, ipMaxSize);
    std::stringstream stream(str);

    stream >> ipHeader.ip_vhl;
    stream >> ipHeader.ip_tos;
    stream.read(reinterpret_cast<char*>(&ipHeader.ip_len), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&ipHeader.ip_id), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&ipHeader.ip_off), sizeof(bit16));
    stream >> ipHeader.ip_ttl;
    stream >> ipHeader.ip_p;
    stream.read(reinterpret_cast<char*>(&ipHeader.ip_sum), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&ipHeader.ip_src), sizeof(in_addr));
    stream.read(reinterpret_cast<char*>(&ipHeader.ip_dst), sizeof(in_addr));
}

// TODO

vector<string> Network::getHeaderData(){
    std::cout << "IP HEADER:\n";
    std::cout << "ip_v: " << getVersion() << "\t\t"
              << "ip_hl: " << getHeaderLength() << "\n"
              << "ip_tos: " << getTos() << "\t"
              << "ip-len: " << ntohs(ipHeader.ip_len) << "\t\t"
              << "ip_id: "  << ntohs(ipHeader.ip_id) << "\n"
              << "ip_flags: " << getFlags() << "\t\t"
              << "ip_off: " << getOffset() << "\t\t"
              << "ip_ttl: " << static_cast<int>(ipHeader.ip_ttl) << "\n"
              << "ip_p: "   << static_cast<int>(ipHeader.ip_p) << "\t\t\t"
              << "ip_sum: " << ntohs(ipHeader.ip_sum) << "\n"
              << Network::getAddress(ipHeader.ip_src) << "\t\t"
              <<  Network::getAddress(ipHeader.ip_dst) << "\n";
    vector<string> v;
    v.push_back("Some Ip data");
    return v;
}
