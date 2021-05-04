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

bool Tcp::isHeaderEmpty() {
    unsigned int size_tcp = getOffset()*4;
    if((int)size_tcp < tcpMinHeaderSize || (int)size_tcp > tcpMaxHeaderSize)
        return true;
    else
        return false;
}

vector<pair<string, string>> Tcp::getHeaderData(){

    vector<pair<string, string>> v = {
        make_pair("Source port", to_string(htons(tcpHeader.tcp_sport))),
        make_pair("Destination port", to_string(htons(tcpHeader.tcp_dport))),
        make_pair("Sequence number", to_string(htonl(tcpHeader.tcp_seqn))),
        make_pair("Acknowledgment Number", to_string(htonl(tcpHeader.tcp_ack))),
        make_pair("Data offset", to_string(getOffset())),
        make_pair("Flags (TODO)", to_string(static_cast<int>(tcpHeader.tcp_flags))),
        make_pair("Window size", to_string(htons(tcpHeader.tcp_win))),
        make_pair("Checksum", to_string(htons(tcpHeader.tcp_sum))),
        make_pair("Urgent Point", to_string(htons(tcpHeader.tcp_urp)))
    };

    return v;
}
