#pragma once
#define	PIXEL	20	//设置每格的边长
#define	GRADE_ROW	16	//行数
#define GRADE_COL	30	//列数
#define	MINE_NUM	99	//地雷数量
#include<windows.h>

//格子的结构体
typedef struct

{
	int		mine_num;	//周围地雷数量
	bool	has_mine;	//true表示有雷
	bool	checked;	//true表示已挖掘
	bool	flag;		//true表示已标记
}MapBlock;

//图片指针。用于储存游戏贴图
IMAGE img, mine, none, flag, menu;

//初始化地图
void CreatMap(MapBlock map[GRADE_COL][GRADE_ROW], int a, int b)//a和b代表第一次点击的坐标。这一格不能有雷;其他参数代表地图基础信息
{
	srand(time(0));

	//初始化地图、并将0视为无雷
	for (int i = 0; i < GRADE_COL; i++)
		for (int j = 0; j < GRADE_ROW; j++)
		{
			map[i][j].mine_num = 0;
			map[i][j].has_mine = false;
			map[i][j].checked = false;
			map[i][j].flag = false;
		}

	//随机产生不重复MINE_NUM个地雷位置

	int num = 1, temp;
	int mine[MINE_NUM + 1];//数组首位为第一次点击的坐标，它和它周围不能有雷
	//这个数组用于存放九个数字。存放的数字等会儿拿来和生成的随机数比较，确保第一次点到的格子周围的8格和它自己不能有雷。
	int checList[] = {0,-1,1,-GRADE_COL,-1- GRADE_COL ,1- GRADE_COL ,1+ GRADE_COL ,GRADE_COL ,GRADE_COL -1};
	mine[0] = b * GRADE_COL + a;
	//后面生成的MINE_NUM个地雷位置不能与之前相同
	while (num <= MINE_NUM) {
		int swit = 0;
		while (swit != 1) {
			swit = 1;
			temp = rand() % (GRADE_ROW * GRADE_COL);
			for (int i = 0; i < num; i++) {
				int a = temp - mine[0];
				for (int n = 0; n < 9; n++) {
					if (a ==checList[n]) {
						swit = 0;
						break;
					}
				}
				if (swit == 0) {
					break;
				}
			}
		}
		mine[num] = temp;
		num++;
	}

	//将产生的MINE_NUM个地雷置入雷区。
	for (int i = 1; i <= MINE_NUM; i++)
	{
		map[mine[i] % (GRADE_COL)][mine[i] / (GRADE_COL)].has_mine = true;
	}

	//产生地雷的逻辑分布图

	int around_mine_num;
	
	for (int i = 0; i < GRADE_COL; i++)
		for (int j = 0; j < GRADE_ROW; j++)
		{

			//本块无雷时判断其周围雷数

			if (!map[i][j].has_mine)
			{
				around_mine_num = 0;
				for (int I = -1; I <= 1; I++)
					for (int J = -1; J <= 1; J++)
					{
						if ((i + I) >= 0 && (j + J) >= 0 && (i + I) < GRADE_COL && (j + J) < GRADE_ROW)
						{
							if (map[i + I][j + J].has_mine)
								around_mine_num++;
						}
					}
				map[i][j].mine_num = around_mine_num;
			}
		}
}

//点击格子后递归筛选出无雷区
void find_around(MapBlock map[GRADE_COL][GRADE_ROW], int x, int y)
{
	Sleep(500);
	for (int I = -1; I <= 1; I++) {
		for (int J = -1; J <= 1; J++) {
			if ((x + I) >= 0 && (y + J) >= 0 && (x + I) < GRADE_COL && (y + J) < GRADE_ROW) {
				if (map[x + I][y + J].mine_num == 0 && map[x + I][y + J].flag == false && map[x + I][y + J].checked == false) {
					putimage(x * PIXEL, y * PIXEL, &none);
					map[x + I][y + J].checked = true;
					find_around(map, x + I, y + J);
				}
				else if (map[x + I][y + J].mine_num >= 1 && map[x + I][y + J].mine_num <= 8 && map[x + I][y + J].checked == false && map[x + I][y + J].flag == false) {
					putimage((x + I) * PIXEL, (y + J) * PIXEL, &none);
					map[x + I][y + J].checked = true;

					char c = char(map[x + I][y + J].mine_num + 48);//ASCII

					setbkmode(TRANSPARENT);
					RECT r = { (x + I) * PIXEL, (y + J) * PIXEL, (x + I + 1) * PIXEL,(y + J + 1) * PIXEL };
					drawtext(c, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					map[x + I][y + J].checked = true;
				}
			}
		}
	}
	for (int i = 0; i < GRADE_COL; i++) {
		for (int j = 0; j < GRADE_ROW; j++) {
			if (map[i][j].checked) {
				putimage(x * PIXEL, y * PIXEL, &none);
			}
		}
	}
}