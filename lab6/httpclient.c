#include <stdio.h>
#include "winsock2.h"
 
#pragma comment(lib, "ws2_32.lib")
 
int main()
{
       SOCKET      sSocket      = INVALID_SOCKET;
       SOCKADDR_IN stSvrAddrIn = {0}; /* 服务器端地址 */
       char        sndBuf[1024] = {0};
       char        rcvBuf[2048] = {0};
       char       *pRcv         = rcvBuf;
       int         num          = 0;
       int         nRet         = SOCKET_ERROR;
 
    WSADATA     wsaData;
 
       /* HTTP 消息构造开始，这是程序的关键之处 */
       sprintf(sndBuf, "GET / HTTP/1.1/n");
       strcat(sndBuf, "Host: github.com/n/r/n");
       /* HTTP 消息构造结束 */
      
       /* socket DLL初始化 */
    WSAStartup(MAKEWORD(2, 0), &wsaData);
 
       stSvrAddrIn.sin_family      = AF_INET;
    stSvrAddrIn.sin_port        = htons(80);
       stSvrAddrIn.sin_addr.s_addr = inet_addr("13.250.177.223");
 
       sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
       /* 连接 */
       nRet = connect(sSocket, (SOCKADDR*)&stSvrAddrIn, sizeof(SOCKADDR));
       if (SOCKET_ERROR == nRet)
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
