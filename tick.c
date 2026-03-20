/*
 * @Date: 2026-03-19 10:49:37
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-20 09:26:16
 * @FilePath: /2JIEDUAN/tick.c
 */
#include "head.h"
#include <sys/time.h>
void tick() //废弃
{
    struct timeval start, end;
    long seconds, microseconds;
    double elapsed_ms;
    gettimeofday(&start, NULL); // 开始计时

    tickTask();

    gettimeofday(&end, NULL);   // 结束计时
    // 计算时间差
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    
    // 转换为毫秒
    elapsed_ms = (seconds * 1000.0) + (microseconds / 1000.0);

    gettimeofday(&end, NULL);   // 结束计时
    if(elapsed_ms<50)
        usleep(1000.0*(50.0-elapsed_ms)); //50ms
    else{
        printf("[警告]1tick消耗过多的时间：%f",elapsed_ms);
    }
}
void updata_map(int x, int y){
    int zx = x-1;
    if(zx<=0)zx=1;
    int zy = y-1;
    if(zy<=0)zy=1;
    int yx = x+1;
    if(yx>=MAPSIZE)yx=MAPSIZE-1;
    int yy = y+1;
    if(yy>=MAPSIZE)yy=MAPSIZE-1;
    for(int j=zx;j<=yx;j++){
        for(int k=zy;k<=yy;k++){
            if(view[j][k]==-1)
                update_cell_color(j,k,WALL_BLOCK);
            else if(view[j][k]==0)
                update_cell_color(j,k,AIR_BLOCK);
            else if(view[j][k]==1)
                update_cell_color(j,k,COIN_BLOCK);
            else if(view[j][k]==2)
                update_cell_color(j,k,HIM_BLOCK);
            else if(view[j][k]==3)
                update_cell_color(j,k,GHOST_BLOCK);
        }
    }
    
}
void tickTask()
{
    if(himinfo.x == ghostinfo.x && himinfo.y == ghostinfo.y)
    {
        isrun = false;
        if(ishuman)
        {
            score = himinfo.sc;
        }
        else
        {
            rival_score = himinfo.sc;
        }
    }
    if(maps[himinfo.x][himinfo.y] == 1){
        maps[himinfo.x][himinfo.y] = 0;
        himinfo.sc++;
    }
    if(isrun)
    {
        gametime += 1; //1tick (1/20秒)
        //更新地图
        getview();
        updata_map(himinfo.x,himinfo.y);    //更新人类玩家周围
        updata_map(ghostinfo.x,ghostinfo.y);    //更新鬼周围
        // update_cell_color(himinfo.x,himinfo.y)
    }
}