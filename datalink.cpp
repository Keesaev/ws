#include "datalink.h"

DataLink::DataLink(QObject *parent) : QObject(parent)
{

}

void DataLink::fillHeader(EthernetHeader *header, const u_char *bytes){
    std::string str(reinterpret_cast<const char*>(bytes), 14);
    std::stringstream stream(str);

    for(int i = 0; i < 6; i++){
        stream >> header->ether_dhost[i];
    }

    for(int i = 0; i < 6; i++){
        stream >> header->ether_shost[i];
    }
    stream >> header->ether_type;

    std::cout << DataLink::getMac(header->ether_dhost) << "\n";
    std::cout << DataLink::getMac(header->ether_shost) << "\n";
    std::cout << header->ether_type << "\n";
}

std::string DataLink::byteToHexString(unsigned char a){
    int b = a / 16;
    int c = a % 16;

    return (getSingleHexRegister(b) + getSingleHexRegister(c));
}

std::string DataLink::getSingleHexRegister(int b){
    std::string res = "";
        if(b > 9){
            switch (b)
            {
            case 10:
            res = "a";
                break;
            case 11:
            res = "b";
                break;
            case 12:
            res = "c";
                break;
            case 13:
            res = "d";
                break;
            case 14:
            res = "e";
                break;
            case 15:
            res = "f";
                break;

            default:
            res = "0";
                break;
            }
        }
        else{
            res = ('0' + b);
        }
        return res;
}
