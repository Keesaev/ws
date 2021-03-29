#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

bool Network::fillHeader(IpHeader *header, const u_char *bytes){
    std::string str(reinterpret_cast<const char*>(bytes), 34);
    str = str.substr(14, 20);
    std::stringstream stream(str);

    stream >> header->ip_vhl;
    stream >> header->ip_tos;
    stream.read(reinterpret_cast<char*>(&header->ip_len), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->ip_id), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->ip_off), sizeof(bit16));
    stream >> header->ip_ttl;
    stream>> header->ip_p;
    stream.read(reinterpret_cast<char*>(&header->ip_sum), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->ip_src), sizeof(in_addr));
    stream.read(reinterpret_cast<char*>(&header->ip_dst), sizeof(in_addr));

    unsigned int size_ip = getHeaderLength(header) * 4;
    if(size_ip < 20)
        return false;
    else{
        std::cout << "ip_v: " << getVersion(header) << "\t\t"
                  << "ip_hl: " << getHeaderLength(header) << "\n";
        std::cout << "ip_tos: " << getPriority(header) << "\t\t"
                  << "DTR bits: " << getDTR(header) << "\n"
                  << "ip-len: " << ntohs(header->ip_len) << "\t\t"
                  << "ip_id: "  << ntohs(header->ip_id) << "\n"
                  << "ip_off: " << static_cast<int>(header->ip_off) << "\t\t"
                  << "ip_ttl: " << static_cast<int>(header->ip_ttl) << "\n"
                  << "ip_p: "   << static_cast<int>(header->ip_p) << "\t\t"
                  << "ip_sum: " << ntohs(header->ip_sum) << "\n";
        std::cout << getAddress(header->ip_src) << "\t\t"
                  <<  getAddress(header->ip_dst) << "\n";
        return true;
    }
}
