#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

vector<pair<string, string>> Network::getHeaderData(){

    vector<pair<string, string>> v = {
        {"Version", to_string(getVersion())},
        {"Internet Header Length", to_string(getHeaderSize())},
        {"Differenciated services code point", getDiffServ()},
        {"Explicit Congestion Notification", getECN()},
        {"Total length", to_string(ntohs(ipHeader.ip_len))},
        {"Identification", to_string(ntohs(ipHeader.ip_id))},
        {"Flags", getFlags()},
        {"Fragment offset", to_string(getOffset())},
        {"Time to live", to_string(static_cast<int>(ipHeader.ip_ttl))},
        {"Protocol", getProtocolName()},
        {"Header checksum", to_string(ntohs(ipHeader.ip_sum))},
        {"Source", getAddress(ipHeader.ip_src)},
        {"Destination", getAddress(ipHeader.ip_dst)}
    };
    return v;
}

void Network::deserializeHeader(const u_char *bytes){
    // Читаем последовательность длиной ethernetSize + ipMaxSize
    string str(reinterpret_cast<const char*>(bytes), ethernetHeaderSize + ipMaxHeaderSize);
    str = str.substr(ethernetHeaderSize, ipMaxHeaderSize);
    stringstream stream(str);

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

bool Network::isHeaderEmpty(){ // TODO
    unsigned int size_ip = getHeaderSize() * 4;
    if(size_ip < ipMinHeaderSize)
        return true;
    else{
        return false;
    }
}

int Network::getProtocol(){
    return ipHeader.ip_p;
}

string Network::getProtocolName(){
    int p = static_cast<int>(ipHeader.ip_p);
    string s = to_string(p);

    map<int, string>::iterator res = transportProts.find(p);
    if(res != transportProts.end())
        s += " (" + res->second + ")";
    return s;
}

// Читаем левые 4 бита поля ip_vhl
int Network::getHeaderSize(){
    return (((ipHeader).ip_vhl) & 0x0f) * 4;
}

string Network::getSourceIp(){
    return getAddress(ipHeader.ip_src);
}

string Network::getDestIp(){
    return getAddress(ipHeader.ip_dst);
}

string Network::getAddress(in_addr addr){
    char cAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr), cAddr, INET_ADDRSTRLEN);
    return cAddr;
}

string Network::getECN(){
    int e = ipHeader.ip_tos & 3;
    string s = "Decimal value: " + to_string(e);

    map<int, string>::iterator res = ecn.find(e);
    if(res != ecn.end())
        s += " (" + res->second + ")";

    return s;
}

string Network::getDiffServ(){
    int ds = ipHeader.ip_tos >> 2;
    string s = "Decimal value: " + to_string(ds);

    map<int, string>::iterator res = diffServ.find(ds);
    if(res != diffServ.end())
        s += " (DSCP: " + res->second + ")";

    return s;
}

// https://www.pearsonitcertification.com/articles/article.aspx?p=1843887
string Network::getFlags(){
    string s = "";
    int a = ntohs(ipHeader.ip_off);
    bool df = (a >> 14) & 1;
    bool mf = (a >> 13) & 1;

    s += df ? "Don't fragment " : "";
    s += mf ? "More fragments" : "";

    if(s.empty())
        s = "Not set";

    return s;
}

int Network::getVersion(){       // Читаем правые 4 бита поля ip_vhl
    return ((ipHeader.ip_vhl) >> 4);
}
int Network::getOffset(){        // Берем всё кроме левых 3 битов в ip_off
    return (ntohs(ipHeader.ip_off) & 8191);
}


