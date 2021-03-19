#include "headers.h"

Headers::Headers(QObject *parent) : QObject(parent)
{

}
u_int Headers::th_off(const Header_tcp *tcp){
    return (((tcp)->th_offx2 & 0xf0) >> 4);
}

u_int Headers::ip_hl(const Header_ip *ip){
    return (((ip)->ip_vhl) & 0x0f);
}

u_int Headers::ip_v(const Header_ip *ip){
    return (((ip)->ip_vhl) >> 4);
}
