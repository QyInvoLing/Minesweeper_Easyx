#pragma once
#define	PIXEL	20	//����ÿ��ı߳�
#define	GRADE_ROW	16	//����
#define GRADE_COL	30	//����
#define	MINE_NUM	99	//��������
#include<windows.h>

//���ӵĽṹ��
typedef struct

{
	int		mine_num;	//��Χ��������
	bool	has_mine;	//true��ʾ����
	bool	checked;	//true��ʾ���ھ�
	bool	flag;		//true��ʾ�ѱ��
}MapBlock;

//ͼƬָ�롣���ڴ�����Ϸ��ͼ
IMAGE img, mine, none, flag, menu;

//��ʼ����ͼ
void CreatMap(MapBlock map[GRADE_COL][GRADE_ROW], int a, int b)//a��b�����һ�ε�������ꡣ��һ��������;�������������ͼ������Ϣ
{
	srand(time(0));

	//��ʼ����ͼ������0��Ϊ����
	for (int i = 0; i < GRADE_COL; i++)
		for (int j = 0; j < GRADE_ROW; j++)
		{
			map[i][j].mine_num = 0;
			map[i][j].has_mine = false;
			map[i][j].checked = false;
			map[i][j].flag = false;
		}

	//����������ظ�MINE_NUM������λ��

	int num = 1, temp;
	int mine[MINE_NUM + 1];//������λΪ��һ�ε�������꣬��������Χ��������
	//����������ڴ�žŸ����֡���ŵ����ֵȻ�����������ɵ�������Ƚϣ�ȷ����һ�ε㵽�ĸ�����Χ��8������Լ��������ס�
	int checList[] = {0,-1,1,-GRADE_COL,-1- GRADE_COL ,1- GRADE_COL ,1+ GRADE_COL ,GRADE_COL ,GRADE_COL -1};
	mine[0] = b * GRADE_COL + a;
	//�������ɵ�MINE_NUM������λ�ò�����֮ǰ��ͬ
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

	//��������MINE_NUM����������������
	for (int i = 1; i <= MINE_NUM; i++)
	{
		map[mine[i] % (GRADE_COL)][mine[i] / (GRADE_COL)].has_mine = true;
	}

	//�������׵��߼��ֲ�ͼ

	int around_mine_num;
	
	for (int i = 0; i < GRADE_COL; i++)
		for (int j = 0; j < GRADE_ROW; j++)
		{

			//��������ʱ�ж�����Χ����

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

//������Ӻ�ݹ�ɸѡ��������
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