#include "tcp.h"

Tcp::Tcp()
{

}

void Tcp::deserializeHeader(const u_char *bytes, int offset){
    // Длина заголовка от 20 до 60, берем 60
    string str(reinterpret_cast<const char*>(bytes), offset + 60);
    str = str.substr(offset, 60);
    stringstream stream(str);

    // возможно потом можно переделать под оператор >> применив ntoh
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_sport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_dport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_seqn), sizeof(tcp_seq));
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_ack), sizeof(tcp_seq));
    stream >> tcpHeader.tcp_offx2 >> tcpHeader.tcp_flags;
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_win), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_sum), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(tcpHeader.tcp_urp), sizeof(bit16));
}

bool Tcp::isHeaderEmpty() {
    unsigned int size_tcp = TcpHeader::getOffset(const_cast<const TcpHeader*>(&tcpHeader))*4;
    if(size_tcp < 20)
        return true;
    else
        return false;
}

vector<string> Tcp::getHeaderData(){

    // Сделать запись в вектор

    std::cout << "TCP HEADER:\n";
    std::cout   << "tcp_sport: " << htons(tcpHeader.tcp_sport) << "\t\t"
                << "tcp_dport: " << htons(tcpHeader.tcp_dport) << "\n"
                << "tcp_seqn: "  << htons(tcpHeader.tcp_seqn) << "\t\t"
                << "tcp_ack: "   << htons(tcpHeader.tcp_ack) << "\n"
                << "tcp_offx2: "  << tcpHeader.tcp_offx2 << "\t\t"
                << "tcp_flags: "   << tcpHeader.tcp_flags << "\n"
                << "tcp_win: "  << htons(tcpHeader.tcp_win) << "\t\t"
                << "tcp_sum: "   << htons(tcpHeader.tcp_sum) << "\n"
                << "tcp_urp: " << htons(tcpHeader.tcp_urp) << "\n";

    vector<string> v;
    return v;
}
