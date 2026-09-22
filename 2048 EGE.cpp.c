#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

#define MaxDiff 10
int board[MaxDiff][MaxDiff]={0};//地图，存2的幂
int lineBoard[MaxDiff]={0};
int difficulty = 4;//难度，可以是3-5阶，一般为4阶
int score;//分数，即合并后数之和

// 2048算法
void Fresh()
{
	int r, c;
	while(1)
	{
		r = rand() % difficulty;
		c = rand() % difficulty;
		if(board[r][c] == 0)
		{
			board[r][c] = 1 + (rand()%10 == 0);//随机放置2或4
			break;
		}
	}
}

void Init()
{
	int r, c;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			board[r][c] = 0;//重置
		}
	}
	score = 0;
	srand(time(NULL));
	Fresh();//生成2个起始块
	Fresh();
}

int CanMove()
{
	int r, c;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			if(board[r][c] == 0) return 1;//存在空或并排等数
			if(c+1 < difficulty && board[r][c] == board[r][c+1]) return 1;
			if(r+1 < difficulty && board[r][c] == board[r+1][c]) return 1;
		}
	}
	return 0;
}

int MergeLine()
{
	int i, i1, i2, b;
	int isChanged = 0;
	for(i=0; i<difficulty; i++)
	{
		for(i1=i; i1<difficulty && lineBoard[i1] == 0; i1++);//找第1个数
		if(i1 == difficulty) break;//不存在第1个数
		for(i2=i1+1; i2<difficulty && lineBoard[i2] == 0; i2++);//找第2个数
		if(i2 == difficulty)//不存在第2个数
		{
			if(i1 != i)//移动i1到i
			{
				lineBoard[i] = lineBoard[i1];
				lineBoard[i1] = 0;
				isChanged++;
			}
			break;//全部工作已结束
		}
		if(lineBoard[i1] == lineBoard[i2])//合并
		{
			b = lineBoard[i1]+1;//通过b暂存，合并考虑i1和i是否相同
			lineBoard[i1] = 0;
			lineBoard[i2] = 0;
			lineBoard[i] = b;
			isChanged++;
			score += (1 << b);
		}
		else//两数不同，只移动第1个数，第2个数在下次循环时考虑
		{
			if(i1 != i)//移动i1到i
			{
				lineBoard[i] = lineBoard[i1];
				lineBoard[i1] = 0;
				isChanged++;
			}
		}
	}
	return isChanged;
}

int Move(char direction)
{
	int r, c;
	int isChanged = 0;
	if(direction == 'a')
	{
		for(r=0; r<difficulty; r++)
		{
			for(c=0; c<difficulty; c++) lineBoard[c] = board[r][c];
			isChanged += MergeLine();
			for(c=0; c<difficulty; c++) board[r][c] = lineBoard[c];
		}
	}
	else if(direction == 'd')
	{
		for(r=0; r<difficulty; r++)
		{
			for(c=0; c<difficulty; c++) lineBoard[difficulty-1-c] = board[r][c];
			isChanged += MergeLine();
			for(c=0; c<difficulty; c++) board[r][c] = lineBoard[difficulty-1-c];
		}
	}
	else if(direction == 'w')
	{
		for(c=0; c<difficulty; c++)
		{
			for(r=0; r<difficulty; r++) lineBoard[r] = board[r][c];
			isChanged += MergeLine();
			for(r=0; r<difficulty; r++) board[r][c] = lineBoard[r];
		}
	}
	else if(direction == 's')
	{
		for(c=0; c<difficulty; c++)
		{
			for(r=0; r<difficulty; r++) lineBoard[difficulty-1-r] = board[r][c];
			isChanged += MergeLine();
			for(r=0; r<difficulty; r++) board[r][c] = lineBoard[difficulty-1-r];
		}
	}
	return isChanged;
}

