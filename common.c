/*
 * @Date: 2026-03-19 09:52:13
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-23 09:10:46
 * @FilePath: /2JIEDUAN/common.c
 */
#include "head.h"
bool isServer; //是否为房主
bool ishuman;  //是否是人类 否为贵
int score=0;   //分数
int rival_score=0;  //对方分数
IPV4 addr;
int udp_sock;
int udp_ret;
bool isrun; // 游戏中
int gametime=0;
pthread_t sendpid = 0,recpid=0;
playerinfo himinfo = {.x=MAPSIZE/2,.y=MAPSIZE/2};     //人类信息
playerinfo ghostinfo = {.x=(MAPSIZE/2)+1,.y=MAPSIZE/2};   //鬼信息

