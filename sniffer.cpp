// https://www.tcpdump.org/manpages/pcap.3pcap.html
// https://habr.com/ru/post/337840/

#include "sniffer.h"

Sniffer::Sniffer(QObject *parent) : QObject(parent)
{

}

// Возвращаем список доступных устройств
// в виде vector<pair<имя, описание>>
bool Sniffer::getDevs(std::vector<std::pair<char*, char*>> &d){
    pcap_if_t *alldevsp;
    char errbuf[PCAP_ERRBUF_SIZE];
    int retVal = pcap_findalldevs(&alldevsp, errbuf);

    if(retVal == PCAP_ERROR){
        qDebug() << errbuf << "\n";
        return false;
    }
    else{
        while(alldevsp->next != NULL){
            d.push_back(std::make_pair(alldevsp->name, alldevsp->description));
            alldevsp = alldevsp->next;
        }
        return true;
    }
}

// Устройство для прослушивания устанавливается извне
void Sniffer::setDev(char *d){
    m_dev = d;
}

// Инициализация pcap
bool Sniffer::initPcap(){
    if(m_dev == NULL || m_dev[0] == '/0')
        return false;
    char errbuf[PCAP_ERRBUF_SIZE];
    m_handle = pcap_create(m_dev, errbuf);
    if(m_handle == NULL){
        qDebug() << errbuf;
        return false;
    }
    else
    {
        pcap_set_snaplen(m_handle, 65535);
        pcap_set_promisc(m_handle, 1);
        pcap_set_rfmon(m_handle, 0);
        pcap_set_timeout(m_handle, 1000);
        pcap_set_immediate_mode(m_handle, 1);
        pcap_set_buffer_size(m_handle, PCAP_BUF_SIZE);
        pcap_set_tstamp_type(m_handle, PCAP_TSTAMP_HOST);
        int retVal = pcap_activate(m_handle);
        if(retVal >= 0){
            return true;
        }
        else{
            pcap_close(m_handle);
            return false;
        }
    }
}

void Sniffer::startLoopingCapture(){
    pcap_loop(m_handle, 10, handlePacket, NULL);
}

void Sniffer::handlePacket(u_char *user, const pcap_pkthdr *header,
                           const u_char *bytes){
    const struct Headers::Header_ethernet *ethernet;
    const struct Headers::Header_ip *ip;
    const struct Headers::Header_tcp *tcp;
    const struct Headers::Header_udp *udp;
    unsigned char *payload;

    unsigned int size_ip;
    unsigned int size_tcp;

    ethernet = (struct Headers::Header_ethernet*)(bytes);
    ip = (struct Headers::Header_ip*)(bytes + 14);
    size_ip = Headers::Header_ip::getHeaderLength(ip)*4;

    if(size_ip < 20){
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
            return;
    }

    switch (ip->ip_p) {
    case 6:
        std::cout << "TCP\n";
        break;
    case 17:
        std::cout << "UDP\n";
        return;
    default:
        std::cout << "Protocol type unknown\n";
        return;
    }

    tcp = (struct Headers::Header_tcp*)(bytes + 14 + size_ip);
    size_tcp = Headers::Header_tcp::getOffset(tcp)*4;
    if(size_tcp < 20){
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
            return;
    }
    payload = (unsigned char*)(bytes + 14 + size_ip + size_tcp);

    std::cout << "--------ETHERNET:--------\n";
    std::cout << "ether_type: " << ethernet->ether_type << "\n";
    std::cout << Headers::Header_ethernet::getMac(ethernet->ether_shost) << "\t"
              << Headers::Header_ethernet::getMac(ethernet->ether_dhost) << "\n";
    std::cout << "--------IP:--------------\n";
    std::cout << "ip_v: " << Headers::Header_ip::getVersion(ip) << "\t\t"
              << "ip_hl: " << Headers::Header_ip::getHeaderLength(ip) << "\n";
    std::cout << "ip_tos: " << Headers::Header_ip::getPriority(ip) << "\t\t"
              << "DTR bits: " << Headers::Header_ip::getDTR(ip) << "\n"
              << "ip-len: " << ntohs(ip->ip_len) << "\t\t"
              << "ip_id: "  << ntohs(ip->ip_id) << "\n"
              << "ip_off: " << static_cast<int>(ip->ip_off) << "\t\t"
              << "ip_ttl: " << static_cast<int>(ip->ip_ttl) << "\n"
              << "ip_p: "   << static_cast<int>(ip->ip_p) << "\t\t"
              << "ip_sum: " << ntohs(ip->ip_sum) << "\n";
    std::cout << Headers::Header_ip::getAddress(ip->ip_src) << "\t\t"
              <<  Headers::Header_ip::getAddress(ip->ip_dst) << "\n";
}

void Sniffer::stopLoopingCapture(){
    pcap_breakloop(m_handle);
    stopCapture();
}

void Sniffer::captureSinglePacket(){
    pcap_pkthdr *header;
    const u_char *pkt_data;
    int retVal = pcap_next_ex(m_handle, &header, &pkt_data);
    if(retVal == 1){
        qDebug() << "len: " << header->len;
        qDebug() << pkt_data;
    }
    else
        qDebug() << retVal;
}

void Sniffer::stopCapture(){
    pcap_close(m_handle);
}



