/*
 * @Date: 2026-03-19 09:52:13
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-19 18:51:52
 * @FilePath: /loudou/file/2JIEDUAN/common.c
 */
#include "head.h"
bool isServer; //是否为房主
bool ishuman = true;  //是否是人类 否为贵
int score;   //分数
int rival_score;  //对方分数
IPV4 addr;
int udp_sock;
int udp_ret;
bool isrun; // 游戏中
int gametime;
playerinfo himinfo = {.x=MAPSIZE/2,.y=MAPSIZE/2};     //人类信息
playerinfo ghostinfo;   //鬼信息