// EGE专用
int sideLength = 96;
void DrawBlock(int x, int y, int number)
{
	int edgeWidth = sideLength/16;
	int fontSize;
	color_t fontColor, fillColor;
	static const color_t fillColorList[16] = {
		EGERGB(238, 228, 218),//0
		EGERGB(238, 228, 218),//2
		EGERGB(237, 224, 200),//4
		EGERGB(242, 177, 121),//8
		EGERGB(245, 149, 99),//16
		EGERGB(246, 124, 95),//32
		EGERGB(246, 94, 59),//64
		EGERGB(237, 207, 114),//128
		EGERGB(237, 204, 97),//256
		EGERGB(237, 200, 80),//512
		EGERGB(237, 197, 63),//1024
		EGERGB(237, 194, 46),//2048
		EGERGB(145, 0, 207),//4096
		EGERGB(89, 0, 128),//8192
		EGERGB(54, 0, 77),//16384
		BLACK//>32768
	};
	if(number < 10) fontSize = sideLength*3/4;
	else if(number < 100) fontSize = sideLength*2/3;
	else if(number < 1000) fontSize = sideLength/2;
	else fontSize = sideLength/3;
	int i = 0;
	for(; number > (1 << i); i++);
	if(number == 3) i--;
	if(i > 15) fillColor = fillColorList[15];
	else fillColor = fillColorList[i];
	if(i < 3) fontColor = GRAY;
	else fontColor = WHITE;
	// 背景和边框
	setfillcolor(PAPAYAWHIP);
	ege_fillrect(x, y, sideLength, sideLength);
	setfillcolor(fillColor);
	ege_fillroundrect(x+edgeWidth, y+edgeWidth, sideLength-2*edgeWidth, sideLength-2*edgeWidth, edgeWidth);
	// 数字
	if(number != 1)
	{
		setfont(fontSize, 0, "Consolas");
		setcolor(fontColor);
		rectprintf(x+edgeWidth, y+edgeWidth, sideLength-2*edgeWidth, sideLength-2*edgeWidth, "%d", number);
	}
}

void DrawBoard()
{
	int r, c;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			DrawBlock(sideLength*c, sideLength*r, 1 << board[r][c]);
			//DrawBlock(sideLength*c, sideLength*r, 1 << (r*difficulty+c));
		}
	}
}

void DrawDisc()//绘制滑盘
{
	int i;
	int cx1 = sideLength*difficulty;
	int cy1 = 0;
	int cd = sideLength*3;
	int cr = cd/2;
	setfillcolor(PINK);
	for(i=0; i<4; i++)
	{
		ege_fillpie(cx1, cy1, cd, cd, i*90+45+5, 80);
	}
	//挖出中洞和字符
	setfillcolor(WHITE);
	ege_fillcircle(cx1+cr, cy1+cr, cr/3);
	setfont(sideLength*3/4, 0, "Consolas");
	setcolor(WHITE);
	outtextxy(cx1+cr, cy1+cr/3, 'W');
	outtextxy(cx1+cr/3, cy1+cr, 'A');
	outtextxy(cx1+cr, cy1+cd-cr/3, 'S');
	outtextxy(cx1+cd-cr/3, cy1+cr, 'D');
}

void DrawSelection()
{
	DrawBlock(sideLength*0, 0, 3);
	DrawBlock(sideLength*1, 0, 4);
	DrawBlock(sideLength*2, 0, 5);
}

void DrawEnd()
{
	if(difficulty == 3) resizewindow(sideLength*(difficulty+3), sideLength*(difficulty+1));
	DrawBoard();
	DrawDisc();
	setfont(sideLength*3/4, 0, "Consolas");
	setcolor(GRAY);
	if(difficulty < 3) rectprintf(0, sideLength*difficulty, sideLength*difficulty, sideLength, "%d", score);
	else rectprintf(sideLength*difficulty, sideLength*3, sideLength*3, sideLength, "%d", score);
}

void Resize(char mode)//调整显示大小
{
	if(mode == '+')//32-96时每格调整4，96+时16
	{
		if(sideLength >= 96) sideLength += 16;
		else sideLength += 4;
	}
	else if(mode == '-')
	{
		if(sideLength > 96) sideLength -= 16;
		else if(sideLength > 32) sideLength -= 4;
	}
}

