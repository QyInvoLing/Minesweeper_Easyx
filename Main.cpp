#include "stdio.h"
#include "graphics.h"
#include "conio.h"
#include "time.h"
#include "MapTool.h"

//声明一下函数
void GameOver(MapBlock map[GRADE_COL][GRADE_ROW],int a);

//主循环
void gameRun() {
	MapBlock map[GRADE_COL][GRADE_ROW];
	

	for (int i = 0; i < GRADE_COL; i++)
		for (int j = 0; j < GRADE_ROW; j++)
			putimage(i * PIXEL, j * PIXEL, &img);
	//第一次不会踩雷
	int tryamount = 0;
	while (1){
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x < PIXEL * GRADE_COL)
		{
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:

				
				if (tryamount == 0) {//第一次保护
					tryamount++;
					CreatMap(map, m.x / PIXEL, m.y / PIXEL);
				}
				//当挖到地雷时
				if (map[m.x / PIXEL][m.y / PIXEL].has_mine == true && map[m.x / PIXEL][m.y / PIXEL].flag == false)
				{
					putimage((m.x / PIXEL) * PIXEL, (m.y / PIXEL) * PIXEL, &mine);
					GameOver(map,0);
				}

				//没有挖到地雷

				else if (map[m.x / PIXEL][m.y / PIXEL].has_mine == false && map[m.x / PIXEL][m.y / PIXEL].flag == false)
				{

					//周围有地雷

					if (map[m.x / PIXEL][m.y / PIXEL].mine_num != 0)
					{
						putimage((m.x / PIXEL) * PIXEL, (m.y / PIXEL) * PIXEL, &none);
						map[m.x / PIXEL][m.y / PIXEL].checked = true;
						char c = char(map[m.x / PIXEL][m.y / PIXEL].mine_num + 48);	//ASCII
						
						setbkmode(TRANSPARENT);
						//在指定格子上标注周围地雷数
						RECT r = { (m.x / PIXEL) * PIXEL, (m.y / PIXEL) * PIXEL, (m.x / PIXEL + 1) * PIXEL,(m.y / PIXEL + 1) * PIXEL };
						drawtext(c, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}

					//周围无雷

					else
					{
						putimage((m.x / PIXEL) * PIXEL, (m.y / PIXEL) * PIXEL, &none);
						find_around(map, m.x / PIXEL, m.y / PIXEL);
					}
					//判断赢了没
					int win = 1;
					for (int i = 0; i < GRADE_COL; i++){
						for (int j = 0; j < GRADE_ROW; j++){
							if (!map[i][j].checked && !map[i][j].has_mine) {
								win = 0;
							}
						}
				    }
					if (win == 1) {
						GameOver(map, 1);
					}
				}
				break;
			case WM_RBUTTONDOWN://插旗子
				if (map[m.x / PIXEL][m.y / PIXEL].checked == false && map[m.x / PIXEL][m.y / PIXEL].flag == false)
				{
					putimage((m.x / PIXEL) * PIXEL, (m.y / PIXEL) * PIXEL, &flag);
					map[m.x / PIXEL][m.y / PIXEL].flag = true;
				}
				else if (map[m.x / PIXEL][m.y / PIXEL].checked == false && map[m.x / PIXEL][m.y / PIXEL].flag == true)
				{
					putimage((m.x / PIXEL) * PIXEL, (m.y / PIXEL) * PIXEL, &img);
					map[m.x / PIXEL][m.y / PIXEL].flag = false;
				}
				break;
			}
		}
	}
}

//游戏结束
void GameOver(MapBlock map[GRADE_COL][GRADE_ROW],int win=1){
	//显示所有地雷
	for (int i = 0; i < GRADE_COL; i++)
		for (int j = 0; j < GRADE_ROW; j++)
		{
			if (map[i][j].has_mine == true)
				putimage(i * PIXEL, j * PIXEL, &mine);
		}
	Sleep(1000);
	//win
	if (win) {
		int isok = MessageBox(GetHWnd(), _T("You win! Play again?"), _T("Game Over"), MB_OKCANCEL);
		if (isok == IDOK)
		{
			gameRun();
		}
		else
		{
			exit(0);
		}
	}
	else {
		int isok = MessageBox(GetHWnd(), _T("You lose! Play again?"), _T("Game Over"), MB_OKCANCEL);
		if (isok == IDOK)
		{
			gameRun();
		}
		else
		{
			exit(0);
		}
	}

}

//程序入口
int main()
{
	HWND hwnd = initgraph(PIXEL * GRADE_COL, PIXEL * GRADE_ROW);
	SetWindowTextA(hwnd, "Minesweeper");
	
	//加载图像
	loadimage(&img, _T("./image/unknow.png"), PIXEL, PIXEL);
	loadimage(&mine, _T("./image/mine.png"), PIXEL, PIXEL);
	loadimage(&none, _T("./image/no.png"), PIXEL, PIXEL);
	loadimage(&flag, _T("./image/flag.png"), PIXEL, PIXEL);
	loadimage(&menu, _T("./image/menu.png"), PIXEL, PIXEL);
	//设置字体颜色
	setcolor(0x000000);
	gameRun();
	closegraph();
	return 0;
}

