#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"

#define FONTSIZE 10
int PrintMenuOS()
{
    int i, j;
    char data_M[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "  *    *  ",
        " ***  *** ",
        " * *  * * ",
        " * *  * * ",
        " *  **  * ",
        " *      * ",
        " *      * ",
        " *      * ",
        "          "
    };
    char data_e[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "          ",
        "    **    ",
        "   *  *   ",
        "  *    *  ",
        "  ******  ",
        "  *       ",
        "   *      ",
        "    ***   ",
        "          "
    };
    char data_n[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "          ",
        "    **    ",
        "   *  *   ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "          "
    };
    char data_u[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "          ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "   *  **  ",
        "    **  * ",
        "          "
    };
    char data_O[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "   ****   ",
        "  *    *  ",
        " *      * ",
        " *      * ",
        " *      * ",
        " *      * ",
        "  *    *  ",
        "   ****   ",
        "          "
    };
    char data_S[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "    ****  ",
        "   **     ",
        "  **      ",
        "   ***    ",
        "     **   ",
        "      **  ",
        "     **   ",
        "  ****    ",
        "          "
    };

    for(i=0; i<FONTSIZE; i++)
    {
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_M[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_e[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_n[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_u[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_O[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_S[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int Quit(int argc, char *argv[])
{
    /* add XXX clean ops */
}

#include"syswrapper.h"
#define MAX_CONNECT_QUEUE   1024
int Replyhi()
{
	char szBuf[MAX_BUF_LEN] = "\0";
	char szReplyMsg[MAX_BUF_LEN] = "hi\0";
	InitializeService();
	while (1)
	{
		ServiceStart();
		RecvMsg(szBuf);
		SendMsg(szReplyMsg);
		ServiceStop();
	}
	ShutdownService();
	return 0;
}

int StartReplyhi(int argc, char *argv[])
{
	int pid;
	/* fork another process */
	pid = fork();
	if (pid < 0)
	{
		/* error occurred */
		fprintf(stderr, "Fork Failed!");
		exit(-1);
	}
	else if (pid == 0)
	{
		/*	 child process 	*/
		Replyhi();
		printf("Reply hi TCP Service Started!\n");
	}
	else
	{
		/* 	parent process	 */
		printf("Please input hello...\n");
	}
}

int Hello(int argc, char *argv[])
{
	char szBuf[MAX_BUF_LEN] = "\0";
	char szMsg[MAX_BUF_LEN] = "hello\0";
	OpenRemoteService();
	SendMsg(szMsg);
	RecvMsg(szBuf);
	CloseRemoteService();
	return 0;
}

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
 
#include <unistd.h>
#include <stdio.h>
#define MAX_IFS 64
 
int BringUpNetInterface()
{
    printf("Bring up interface:lo\n");
    struct sockaddr_in sa;
    struct ifreq ifreqlo;
    int fd;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strncpy(ifreqlo.ifr_name, "lo",sizeof("lo"));
    memcpy((char *) &ifreqlo.ifr_addr, (char *) &sa, sizeof(struct sockaddr));
    ioctl(fd, SIOCSIFADDR, &ifreqlo);
    ioctl(fd, SIOCGIFFLAGS, &ifreqlo);
    ifreqlo.ifr_flags |= IFF_UP|IFF_LOOPBACK|IFF_RUNNING;
    ioctl(fd, SIOCSIFFLAGS, &ifreqlo);
    close(fd);
    
    printf("Bring up interface:eth0\n");
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("10.0.2.15");
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strncpy(ifreqlo.ifr_name, "eth0",sizeof("eth0"));
    memcpy((char *) &ifreqlo.ifr_addr, (char *) &sa, sizeof(struct sockaddr));
    ioctl(fd, SIOCSIFADDR, &ifreqlo);
    ioctl(fd, SIOCGIFFLAGS, &ifreqlo);
    ifreqlo.ifr_flags |= IFF_UP|IFF_RUNNING;
    ((unsigned char *) &ifreqlo.ifr_hwaddr.sa_data)[0] = 0x02;
    ((unsigned char *) &ifreqlo.ifr_hwaddr.sa_data)[1] = 0x42;
    ((unsigned char *) &ifreqlo.ifr_hwaddr.sa_data)[2] = 0xc0;
    ((unsigned char *) &ifreqlo.ifr_hwaddr.sa_data)[3] = 0xa8;
    ((unsigned char *) &ifreqlo.ifr_hwaddr.sa_data)[4] = 0x28;
    ((unsigned char *) &ifreqlo.ifr_hwaddr.sa_data)[5] = 0x05;
    ioctl(fd, SIOCSIFFLAGS, &ifreqlo);
    close(fd);

    printf("List all interfaces:\n");
    struct ifreq *ifr, *ifend;
    struct ifreq ifreq;
    struct ifconf ifc;
    struct ifreq ifs[MAX_IFS];
    int SockFD;
 
 
    SockFD = socket(PF_INET, SOCK_DGRAM, 0);
 
 
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(SockFD, SIOCGIFCONF, &ifc) < 0)
    {
        printf("ioctl(SIOCGIFCONF): %m\n");
        return 0;
    }
 
    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
    {
        printf("interface:%s\n", ifr->ifr_name);
#if 0
        if (strcmp(ifr->ifr_name, "lo") == 0)
        {
            strncpy(ifreq.ifr_name, ifr->ifr_name,sizeof(ifreq.ifr_name));
            ifreq.ifr_flags == IFF_UP;
            if (ioctl (SockFD, SIOCSIFFLAGS, &ifreq) < 0)
            {
              printf("SIOCSIFFLAGS(%s): IFF_UP %m\n", ifreq.ifr_name);
              return 0;
            }			
	    }
#endif
	    if (ifr->ifr_addr.sa_family == AF_INET)
        {
            strncpy(ifreq.ifr_name, ifr->ifr_name,sizeof(ifreq.ifr_name));
            if (ioctl (SockFD, SIOCGIFHWADDR, &ifreq) < 0)
            {
              printf("SIOCGIFHWADDR(%s): %m\n", ifreq.ifr_name);
              return 0;
            }
 
            printf("Ip Address %s\n", inet_ntoa( ( (struct sockaddr_in *)  &ifr->ifr_addr)->sin_addr)); 
            printf("Device %s -> Ethernet %02x:%02x:%02x:%02x:%02x:%02x\n", ifreq.ifr_name,
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[0],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[1],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[2],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[3],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[4],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[5]);
        }
    }
 
    return 0;
}
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/route.h>
#include <sys/types.h>
#include <sys/ioctl.h>

int    SetDefaultGateway()
{
    int sockfd;
    struct rtentry route;
    struct sockaddr_in *addr;
    int err = 0;

    if(((sockfd = socket(AF_INET, SOCK_DGRAM, 0)))<0){
        perror("socket");
        exit(1);
    }

    memset(&route, 0, sizeof(route));
    addr = (struct sockaddr_in*) &route.rt_gateway;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr("10.0.2.2");
    addr = (struct sockaddr_in*) &route.rt_dst;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr = (struct sockaddr_in*) &route.rt_genmask;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    route.rt_dev = "eth0";
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_metric = 0;
    if ((err = ioctl(sockfd, SIOCADDRT, &route)) != 0) {
         perror("SIOCADDRT failed");
         exit(1);
    }
    printf("Default gateway %s\n", inet_ntoa( ( (struct sockaddr_in *)  &route.rt_gateway)->sin_addr)); 
}

#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<stdlib.h>	//malloc
#include<sys/socket.h>	//you know what this is for
#include<arpa/inet.h>	//inet_addr , inet_ntoa , ntohs etc
#include<netinet/in.h>
#include<unistd.h>	//getpid
#include "getroute.c"
#include "dnsquery.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netdb.h>
#include <errno.h>

char dns_servers[10][100];
int dns_server_count = 0;
//DNS result
char dns_result[100];
//Types of DNS resource records :)

#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server

int httpClient() 
{

        int     sSocket;
        struct sockaddr_in stSvrAddrIn; /* 服务器端地址 */
	struct sockaddr_in clientAddr;/*client addr*/
        char        sndBuf[1024] = {0};
        char        rcvBuf[2048] = {0};
        char       *pRcv         = rcvBuf;
        int         num          = 0;
        int         nRet         ;
	char	*dst_ip;
	int	ip_lenth;
	
	ip_lenth = strlen(dns_result);
	dst_ip = (char *)malloc(ip_lenth*sizeof(char));
	dst_ip = dns_result;
        /* HTTP 消息构造开始,这是程序的关键之处 */
/*
        sprintf(sndBuf, "GET / HTTP/1.1\n");
        strcat(sndBuf, "Host: www.163.com\r\n");
	strcat(sndBuf, "User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n");
	strcat(sndBuf, "Accept-Language: en, mi\r\n");
*/
	sprintf(sndBuf,"GET / HTTP/1.1\r\n");
	strcat(sndBuf,"Accept:html/text*/*\r\n");
	strcat(sndBuf,"Accept-language:zh-ch\r\n");
	strcat(sndBuf,"Accept-Encoding:gzip,deflate\r\n");

//	strcat(sndBuf,"Host: 180.97.33.107:80\r\n");
//	strcat(sndBuf,"Host: 13.250.177.223\r\n");
	strcat(sndBuf,"Host: ");
	strcat(sndBuf,dst_ip);
	strcat(sndBuf,"\r\n");
	strcat(sndBuf,"User-Agent:client<1.0>\r\n");
	strcat(sndBuf,"Connection:Close\r\n");
	strcat(sndBuf,"\r\n");
	printf("sndBuf = %s\n",sndBuf);

        /* HTTP 消息构造结束 */
        /* socket DLL初始化 */
     
     stSvrAddrIn.sin_family      = AF_INET;
     stSvrAddrIn.sin_port        = htons(80);
     stSvrAddrIn.sin_addr.s_addr = inet_addr(dst_ip);//www.baidu.com 180.97.33.107

     clientAddr.sin_family = AF_INET;
     clientAddr.sin_port = htons(8080);
     clientAddr.sin_addr.s_addr = inet_addr("192.168.40.18");//192.168.40.254 MenuOS


     sSocket = socket(AF_INET, SOCK_STREAM, 0);

     bind(sSocket, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
          /* 连接 */
     nRet = connect(sSocket, (struct sockaddr *)&stSvrAddrIn, sizeof(stSvrAddrIn));
        if (nRet<0)
        {
               printf("connect fail!/n");
               return -1;
        }
               /* 发送HTTP请求消息 */

        send(sSocket, (char*)sndBuf, sizeof(sndBuf), 0);
          /* 接收HTTP响应消息 */
        while(1)
        {
               num = recv(sSocket, pRcv, 2048, 0);
                 pRcv += num;
              if((0 == num) || (-1 == num))
               {
                      break ;
               }
	
        }
               /* 打印响应消息 */
        printf("%s/n", rcvBuf);
          return 0; 
} 

int main()
{
    BringUpNetInterface();
    SetDefaultGateway();
    GetRoute();
    PrintMenuOS();
    SetPrompt("MenuOS>>");
    MenuConfig("version","MenuOS V1.0(Based on Linux 3.18.6)",NULL);
    MenuConfig("quit","Quit from MenuOS",Quit);
    MenuConfig("httpclient","open github.com",httpClient);
    MenuConfig("replyhi", "Reply hi TCP Service", StartReplyhi);
    MenuConfig("hello", "Hello TCP Client", Hello);
    MenuConfig("dns", "DNS Query github.com", Gethostbyname);
    ExecuteMenu();
}

