#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "head.h"
#define SIZE 50
#define WALL -1
#define PATH 1
#define CENTER_ROOM 0

// 方向数组：上、右、下、左 (步长为2，因为我们在逻辑网格上跳跃)
// 注意：为了适配 100x100 的稠密显示，我们采用一种映射策略：
// 逻辑生成使用 50x50 的单元格网络，每个单元格对应实际数组的 2x2 区域或者通过插值。
// 但为了最简单的“单像素墙壁、单像素道路”且保证联通，通常使用奇数尺寸生成再裁剪，
// 或者直接在一个大数组上进行“挖墙”操作。
// 这里采用标准递归回溯，但在 100x100 的偶数尺寸上需要特殊处理边界。
// 策略：我们在 0..99 范围内，以 2 为步长生成主节点，确保 (1,1) 到 (97,97) 的奇数坐标作为通道中心。

int dx[4] = {0, 2, 0, -2};
int dy[4] = {2, 0, -2, 0};

// 检查坐标是否在有效生成范围内 (必须是奇数坐标作为节点)
int isValid(int x, int y) {
    return (x >= 1 && x < SIZE - 1 && y >= 1 && y < SIZE - 1 && x % 2 == 1 && y % 2 == 1);
}

// 递归回溯生成迷宫
void generateMaze(int x, int y) {
    maps[x][y] = PATH; // 标记当前点为通路 (1)

    // 随机打乱方向顺序，增加随机性
    int dirOrder[4] = {0, 1, 2, 3};
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = dirOrder[i];
        dirOrder[i] = dirOrder[j];
        dirOrder[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int dir = dirOrder[i];
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (isValid(nx, ny) && maps[nx][ny] == WALL) {
            // 打通当前的墙 (中间点)
            maps[x + dx[dir] / 2][y + dy[dir] / 2] = PATH;
            // 递归进入下一个节点
            generateMaze(nx, ny);
        }
    }
}

void createMaze() {
    // 1. 初始化全部为墙壁 (-1)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            maps[i][j] = WALL;
        }
    }

    // 2. 从 (1,1) 开始生成迷宫
    // 由于 SIZE=100 是偶数，我们生成的有效范围是奇数坐标 1, 3, ..., 97
    // 这样可以保证最外圈 (0 和 99) 始终是墙壁，形成封闭迷宫
    srand(time(NULL));
    generateMaze(1, 1);

    // 3. 处理中心 3x3 区域 (50,50) 为中心
    // 坐标范围：行 49, 50, 51; 列 49, 50, 51
    int centerX = 25;
    int centerY = 25;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int r = centerX + i;
            int c = centerY + j;
            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
                maps[r][c] = CENTER_ROOM; // 设置为 0
            }
        }
    }
    
    // 注意：由于迷宫生成算法保证了所有 PATH(1) 是连通的。
    // 我们将中心的 3x3 强制设为 0，这相当于把原来的路或墙挖空成了房间。
    // 只要这个房间边缘至少有一个点原本连接着 PATH(1)，或者房间内部覆盖了原本的 PATH，
    // 连通性就保持。
    // 在递归回溯法中，(50,50) 附近必然有路径经过或被打通。
    // 即使 (50,50) 原本是个墙，我们把它挖成 0，它周围如果是 1，则自然连通。
    // 如果它周围全是 -1 (不可能，因为迷宫是全覆盖的)，算法会保证至少有一个入口。
    // 实际上，因为 50 是偶数，而我们的生成节点是奇数 (49, 51...)，
    // (50,50) 处于四个生成节点的中间。
    // 生成器会打通 (49,50), (51,50), (50,49), (50,51) 中的某些墙壁。
    // 所以 3x3 的 0 区域一定会与周围的 1 区域相连。
}

void printMaze() {
    printf("int maze[%d][%d] = {\n", SIZE, SIZE);
    for (int i = 0; i < SIZE; i++) {
        printf("    {");
        for (int j = 0; j < SIZE; j++) {
            printf("%2d", maps[i][j]);
            if (j < SIZE - 1) {
                printf(", ");
            }
        }
        printf("}");
        if (i < SIZE - 1) {
            printf(",");
        }
        printf("\n");
    }
    printf("};\n");
}

// int main() {
//     createMaze();
    
//     // 输出数组初始化代码
//     printMaze();

//     return 0;
// }
