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

void Sniffer::startLoopingCapture(int c){

    if(c == -1){
        while(true){
            captureSinglePacket();
        }
    }
    else{
        while(c--){
            captureSinglePacket();
        }
    }
}

void Sniffer::captureSinglePacket(){

    pcap_pkthdr *header;
    const u_char *bytes;
    int retVal = pcap_next_ex(m_handle, &header, &bytes);
    if(retVal != 1){
        // Сломанный пакет TODO
        return;
    }

    DataLink *datalink = new DataLink();
    Network *network = new Network();
    BaseTransport *transport;

    datalink->deserializeHeader(bytes);
    network->deserializeHeader(bytes);
    if(network->isHeaderEmpty()){
        delete datalink;
        delete network;
        return;
    }

    transport = Factory::makeTransport(network->getProtocol());
    transport->deserializeHeader(bytes, datalink->getHeaderSize() + network->getHeaderSize());

    PacketData packet;

    packet.dataLinkFullData = datalink->getHeaderData();
    packet.networkFullData = network->getHeaderData();
    packet.transportFullData = transport->getHeaderData();
    packet.timestamp = header->ts;
    packet.sourceIp = network->getSourceIp();
    packet.destIp = network->getDestIp();
    packet.protocol = network->getProtocol();
    packet.length = to_string(header->len);

    emit packetDeserialized(packet);

    delete datalink;
    delete network;
    delete transport;
}

void Sniffer::stopCapture(){
    pcap_close(m_handle);
}



