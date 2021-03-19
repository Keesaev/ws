// https://www.tcpdump.org/manpages/pcap.3pcap.html

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

u_int Sniffer::th_off(const header_tcp *tcp){
    return (((tcp)->th_offx2 & 0xf0) >> 4);
}

u_int Sniffer::ip_hl(const header_ip *ip){
    return (((ip)->ip_vhl) & 0x0f);
}

u_int Sniffer::ip_v(const header_ip *ip){
    return (((ip)->ip_vhl) >> 4);
}

void Sniffer::handlePacket(u_char *user, const pcap_pkthdr *header,
                           const u_char *bytes){
    const struct header_ethernet *ethernet;
    const struct header_ip *ip;
    const struct header_tcp *tcp = new header_tcp();
    u_char *payload;

    u_int size_ip;
    u_int size_tcp;

    ethernet = (struct header_ethernet*)(bytes);
    ip = (struct header_ip*)(bytes + 14);
    size_ip = ip_hl(ip)*4;
    if(size_ip < 20){
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
            return;
    }
    tcp = (struct header_tcp*)(bytes + 14 + size_ip);
    size_tcp = th_off(tcp)*4;
    if(size_tcp < 20){
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
            return;
    }
    payload = (u_char*)(bytes + 14 + size_ip + size_tcp);
    qDebug() << "Successfull packet handling";
    char dstAddr[INET_ADDRSTRLEN];
    char sndAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip->ip_dst.s_addr), dstAddr, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->ip_dst.s_addr), sndAddr, INET_ADDRSTRLEN);
    qDebug() << sndAddr << "\t" << dstAddr;
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



