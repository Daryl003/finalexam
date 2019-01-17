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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

char * Rstrchr(char * s, char x) {

	int i = strlen(s);
	if(!(*s)) return 0;
	while(s[i-1]) if(strchr(s + (i - 1), x)) return (s + (i - 1)); else i--;
	return 0;
}

void GetHost(char * src, char * web, char * file, int * port) {

	char * pA;
	char * pB;
	memset(web, 0, sizeof(web));
	memset(file, 0, sizeof(file));
	*port = 0;
	if(!(*src)) return;
	    pA = src;
    
	if(!strncmp(pA, "http://", strlen("http://"))) 
        pA = src+strlen("http://");
	else if(!strncmp(pA, "https://", strlen("https://"))) 
        pA = src+strlen("https://");

	pB = strchr(pA, '/');
	if(pB) {
		memcpy(web, pA, strlen(pA) - strlen(pB));
		if(pB+1) {
			memcpy(file, pB + 1, strlen(pB) - 1);
			file[strlen(pB) - 1] = 0;
		}
	}

	else memcpy(web, pA, strlen(pA));
	if(pB) web[strlen(pA) - strlen(pB)] = 0;
	else web[strlen(pA)] = 0;
	pA = strchr(web, ':');
	if(pA) *port = atoi(pA + 1);
	else *port = 80;
}

int httpclient( )
{
        char ipstring[20],**alias,**iplist;
        char *argv="github.com";
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber,nbytes;
	char host_addr[256];
	char host_file[1024];
	char local_file[256];
	FILE * fp;
	char request[1024];
	int send, totalsend;
	int i;
	char * pt;

//	if(argc!=2)
//	{
//		fprintf(stderr,"Usage:%s web-address\a\n",argv[0]);
//		exit(1);
//	}

	printf("parameter.1 is: %s\n", argv);
	GetHost(argv, host_addr, host_file, &portnumber);/*????????????*/
	printf("webhost:%s\n", host_addr);
	printf("hostfile:%s\n", host_file);
	printf("portnumber:%d\n\n", portnumber);

	if((host=gethostbyname(host_addr))==NULL)/*????IP??*/
	{
		fprintf(stderr,"Gethostname error, %s\n", strerror(errno));
		exit(1);
	}
	else
	{
	        switch(host->h_addrtype)
	        {
	                case AF_INET:
	                case AF_INET6:
	                for(iplist=host->h_addr_list;*iplist!=NULL;iplist++)
	                {
	                inet_ntop(host->h_addrtype,*host->h_addr_list,ipstring,sizeof(ipstring));
	                printf("IP address:%s\n",ipstring);
	                }
	                break;
	                default:
	                printf("UnKnow address");
	        }

	}

	/* ???????? sockfd??? */
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)/*??SOCKET??*/
	{
		fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		exit(1);
	}

	/* ???????????? */
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);

	/* ?????????? */
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)/*????*/
	{
		fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		exit(1);
	}

	sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
			User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
			Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber);
	printf("%s", request);/*??request????????*/

	/*????????*/
	if(host_file && *host_file) pt = Rstrchr(host_file, '/');
	else pt = 0;

	memset(local_file, 0, sizeof(local_file));
	if(pt && *pt) {
		if((pt + 1) && *(pt+1)) strcpy(local_file, pt + 1);
		else memcpy(local_file, host_file, strlen(host_file) - 1);
	}
	else if(host_file && *host_file) strcpy(local_file, host_file);
	else strcpy(local_file, "index.html");
	printf("local filename to write:%s\n\n", local_file);

	/*??http??request*/
	send = 0;totalsend = 0;
	nbytes=strlen(request);
	while(totalsend < nbytes) {
		send = write(sockfd, request + totalsend, nbytes - totalsend);
		if(send==-1) {printf("send error!%s\n", strerror(errno));exit(0);}
		totalsend+=send;
		printf("%d bytes send OK!\n", totalsend);
	}

	fp = fopen(local_file, "a");
	if(!fp) {
		printf("create file error! %s\n", strerror(errno));
		return 0;
	}
	printf("\nThe following is the response header:\n");
	i=0;

	/* ????????http???response */
	printf("\nhead  of  http:\n");
	while((nbytes=read(sockfd,buffer,1))==1)
	{
		if(i < 4) {
			if(buffer[0] == '\r' || buffer[0] == '\n') i++;
			else i = 0;
			printf("%c", buffer[0]);/*?http?????????*/
		}
		else {
			fwrite(buffer, 1, 1, fp);/*?http????????*/
			i++;
			if(i%1024 == 0) fflush(fp);/*?1K?????*/
		}
	}
	
	
	fclose(fp);
	/* ???? */
	close(sockfd);
	exit(0);
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
    MenuConfig("httpclient","open github.com",httpclient);
    MenuConfig("replyhi", "Reply hi TCP Service", StartReplyhi);
    MenuConfig("hello", "Hello TCP Client", Hello);
    MenuConfig("dns", "DNS Query github.com", Gethostbyname);
    ExecuteMenu();
}

