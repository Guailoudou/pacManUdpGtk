/*
 * @Date: 2026-03-18 10:35:02
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-20 12:36:27
 * @FilePath: /2JIEDUAN/head.h
 */
/*
 * @Date: 2026-03-16 09:55:04
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-19 18:30:24
 * @FilePath: /loudou/file/2JIEDUAN/head.h
 */
#ifndef __HEAD_H
#define __HEAD_H



#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <pthread.h>
#include <dirent.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

#include <gtk/gtk.h>

typedef struct sockaddr_in IPV4;
typedef struct playerinfo
{
    int x;
    int y;
    int sc;
}playerinfo;
#endif

#define MAPSIZE 50

extern IPV4 addr;
extern bool isServer;
extern int udp_sock;
extern int udp_ret;
extern int score;   //分数
extern int rival_score;  //对方分数
extern bool isrun;
extern bool ishuman;
extern int gametime;
extern playerinfo himinfo;
extern playerinfo ghostinfo;
extern int maps[MAPSIZE][MAPSIZE];
extern int view[MAPSIZE][MAPSIZE];
extern void createMaze();
extern void tick();
extern void tickTask();
extern int getview();
extern int createMap();
extern int cdrui_init(int argc, char *argv[]);

extern void up(); //上
extern void down(); //下
extern void left(); //左
extern void right(); //右边

extern void update_cell_color(int x, int y, guint color_hex);


#define WALL_BLOCK 0x323233
#define AIR_BLOCK 0xD4D4D4
#define COIN_BLOCK 0xFBDF9B
#define HIM_BLOCK 0x2CDB1D
#define GHOST_BLOCK 0xEF2E26
