#include "transport.h"

Transport::Transport(QObject *parent) : QObject(parent)
{

}

void Transport::fillIcmp(const u_char *bytes, IcmpHeader *header, int offset){
    
}

void Transport::fillTcp(const u_char *bytes, TcpHeader *header, int offset){
    // Длина заголовка от 20 до 60, берем 60
    std::string str(reinterpret_cast<const char*>(bytes), offset + 60);
    str = str.substr(offset, str.length() - offset);
    std::stringstream stream(str);

    stream.read(reinterpret_cast<char*>(&header->tcp_sport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->tcp_dport), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->tcp_seqn), sizeof(tcp_seq));
    stream.read(reinterpret_cast<char*>(&header->tcp_ack), sizeof(tcp_seq));
    stream >> header->tcp_offx2 >> header->tcp_flags;
    stream.read(reinterpret_cast<char*>(&header->tcp_win), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->tcp_sum), sizeof(bit16));
    stream.read(reinterpret_cast<char*>(&header->tcp_urp), sizeof(bit16));

}

void Transport::fillUdp(const u_char *bytes, UdpHeader *header, int offset){

}

void Transport::fillHeader(const u_char *bytes, BaseTransportHeader *header, bit8 protocol, int offset){
    switch (protocol) {
    case 1:
        std::cout << "ICMP\n";
        header = new IcmpHeader();
        return;
    case 6:
        std::cout << "TCP\n";
        header = new TcpHeader();
        fillTcp(bytes, reinterpret_cast<TcpHeader*>(header), offset);

        /*unsigned int size_tcp = Headers::Header_tcp::getOffset(tcp)*4;
        if(size_tcp < 20){
            printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
                return;
        }*/

        break;
    case 17:
        std::cout << "UDP\n";
        header = new UdpHeader();
        return;
    default:
        std::cout << "Protocol type unknown\n";
        return;
    }
}
