#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

bool Network::isHeaderEmpty(){ // TODO
    unsigned int size_ip = getHeaderSize() * 4;
    if(size_ip < ipMinHeaderSize)
        return true;
    else{
        return false;
    }
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

// TODO

vector<pair<string, string>> Network::getHeaderData(){

    vector<pair<string, string>> v = {
        make_pair("Version", to_string(getVersion())),
        make_pair("Internet Header Length", to_string(getHeaderSize())),
        make_pair("Differenciated services code point", getDiffServ()),
        make_pair("Explicit Congestion Notification", getECN()),
        make_pair("Total length", to_string(ntohs(ipHeader.ip_len))),
        make_pair("Identification", to_string(ntohs(ipHeader.ip_id))),
        make_pair("Flags", getFlags()),
        make_pair("Fragment offset", to_string(getOffset())),
        make_pair("Time to live", to_string(static_cast<int>(ipHeader.ip_ttl))),
        make_pair("Protocol", getProtocolName()),
        make_pair("Header checksum", to_string(ntohs(ipHeader.ip_sum))),
        make_pair("Source", getAddress(ipHeader.ip_src)),
        make_pair("Destination", getAddress(ipHeader.ip_dst))
    };
    return v;
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

string Network::getProtocolName(){
    int p = static_cast<int>(ipHeader.ip_p);
    string s = to_string(p);

    map<int, string>::iterator res = transportProts.find(p);
    if(res != transportProts.end())
        s += " (" + res->second + ")";
    return s;
}
