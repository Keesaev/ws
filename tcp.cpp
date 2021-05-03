#include "tcp.h"

Tcp::Tcp()
{

}

void Tcp::deserializeHeader(const u_char *bytes, int offset){
    string str(reinterpret_cast<const char*>(bytes), offset + tcpMaxHeaderSize);
    str = str.substr(offset, tcpMaxHeaderSize);
    stringstream stream(str);

    // TODO
    // возможно потом можно переделать под оператор >> применив ntoh
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
    if(size_tcp < tcpMinHeaderSize || size_tcp > tcpMaxHeaderSize)
        return true;
    else
        return false;
}

vector<string> Tcp::getHeaderData(){

    // TODO

    std::cout << "TCP HEADER:\n";
    std::cout   << "tcp_sport: " << htons(tcpHeader.tcp_sport) << "\t\t"
                << "tcp_dport: " << htons(tcpHeader.tcp_dport) << "\n"
                << "tcp_seqn: "  << htonl(tcpHeader.tcp_seqn) << "\t\t"
                << "tcp_ack: "   << htonl(tcpHeader.tcp_ack) << "\n"
                << "tcp_offx2: "  << getOffset() << "\t\t"
                << "tcp_flags: "   << static_cast<int>(tcpHeader.tcp_flags) << "\n"
                << "tcp_win: "  << htons(tcpHeader.tcp_win) << "\t\t"
                << "tcp_sum: "   << htons(tcpHeader.tcp_sum) << "\n"
                << "tcp_urp: " << htons(tcpHeader.tcp_urp) << "\n";

    vector<string> v;
    return v;
}
