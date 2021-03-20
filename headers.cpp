#include "headers.h"

Headers::Headers(QObject *parent) : QObject(parent)
{

}

std::string Headers::byteToHexString(u_char a){
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
            res = "A";
                break;
            case 11:
            res = "B";
                break;
            case 12:
            res = "C";
                break;
            case 13:
            res = "D";
                break;
            case 14:
            res = "E";
                break;
            case 15:
            res = "F";
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
