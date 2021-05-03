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
        std::cout << errbuf << "\n";
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
    if(m_dev == NULL)
        return false;
    char errbuf[PCAP_ERRBUF_SIZE];
    m_handle = pcap_create(m_dev, errbuf);
    if(m_handle == NULL){
        std::cout << errbuf;
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
    pcap_loop(m_handle, 100, handlePacket, NULL);
}

void Sniffer::handlePacket(u_char *user, const pcap_pkthdr *header,
                           const u_char *bytes){
    DataLink *datalink = new DataLink();
    Network *network = new Network();
    BaseTransport *transport;

    unsigned char *payload;
    const unsigned int size_datalink = 14;
    unsigned int size_network;
    unsigned int size_transport;

    datalink->deserializeHeader(bytes);
    network->deserializeHeader(bytes);
    if(network->isHeaderEmpty()){
        delete datalink;
        delete network;
        return;
    }
    size_network = network->getHeaderLength() * 4;

    // Тут проблема, скорее всего надо присмотреться к Factory или к наследованию
    // Перечитать как делаются виртуальные функции

    transport = Factory::makeTransport(network->getProtocol());
    transport->deserializeHeader(bytes, size_datalink + size_network);

    network->getHeaderData();
    transport->getHeaderData();

    delete datalink;
    delete network;
    delete transport;
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
        std::cout << "len: " << header->len;
        std::cout << pkt_data;
    }
    else
        std::cout << retVal;
}

void Sniffer::stopCapture(){
    pcap_close(m_handle);
}



