#include "tcp.h"

Tcp::Tcp()
{

}

void Tcp::deserializeHeader(const u_char *bytes, int offset){
    string str(reinterpret_cast<const char*>(bytes), offset + tcpMaxHeaderSize);
    str = str.substr(offset, tcpMaxHeaderSize);
    stringstream stream(str);

    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_sport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_dport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_seqn), sizeof(bit32));
    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_ack), sizeof(bit32));
    stream >> tcpHeader.tcp_offx2 >> tcpHeader.tcp_flags;
    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_win), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_sum), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&tcpHeader.tcp_urp), sizeof(bit16));
}

bool Tcp::isHeaderInvalid() {
    unsigned int size_tcp = getOffset()*4;
    if((int)size_tcp < tcpMinHeaderSize || (int)size_tcp > tcpMaxHeaderSize)
        return true;
    else
        return false;
}

vector<pair<string, string>> Tcp::getHeaderData(){

    vector<pair<string, string>> v = {
        {"Protocol", "TCP"},
        {"Source port", to_string(htons(tcpHeader.tcp_sport))},
        {"Destination port", to_string(htons(tcpHeader.tcp_dport))},
        {"Sequence number", to_string(htonl(tcpHeader.tcp_seqn))},
        {"Acknowledgment Number", to_string(htonl(tcpHeader.tcp_ack))},
        {"Data offset", to_string(getOffset())},
        {"Flags", getFlags()},
        {"Window size", to_string(htons(tcpHeader.tcp_win))},
        {"Checksum", to_string(htons(tcpHeader.tcp_sum))},
        {"Urgent Point", to_string(htons(tcpHeader.tcp_urp))}
    };

    return v;
}

int Tcp::getOffset(){
    return ((tcpHeader.tcp_offx2) >> 4);
}

// https://en.wikipedia.org/wiki/Transmission_Control_Protocol
// https://networkguru.ru/protokol-transportnogo-urovnia-tcp-chto-nuzhno-znat/
string Tcp::getFlags(){
    string s = "";
    if(tcpHeader.tcp_offx2 & 1)
        s += "NC (Nonce Sum)\n";
    for(auto i : flags){
        if(tcpHeader.tcp_flags & i.first)
            s += i.second;
    }
    return s;
}
