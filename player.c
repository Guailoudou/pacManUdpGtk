/*
 * @Date: 2026-03-19 14:37:29
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-19 19:04:05
 * @FilePath: /loudou/file/2JIEDUAN/player.c
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
            if(maps[himinfo.x][himinfo.y-1] == 1){
                maps[himinfo.x][himinfo.y-1] = 0;
                himinfo.sc++;
            }
            himinfo.y--;
        }
    }else{
        if(maps[ghostinfo.x][ghostinfo.y-1] != -1){
            ghostinfo.y--;
        }
    }
}
void down() //下
{
    if(ishuman){
        if(maps[himinfo.x][himinfo.y+1] != -1){
            if(maps[himinfo.x][himinfo.y+1] == 1){
                maps[himinfo.x][himinfo.y+1] = 0;
                himinfo.sc++;
            }
            himinfo.y++;
        }
    }else{
        if(maps[ghostinfo.x][ghostinfo.y+1] != -1){
            ghostinfo.y++;
        }
    }
}

void left() //左
{
    if(ishuman){
        if(maps[himinfo.x-1][himinfo.y] != -1){
            if(maps[himinfo.x-1][himinfo.y] == 1){
                maps[himinfo.x-1][himinfo.y] = 0;
                himinfo.sc++;
            }
            himinfo.x--;
        }
    }else{
        if(maps[ghostinfo.x-1][ghostinfo.y] != -1){
            ghostinfo.x--;
        }
    }
}

void right() //右边
{
    if(ishuman){
        if(maps[himinfo.x+1][himinfo.y] != -1){
            if(maps[himinfo.x+1][himinfo.y] == 1){
                maps[himinfo.x+1][himinfo.y] = 0;
                himinfo.sc++;
            }
            himinfo.x++;
        }
    }else{
        if(maps[ghostinfo.x][ghostinfo.y] != -1){
            ghostinfo.x++;
        }
    }
}