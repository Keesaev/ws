#ifndef HEADERS_H
#define HEADERS_H

#include <QObject>
#include <pcap/pcap.h>

class Headers : public QObject
{
    Q_OBJECT
public:
    explicit Headers(QObject *parent = nullptr);

    struct Header_ethernet
    {
        u_char ether_dhost[6];
        u_char ether_shost[6];
        u_short ether_type;
    };
    struct Header_ip{
        u_char ip_vhl;  // version
        u_char ip_tos;  // type of service
        u_short ip_len; // length
        u_short ip_id;  // id
        u_short ip_off; // offset
        /*
        #define IP_RF 0x8000
        #define IP_DF 0x4000
        #define IP_MF 0x2000
        #define IP_OFFMASK 0x1fff
        */

        u_char ip_ttl;  // time to live
        u_char ip_p;    // protocol
        u_short ip_sum; // checksum
        struct in_addr ip_src, ip_dst;
    };

    typedef u_int tcp_seq;

    struct Header_tcp{
        u_short th_sport;   // sender port
        u_short th_dport;   // destination port
        tcp_seq th_seq;     // sequence number
        tcp_seq th_ack;     // acknowledge number
        u_char th_offx2;    // offset

        u_char th_flags;
        /*
        #define TH_FIN 0x01
        #define TH_SYN 0x02
        #define TH_RST 0x04
        #define TH_PUSH 0x08
        #define TH_ACK 0x10
        #define TH_URG 0x20
        #define TH_ECE 0x40
        #define TH_CWR 0x80
        #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        */
        u_short th_win;  /* окно */
        u_short th_sum;  /* контрольная сумма */
        u_short th_urp;
    };

    static u_int th_off(const Header_tcp *tcp);
    static u_int ip_hl(const Header_ip *ip);
    static u_int ip_v(const Header_ip *ip);

signals:

};

#endif // HEADERS_H
