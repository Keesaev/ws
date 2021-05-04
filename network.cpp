#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

bool Network::isHeaderEmpty(){
    unsigned int size_ip = getHeaderSize() * 4;
    if(size_ip < ipMinHeaderSize)
        return true;
    else{
        return false;
    }
}

void Network::deserializeHeader(const u_char *bytes){
    // Читаем последовательность длиной ethernetSize + ipMaxSize
    std::string str(reinterpret_cast<const char*>(bytes), ethernetHeaderSize + ipMaxHeaderSize);
    str = str.substr(ethernetHeaderSize, ipMaxHeaderSize);
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

vector<pair<string, string>> Network::getHeaderData(){

    vector<pair<string, string>> v = {
        make_pair("Version", to_string(getVersion())),
        make_pair("Header Length", to_string(getHeaderSize())),
        make_pair("Type of service", getTos()),
        make_pair("Total length", to_string(ntohs(ipHeader.ip_len))),
        make_pair("Identification", to_string(ntohs(ipHeader.ip_id))),
        make_pair("Flags", getFlags()),
        make_pair("Fragment offset", to_string(getOffset())),
        make_pair("Time to live", to_string(static_cast<int>(ipHeader.ip_ttl))),
        make_pair("Protocol", to_string(static_cast<int>(ipHeader.ip_p))),
        make_pair("Header checksum", to_string(ntohs(ipHeader.ip_sum))),
        make_pair("Source", getAddress(ipHeader.ip_src)),
        make_pair("Destination", getAddress(ipHeader.ip_dst))
    };
    return v;
}

std::string Network::getAddress(in_addr addr){
    char cAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr), cAddr, INET_ADDRSTRLEN);
    return cAddr;
}

std::string Network::getTos(){
    std::string s = "";
    for(int i = 128; i >= 1; i /= 2){
        if((ipHeader.ip_tos) & i)
            s += "1";
        else
            s += "0";
    }
    return s;
}

std::string Network::getFlags(){
    std::string s = "";
    int a = ntohs(ipHeader.ip_off);
    a = a >> 14;
    int b = 4;
    for(int i = 0; i < 3; i++){
        if(a & i)
            s += "1";
        else
            s += "0";
        b /= 2;
    }
    return s;
}
