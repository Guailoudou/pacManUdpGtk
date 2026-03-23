/*
 * @Date: 2026-03-12 10:05:56
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-23 09:38:52
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
int setaddr(int port, char *ip){
    //2.绑定IP+端口

    addr.sin_family = AF_INET;   
    if(isServer)                   //设置地址族 网络协议 IPV4
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY); 
        addr.sin_port = htons((short)port);
    }
    else
    {
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons((short)port);
    }
        
        //设置端口号，无符号16位网络字节序,将数字端口号进行转化
    if(isServer){
        udp_ret = bind(udp_sock, (struct sockaddr *)&addr, sizeof(addr));
        if (udp_ret == -1)
        {
            perror("bind fauler:\n\t");
            return -1;
        }
        // char buf[10];
        // socklen_t addr_len = sizeof(addr);
        // int recv_num = recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
    }else{
        // char buf[10]="hi";

        // socklen_t addr_len = sizeof(addr);
        // sendto(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, addr_len);
    
    }
    
}
// void send(char *data,int len)
// {
//     socklen_t addr_len = sizeof(addr);
//     sendto(udp_sock, data, len, 0, (struct sockaddr *)&addr, addr_len);
// }
// void recv(char *buf,int len)
// {
//     socklen_t addr_len = sizeof(addr);
//     memset(buf,0,sizeof(buf));
//     int recv_num = recvfrom(udp_sock, buf, len, 0, (struct sockaddr *)&addr, &addr_len);
// }
void *sendinfo(void *arg)
{
    while (isrun)
    {
        char buf[1024];
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%d_%d_%d_%d_%d_%d",himinfo.x,himinfo.y,
            ghostinfo.x,ghostinfo.y,himinfo.sc,gametime);
        socklen_t addr_len = sizeof(addr);
        sendto(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, addr_len);
        usleep(100);
    }
}
void *getinfo(void *arg)
{
    while (isrun)
    {
        char buf[1024];
        memset(buf,0,sizeof(buf));
        int tempx,tempy,tempc,tempgametime;
        // recv(buf, sizeof(buf));
        socklen_t addr_len = sizeof(addr);
        int recv_num = recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
        if(ishuman){
            sscanf(buf,"%d_%d_%d_%d_%d_%d",&tempx,&tempy,
            &ghostinfo.x,&ghostinfo.y,&tempc,&tempgametime);
        }else{
            sscanf(buf,"%d_%d_%d_%d_%d_%d",&himinfo.x,&himinfo.y,
            &tempx,&tempy,&himinfo.sc,&tempgametime);
        }
        if(tempgametime!=gametime && !isServer){
            gametime = tempgametime;
        }
        usleep(100);
    }
}

void *sendmap(void *arg)
{
    char buf[10];
    socklen_t addr_len = sizeof(addr);
    while (strcmp("hi",buf)!=0) //等待收到hi然后发送地图
    {
        int recv_num = recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
    }
    
    ssize_t sent_bytes = sendto(udp_sock, (const char *)maps, 
                                sizeof(maps), 
                                0, (struct sockaddr *)&addr, 
                                sizeof(addr));

    if (sent_bytes == -1) {
        perror("Send failed");
    } else {
        printf("Sent %zd bytes.\n", sent_bytes);
        //进入游戏
        if(ishuman){
            himinfo.x=MAPSIZE/2;
            himinfo.y=MAPSIZE/2;
            himinfo.sc=0;
        }else{
            ghostinfo.x=MAPSIZE/2;
            ghostinfo.y=MAPSIZE/2;
            sleep(5);
        }
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_game");
        isrun=true;
        runTick();
        runNetTask();
    }
}
void *recvmap(void *arg)
{
    socklen_t addr_len = sizeof(addr);

    char buf[10]="hi";
    sendto(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, addr_len);


    ssize_t recv_len = recvfrom(udp_sock, (char *)maps, sizeof(maps), 
                                0, (struct sockaddr *)&addr, &addr_len);
    if (recv_len < 0) {
        perror("Recv failed");
    } else {
        int count = recv_len / sizeof(int);
        printf("Received %zd bytes (%d integers).\n", recv_len, count);
        if(ishuman){
            himinfo.x=MAPSIZE/2;
            himinfo.y=MAPSIZE/2;
            himinfo.sc=0;
        }else{
            ghostinfo.x=MAPSIZE/2;
            ghostinfo.y=MAPSIZE/2;
            sleep(5);
        }
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_game");
        isrun=true;
        runTick();
        runNetTask();
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




