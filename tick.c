/*
 * @Date: 2026-03-19 10:49:37
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-23 13:21:09
 * @FilePath: /2JIEDUAN/tick.c
 */
#include "head.h"
#include <sys/time.h>
pthread_t tickpid=0;
void *tick(void *arg) //弃用
{
    struct timeval start, end;
    long seconds, microseconds;
    double elapsed_ms;
    while (isrun)
    {
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
    
    
}
void updata_map(int x, int y){
    int zx = x-RENDERING_RANGE;
    if(zx<=0)zx=1;
    int zy = y-RENDERING_RANGE;
    if(zy<=0)zy=1;
    int yx = x+RENDERING_RANGE;
    if(yx>=MAPSIZE)yx=MAPSIZE-1;
    int yy = y+RENDERING_RANGE;
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
    if(himinfo.x == ghostinfo.x && himinfo.y == ghostinfo.y &&
        !(himinfo.x==MAPSIZE/2 && himinfo.y==MAPSIZE/2))
    {

        isrun = false;
        himinfo.x = 0;
        ghostinfo.x = 1;
        if(ishuman)
        {
            score = himinfo.sc;
        }
        else
        {
            rival_score = himinfo.sc;
        }
        if(score == 0 || rival_score==0){
            printf("反转！\n");
            //////
            ishuman = ishuman?false:true;
            if(!ishuman)usleep(50);
            gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_waiting");
        }
        else
        {
            printf("游戏结束！\n我的分数：%d ，对方分数：%d",score,rival_score);
            gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_checkout");
            close_sock();
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

void runNetTask()
{
    pthread_create(&sendpid, NULL, sendinfo, NULL);
    pthread_create(&recpid, NULL, getinfo, NULL);
}

void runsendmap()
{
    if(ishuman){
        createMap();
        pthread_create(&sendmappid, NULL, sendmap, NULL);
    }else{
        pthread_create(&recmappid, NULL, recvmap, NULL);
    }
}
// void runTick()
// {
//     pthread_create(&tickpid, NULL, tick, NULL);
// }