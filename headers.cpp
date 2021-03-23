#include "headers.h"

Headers::Headers(QObject *parent) : QObject(parent)
{

}

std::string Headers::byteToHexString(unsigned char a){
    int b = a / 16;
    int c = a % 16;

    return (getSingleHexRegister(b) + getSingleHexRegister(c));
}

std::string Headers::getSingleHexRegister(int b){
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
