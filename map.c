/*
 * @Date: 2026-03-19 10:50:41
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-19 18:25:27
 * @FilePath: /loudou/file/2JIEDUAN/map.c
 */
#include "head.h"
extern playerinfo himinfo;
extern playerinfo ghostinfo;

int maps[MAPSIZE][MAPSIZE]={};
int view[MAPSIZE][MAPSIZE]={};
int getview()
{
    memcpy(view, maps, sizeof(maps));
    view[himinfo.x][himinfo.y]=2;
    view[ghostinfo.x][ghostinfo.y]=3;
}
int createMap(){
    //阿巴阿巴，生成地图（
    createMaze();
}