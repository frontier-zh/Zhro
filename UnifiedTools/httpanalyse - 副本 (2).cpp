#include "httpanalyse.h"
#include "ui_httpanalyse.h"
#include <stdio.h>
#include <stdlib.h>
#define HAVE_REMOTE
#include <pcap.h>
#include "pheader.h"
#include <winsock.h>
 #include <winsock2.h>
 #include <ws2tcpip.h>

HttpAnalyse::HttpAnalyse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpAnalyse)
{
    ui->setupUi(this);
}

HttpAnalyse::~HttpAnalyse()
{
    delete ui;
}

void HttpAnalyse::on_button_clicked()
{
    pcap_if_t* alldevs; // list of all devices
    pcap_if_t* d; // device you chose

    pcap_t* adhandle;

    char errbuf[PCAP_ERRBUF_SIZE] = {0}; //error buffer
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
                 8192, // length of packet that has to be retained
                 PCAP_OPENFLAG_PROMISCUOUS, // promiscuous mode
                 2000, // read time out
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
        if( i > 100 ){
            break;
        }
        ether_header * eheader = (ether_header*)pkt_data;
        qDebug() << "ether_type: " << eheader->ether_type;
        //type ARP[1544] IP[8]
        if(eheader->ether_type == 8 ) { /* ip packet only */
            ip_header * ih = (ip_header*)(pkt_data+14); /* get ip header */
            qDebug() << "length: " << ih->tlen << "protocol: " << ih->proto;
            //type TCP[6]
            if(ih->proto == 6 ) { /* tcp packet only */
                int     ip_len = ih->tlen;
                int     find_http = false;
                char*   ip_pkt_data = (char*)pkt_data;
                int     n = 34;
                int     bufsize = 0;
                QString string = "";
                for(; n<ip_len; n++)
                {
                    /* http get or post request */
//                        if(!find_http && ((n+3<ip_len && strncmp(ip_pkt_data+n,"GET",strlen("GET")) ==0 )
//                       || (n+4<ip_len && strncmp(ip_pkt_data+n,"POST",strlen("POST")) == 0)) ){
                    if(!find_http && (n+4<ip_len && strncmp(ip_pkt_data+n,"POST",strlen("POST")) == 0)){
                            find_http = true;
                            qDebug() << "pos: " << n;
                    }
                    /* http response */
                    /* if http is found */
                    if(find_http)
                    {
                        if( ip_pkt_data[n] != 0 ){
                            buffer[bufsize] = ip_pkt_data[n]; /* copy http data to buffer */
                            bufsize ++;
                            string.append(ip_pkt_data[n]);
                        }
                    }
                }
                /* print http content */
                if(find_http) {
//                    buffer[bufsize] = '\0';
                    qDebug() << buffer << " | " << strlen(buffer) << " | " << string.size();
                    qDebug() << "\n**********************************************\n\n";
                    QStringList  headers = string.split("\r\n");
                    foreach( QString item, headers ){
                        qDebug() <<  "[ " << item << " ]";
                        if( item.contains("Accept:",Qt::CaseInsensitive)){
                            item = item.replace("Accept:", "");
                            this->ui->edit1->setText(item);
                        }
                        if( item.contains("Content-Type:",Qt::CaseInsensitive)){
                            item = item.replace("Content-Type:", "");
                            this->ui->edit2->setText(item);
                        }
                        if( item.contains("Referer:",Qt::CaseInsensitive)){
                            item = item.replace("Referer:", "");
                            this->ui->edit3->setText(item);
                        }
                        if( item.contains("Accept-Language:",Qt::CaseInsensitive)){
                            item = item.replace("Accept-Language:", "");
                            this->ui->edit4->setText(item);
                        }
                        if( item.contains("Accept-Encoding:",Qt::CaseInsensitive)){
                            item = item.replace("Accept-Encoding:", "");
                            this->ui->edit5->setText(item);
                        }
                        if( item.contains("User-Agent:",Qt::CaseInsensitive)){
                            item = item.replace("User-Agent:", "");
                            this->ui->edit6->setText(item);
                        }
                        if( item.contains("Cookie:",Qt::CaseInsensitive)){
                            item = item.replace("Cookie:", "");
                            this->ui->plainedit->setPlainText(item);
                        }
//                            if( item.contains("Cookie:",Qt::CaseInsensitive)){
//                                item = item.replace("Cookie:", "");
//                                this->ui->edit1->setText(item);
//                            }
                    }
                    break;
                }
            }
        }
    }
    qDebug() << "------------  over  ---------";
    pcap_close(adhandle);

}
