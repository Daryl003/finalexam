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

int main( )
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