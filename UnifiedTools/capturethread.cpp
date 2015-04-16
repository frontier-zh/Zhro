#include "capturethread.h"
#include <stdio.h>
#include <stdlib.h>
#define HAVE_REMOTE
#include <pcap.h>
#include "pheader.h"
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>

CaptureThread::CaptureThread(QObject *parent) :
    QThread(parent)
{
}

void CaptureThread::run()
{
    pcap_if_t* alldevs; // list of all devices
    pcap_if_t* d; // device you chose
    pcap_t* adhandle;

    char errbuf[PCAP_ERRBUF_SIZE] = {0}; //error buffer
    char    buff[512] = {0};
    struct pcap_pkthdr *pheader; /* packet header */
    const u_char * pkt_data; /* packet data */
    int res;
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        qDebug() << "Error in pcap_findalldevs_ex: " << errbuf;
        return;
    }

    //print the list of all devices
    for(d = alldevs; d != NULL; d = d->next)
    {
        qDebug() << d->name;
        int  ret = memcmp(d->name,"rpcap:\/\/\\Device\\NPF_{2EB9A4C6-4F02-40E5-AD92-3BCC918984A8}",strlen(d->name));
        if( !ret ){
            qDebug() << "found ethernet interface device.";
            break;
        }
        qDebug() << "not found ethernet interface device.";
        return;
    }
    // open the selected interface
    if((adhandle = pcap_open(d->name, // the interface name
                 65535, // length of packet that has to be retained
                 1, // promiscuous mode
                 10, // read time out
                 NULL, // auth
                 errbuf // error buffer
                 )) == NULL)
                 {
                     qDebug() << "Unable to open the adapter. " << d->description << " is not supported by Winpcap";
                     return;
                 }

    qDebug() << "\nListening on . " << d->description;

    pcap_freealldevs(alldevs); // release device list
    /* capture packet */
    struct bpf_program filter;
    pcap_compile(adhandle, &filter, "tcp", 1, 0);
    pcap_setfilter(adhandle, &filter);

    int i=0;
    char    buffer[BUFFER_MAX_LENGTH] = {0};
    while((res = pcap_next_ex(adhandle, &pheader, &pkt_data)) >= 0) {
        if(res == 0)
            continue; /* read time out*/
        if( i > 500 ){
            break;
        }
        QString     info;
        //IP Header.
        ether_header * eheader = (ether_header*)pkt_data;
        info.append("---------------------\n");
        sprintf(buff,"Frame Header Length: %d \t Ethernt TYPE: %d \n", pheader->len, eheader->ether_type);
        info.append(buff);
        sprintf(buff,"S_MAC:%02X:%02X:%02X:%02X:%02X:%02X\n", eheader->ether_shost[0],eheader->ether_shost[1],
                eheader->ether_shost[2],eheader->ether_shost[3],eheader->ether_shost[4],eheader->ether_shost[5]);
        info.append(buff);
        sprintf(buff,"D_MAC:%02X:%02X:%02X:%02X:%02X:%02X\n", eheader->ether_dhost[0],eheader->ether_dhost[1],
                eheader->ether_dhost[2],eheader->ether_dhost[3],eheader->ether_dhost[4],eheader->ether_dhost[5]);
        info.append(buff);
        //type ARP[1544] IP[8]
        //TCP Header.
        if(eheader->ether_type == 8 ) {
            ip_header * ih = (ip_header*)(pkt_data+14);
            sprintf(buff,"Total length: %d \t Protocol: %d\n", ih->tlen, ih->proto);
            info.append(buff);
            sprintf(buff,"Source IP: %d.%d.%d.%d \n", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
            info.append(buff);
            sprintf(buff,"Destin IP: %d.%d.%d.%d \n", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
            info.append(buff);
            qDebug() << info;
            //type ICMP[1] TCP[6] UDP[17] 未知[其他]
            if(ih->proto == 6 ) { /* tcp packet only */
                int     ip_len = ih->tlen;
                int     find_http = false;
                int     method = false;
                char*   ip_pkt_data = (char*)pkt_data;
                int     n = 54;
                int     bufsize = 0;
                QString string = "";
                for(; n<ip_len; n++)
                {
                    /* http get or post request */
                    if(!find_http && ((n+3<ip_len && strncmp(ip_pkt_data+n,"GET",strlen("GET")) ==0 )
                       || (n+4<ip_len && strncmp(ip_pkt_data+n,"POST",strlen("POST")) == 0)) ){
                            find_http = true;
                            qDebug() << "pos: " << n;
                    }
                    /* http response */
                    /* if http is found */
                    if(find_http)
                    {
                        if( ip_pkt_data[n] == 0 ){
                            buffer[bufsize] = '.'; /* copy http data to buffer */
                            string.append('.');
                        }else{
                            buffer[bufsize] = ip_pkt_data[n];
                            string.append(ip_pkt_data[n]);
                        }
                        bufsize ++;
                    }
                }
                /* print http content */
                if(find_http) {
                    if( !string.contains(url)){
                        i++;
                        continue;
                    }
                    if( string.contains("POST")){
                        method = true;
                    }
                    QStringList  headers = string.split("\r\n");
                    foreach( QString item, headers ){
                        qDebug() <<  "[ " << item << " ]";
                        if( item.contains("Accept:",Qt::CaseInsensitive)){
                            item = item.replace("Accept:", "");
                            result.insert("Accept:",item);
                        }else if( item.contains("Content-Type:",Qt::CaseInsensitive)){
                            item = item.replace("Content-Type:", "");
                            result.insert("Content-Type:",item);
                        }else if( item.contains("Referer:",Qt::CaseInsensitive)){
                            item = item.replace("Referer:", "");
                            result.insert("Referer:",item);
                        }else if( item.contains("Accept-Language:",Qt::CaseInsensitive)){
                            item = item.replace("Accept-Language:", "");
                            result.insert("Accept-Language:",item);
                        }else if( item.contains("Accept-Encoding:",Qt::CaseInsensitive)){
                            item = item.replace("Accept-Encoding:", "");
                            result.insert("Accept-Encoding:",item);
                        }else if( item.contains("User-Agent:",Qt::CaseInsensitive)){
                            item = item.replace("User-Agent:", "");
                            result.insert("User-Agent:",item);
                        }else if( item.contains("Cookie:",Qt::CaseInsensitive)){
                            item = item.replace("Cookie:", "");
                            result.insert("Cookie:",item);
                        }else if( method ){
                                if( method && item.size() < 100 ){
                                    continue;
                                }else{
                                    result.insert("Post:",item);
                                    break;
                                }
                        }
                    }
                    break;
                }
            }
        }
    }
    qDebug() << "------------  over  ---------";
    pcap_close(adhandle);

}
