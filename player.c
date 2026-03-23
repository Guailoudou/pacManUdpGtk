/*
 * @Date: 2026-03-19 14:37:29
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-23 12:35:43
 * @FilePath: /2JIEDUAN/player.c
 */
/*
 * @Date: 2026-03-19 14:37:29
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-19 14:43:42
 * @FilePath: /loudou/file/2JIEDUAN/player.c
 */
#include "head.h"
//Up, down, left, right
// extern int isrun;
// extern int ishuman;
// extern int gametime;
// extern int map[MAPSIZE][MAPSIZE];
// extern playerinfo himinfo;
// extern playerinfo ghostinfo;
void up() //上
{
    if(ishuman){
        if(maps[himinfo.x][himinfo.y-1] != -1){
            himinfo.y--;
        }
    }else{
        if(gametime<20*5)return;
        if(maps[ghostinfo.x][ghostinfo.y-1] != -1){
            ghostinfo.y--;
        }
    }
}
void down() //下
{
    if(ishuman){
        if(maps[himinfo.x][himinfo.y+1] != -1){
            himinfo.y++;
        }
    }else{
        if(gametime<20*5)return;
        if(maps[ghostinfo.x][ghostinfo.y+1] != -1){
            ghostinfo.y++;
        }
    }
}

void left() //左
{
    if(ishuman){
        if(maps[himinfo.x-1][himinfo.y] != -1){
            himinfo.x--;
        }
    }else{
        if(gametime<20*5)return;
        if(maps[ghostinfo.x-1][ghostinfo.y] != -1){
            ghostinfo.x--;
        }
    }
}

void right() //右边
{
    if(ishuman){
        if(maps[himinfo.x+1][himinfo.y] != -1){
            himinfo.x++;
        }
    }else{
        if(gametime<20*5)return;
        if(maps[ghostinfo.x+1][ghostinfo.y] != -1){
            ghostinfo.x++;
        }
    }
}