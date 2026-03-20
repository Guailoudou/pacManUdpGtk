/*
 * @Date: 2026-03-12 10:05:56
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-20 12:41:40
 * @FilePath: /2JIEDUAN/socks.c
 */
#include "head.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;    // 条件变量
pthread_cond_t queuecond = PTHREAD_COND_INITIALIZER;    // 条件变量

//PORT   50001
void getsock()
{
    //1.申请网络套接字
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock == -1)
    {
        perror("socket faluer:\n\t");
    }
}
int setaddr(int argc, char *argv[]){
    //2.绑定IP+端口

    addr.sin_family = AF_INET;   
    if(isServer)                   //设置地址族 网络协议 IPV4
        addr.sin_addr.s_addr = htonl(INADDR_ANY);       //由于服务器段接受数据不局限于特定IP，所以设置任意IP地址
    else
        addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons((short)atoi(argv[1]));    //设置端口号，无符号16位网络字节序,将数字端口号进行转化
    if(isServer){
        udp_ret = bind(udp_sock, (struct sockaddr *)&addr, sizeof(addr));
        if (udp_ret == -1)
        {
            perror("bind fauler:\n\t");
            return -1;
        }
    }
    
}
void send(char *data,int len)
{
    socklen_t addr_len = sizeof(addr);
    sendto(udp_sock, data, len, 0, (struct sockaddr *)&addr, addr_len);
}
void recv(char buf[],len)
{
    socklen_t addr_len = sizeof(addr);
    memset(buf,0,sizeof(buf));
    int recv_num = recvfrom(udp_sock, buf, len, 0, (struct sockaddr *)&addr, &addr_len);

}
void sendinfo()
{
    while (isrun)
    {
        char buf[1024];
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%d-%d-%d-%d-%d-%d",himinfo.x,himinfo.y,
            ghostinfo.x,ghostinfo.y,himinfo.sc,gametime);
        send(buf,strlen(buf));
    }
}
void getinfo()
{
    while (isrun)
    {
        char buf[1024];
        memset(buf,0,sizeof(buf));
        int tempx,tempy,tempc,tempgametime;
        recv(buf, sizeof(buf));
        socklen_t addr_len = sizeof(addr);
        int recv_num = recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
        if(ishuman){
            sscanf(buf,"%d-%d-%d-%d-%d-%d",&tempx,&tempy,
            &ghostinfo.x,&ghostinfo.y,&tempc,&tempgametime);
        }else{
            sscanf(buf,"%d-%d-%d-%d-%d-%d",&himinfo.x,&himinfo.y,
            &tempx,&tempy,&himinfo.sc,&tempgametime);
        }
        if(tempgametime!=gametime && !isServer){
            gametime = tempgametime;
        }

    }
}
    // char buf[256];
    // socklen_t addr_len = sizeof(buf);
    // while (1)
    // {
    //     memset(buf,0,sizeof(buf));
    //     int recv_num = recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
    //     printf("recv:[%d]byte, Msg:<%s>\n",recv_num, buf);
        
    //     sendto(udp_sock, buf, strlen(buf), 0, (struct sockaddr *)&addr, addr_len);
    //     // memset(buf,0,sizeof(buf));
    //     if (!strncmp(buf,"quit",4)) break;
    // }
    
    // close(udp_sock);
    // return 0;




