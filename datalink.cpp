#include "datalink.h"

DataLink::DataLink(QObject *parent) : QObject(parent)
{

}

void DataLink::deserializeHeader(const u_char *bytes){
    string str(reinterpret_cast<const char*>(bytes), ethernetHeaderSize);
    stringstream stream(str);

    for(int i = 0; i < 6; i++){
        stream >> ethernetHeader.ether_dhost[i];
    }

    for(int i = 0; i < 6; i++){
        stream >> ethernetHeader.ether_shost[i];
    }
    // stringstream не умеет читать u_short, перегрузка >> не помогает, поэтому так:
    stream >> reinterpret_cast<char*>(&ethernetHeader.ether_type), sizeof(bit16);
}

vector<pair<string, string>> DataLink::getHeaderData(){
    vector<pair<string, string>> v = {
        {"Source", getMac(ethernetHeader.ether_shost)},
        {"Destination", getMac(ethernetHeader.ether_dhost)},
        {"Type", to_string(ethernetHeader.ether_type)}
    };
    return v;
}

string DataLink::getMac(const bit8 addr[]){
    string s = "";
    for(int i = 0; i < 6; i++){
        s += byteToHexString(addr[i]) + ":";
    }
    s[s.length() - 1] = ' ';
    return s;
}

string DataLink::byteToHexString(unsigned char a){
    int b = a / 16;
    int c = a % 16;

    return (getSingleHexRegister(b) + getSingleHexRegister(c));
}

string DataLink::getSingleHexRegister(int b){
    string res = "";
        if(b > 9){
            map<int, string>::iterator f = hexLetters.find(b);
            if(f != hexLetters.end()){
                res = f->second;
            }
            else{
                res = "0";
            }
        }
        else{
            res = ('0' + b);
        }
        return res;
}
