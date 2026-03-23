/*
 * @Date: 2026-03-12 10:05:56
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-23 14:18:38
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
        int recv_num = recvfrom(udp_sock, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr *)&addr, &addr_len);
        if(recv_num<0 && errno == EWOULDBLOCK){
            usleep(100);
            continue;
        }
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
    char temp[1024];
    while (recvfrom(udp_sock, temp, sizeof(temp), MSG_DONTWAIT, NULL, NULL) > 0) {
        // 清空缓冲区，不做处理
        printf("%s ",temp);
    }
    while (strcmp("hi",buf)!=0) //等待收到hi然后发送地图
    {
        usleep(500);
        int recv_num = recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
        if(strcmp("hi",buf)==0)
        sendto(udp_sock, (const char *)buf, 
                                sizeof(buf), 
                                0, (struct sockaddr *)&addr, 
                                sizeof(addr));
    }

    memset(buf, 0, sizeof(buf));
    ssize_t sent_bytes;
    // while (strcmp("ok",buf)!=0)
    // {
    //     usleep(500);
        sent_bytes = sendto(udp_sock, (const char *)maps, 
                                sizeof(maps), 
                                0, (struct sockaddr *)&addr, 
                                sizeof(addr));
    //     recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
    // }
    
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
        }
        printf("开始切换到游戏页\n");
        // gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_game");
        isrun=true;
        // runTick();
        runNetTask();
    }
}
void *recvmap(void *arg)
{
    socklen_t addr_len = sizeof(addr);
    char temp[1024];
    while (recvfrom(udp_sock, temp, sizeof(temp), MSG_DONTWAIT, NULL, NULL) > 0) {
        // 清空缓冲区，不做处理
        printf("%s ",temp);
    }
    char buf[10]="hi",buf2[10];
    while (strcmp("hi",buf2)!=0)
    {
        sendto(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, addr_len);
        recvfrom(udp_sock, (char *)buf2, sizeof(buf2), 
                            MSG_DONTWAIT, (struct sockaddr *)&addr, &addr_len);
    }
    // char buf3[10]="ok";
    ssize_t recv_len = recvfrom(udp_sock, (char *)maps, sizeof(maps), 
                                0, (struct sockaddr *)&addr, &addr_len);
    // sendto(udp_sock, buf3, sizeof(buf3), 0, (struct sockaddr *)&addr, addr_len);
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
        }
        printf("开始切换到游戏页\n");
        
        isrun=true;
        // runTick();
        runNetTask();
    }
}

void close_sock()
{
    printf("关闭套接字\n");
    close(udp_sock);
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