int main()
{
	char direction;
	mouse_msg mouseMsg;
	key_msg keyMsg;
	int isSliding = 0;
	//自适应显示大小
	int screenHeight, screenWidth;
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	if(EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) == 0)//无法获取显示屏分辨率
	{
		screenWidth = 1920;
		screenHeight = 1080;
	}
	else
	{
		screenWidth = dm.dmPelsWidth;
		screenHeight = dm.dmPelsHeight;
	}
	if(screenHeight >= 2160) sideLength = 192;
	else if(screenHeight >= 1440) sideLength = 128;
	else if(screenHeight >= 1080) sideLength = 96;
	else sideLength = 72;
	//初始化窗口
	setcaption("2048");
	SetProcessDPIAware();
	initgraph(sideLength*3, sideLength, INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	ege_enable_aa(true);
	for(difficulty = 0; difficulty == 0;)
	{
		DrawSelection();
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_up())
			{
				difficulty = mouseMsg.x/sideLength + 3;
			}
			if(mouseMsg.is_wheel() && keystate(key_control))
			{
				Resize(mouseMsg.wheel > 0 ? '+' : '-');
				resizewindow(sideLength*3, sideLength);
				DrawSelection();
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)
			{
				if(keyMsg.key >= '1' && keyMsg.key <= '9')
				{
					difficulty = keyMsg.key-'1'+1;
				}
				else if(keyMsg.key >= key_num1 && keyMsg.key <= key_num9)//小键盘
				{
					difficulty = keyMsg.key-key_num1+1;
				}
			}
		}
		delay_ms(100);
	}
	if(difficulty < 2) difficulty = 2;//1阶会直接输
	if(difficulty > MaxDiff) difficulty = MaxDiff;
restart:
	Init();//初始化
	//初始化窗口
	resizewindow(sideLength*(difficulty+3), sideLength*(difficulty < 3 ? 3 : difficulty));
	DrawBoard();
	DrawDisc();
	delay_ms(0);
	while(CanMove())//判断终局
	{
		//获取操作
		for(direction = 0; direction == 0;)//无操作时在此处循环
		{
			while(kbmsg())
			{
				keyMsg = getkey();
				if(keyMsg.msg == key_msg_down)
				{
					direction = keyMsg.key;
					if(direction == 'W' || direction == 'A' || direction == 'S' || direction == 'D')
					{
						direction = direction-'A'+'a';
						break;
					}
					else if(direction == key_left || direction == key_up
						|| direction == key_right || direction == key_down)
					{
						if(direction == key_left) direction = 'a';
						else if(direction == key_up) direction = 'w';
						else if(direction == key_right) direction = 'd';
						else if(direction == key_down) direction = 's';
						break;
					}
					else if(direction == 'R')
					{
						Init();
						DrawBoard();
					}
					else
					{
						direction = 0;
					}
				}
			}
			while(mousemsg())//鼠标滑盘操作
			{
				mouseMsg = getmouse();
				if(mouseMsg.is_down()) isSliding = 1;
				if(mouseMsg.is_up()) isSliding = 0;
				if(isSliding)
				{
					int xc = mouseMsg.x - sideLength*difficulty;
					int yc = mouseMsg.y;
					int dc = sideLength*3;
					if(xc >= 0 && xc < dc && yc >= 0 && yc < dc)
					{
						//将坐标转译成方向
						int index = (yc > xc) + 2*(yc > dc-1-xc);//存储分别与主副对角线的比较结果
						direction = "wads"[index];
						break;
					}
					else isSliding = 0;
				}
				if(mouseMsg.is_wheel() && keystate(key_control))
				{
					Resize(mouseMsg.wheel > 0 ? '+' : '-');
					resizewindow(sideLength*(difficulty+3), sideLength*(difficulty < 3 ? 3 : difficulty));
					DrawBoard();
					DrawDisc();
				}
			}
			if(!isSliding) delay_ms(25);//最好延时一下
		}
		if(Move(direction))//移动，仅移动有效时刷新块
		{
			Fresh();//刷新块
			DrawBoard();//刷新显示
			delay_ms(0);
		}
	}
	//终局，显示分数
	DrawEnd();
	delay_ms(100);
	while(1)
	{
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_wheel() && keystate(key_control))
			{
				Resize(mouseMsg.wheel > 0 ? '+' : '-');
				resizewindow(sideLength*(difficulty+3), sideLength*(difficulty < 3 ? 3 : difficulty));
				DrawEnd();
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)
			{
				if(keyMsg.key == 'R')
				{
					cleardevice();
					isSliding = 0;
					goto restart;
				}
			}
		}
		delay_ms(50);
	}
	closegraph();
	return 0;
}

/*--------------------------------
更新日志：
2048 0.2
——优化 出4概率由50%下降至10%
——优化 不再连续合成，如2222将合成4400而不是8000
2048 0.3
——优化 生成2个起始块
——优化 无效操作不再刷出新块
——优化 刷新率从10Hz提高到40Hz
2048 0.4
——新增 分数
2048 EGE 0.5
——新增 滑盘
——新增 显示大小的自适应和Ctrl+滚轮调整
——新增 按数字键选择阶数
——新增 按R重玩一局
——优化 重新设计主题配色
——修复 65536开始的背景色
--------------------------------*/
