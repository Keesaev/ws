#include "sniffer.h"

Sniffer::Sniffer(QObject *parent) : QObject(parent)
{

}

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

void Sniffer::setDev(char *d){
    m_dev = d;
}

void Sniffer::initPcap(){

}

void Sniffer::startCapture(){

}

void Sniffer::stopCapture(){

}
