#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

int board[10][10]={0};//地图，存2的幂
int lineBoard[10]={0};
int difficulty = 4;//难度，可以是3-5阶，一般为4阶

// 2048算法
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
	srand(time(NULL));
}

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

int NoSpace()
{
	int r, c;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			if(board[r][c] == 0) return 0;//存在空
		}
	}
	return 1;
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

void MergeLine()
{
	int i, i1, i2, b;
	for(i=0; i<difficulty; i++)
	{
		for(i1=i; i1<difficulty && lineBoard[i1] == 0; i1++);//找第1个数
		if(i1 == difficulty) break;//不存在第1个数
		if(i1 == difficulty-1)//不存在第2个数
		{
			b = lineBoard[i1];//移动
			lineBoard[i1] = 0;
			lineBoard[i] = b;
			break;
		}
		for(i2=i1+1; i2<difficulty && lineBoard[i2] == 0; i2++);//找第2个数
		if(i2 == difficulty)//不存在第2个数
		{
			b = lineBoard[i1];//移动
			lineBoard[i1] = 0;
			lineBoard[i] = b;
			break;
		}
		if(lineBoard[i1] == lineBoard[i2])//合并
		{
			b = lineBoard[i1]+1;
			lineBoard[i1] = 0;
			lineBoard[i2] = 0;
			lineBoard[i] = b;
		}
		else
		{
			b = lineBoard[i1];//移动第1个数
			lineBoard[i1] = 0;
			lineBoard[i] = b;
		}
	}
}

void Move(char direction)
{
	int r, c, i, i1, i2;
	if(direction == 'a')
	{
		for(r=0; r<difficulty; r++)
		{
			for(c=0; c<difficulty; c++) lineBoard[c] = board[r][c];
			MergeLine();
			for(c=0; c<difficulty; c++) board[r][c] = lineBoard[c];
		}
	}
	else if(direction == 'd')
	{
		for(r=0; r<difficulty; r++)
		{
			for(c=0; c<difficulty; c++) lineBoard[difficulty-1-c] = board[r][c];
			MergeLine();
			for(c=0; c<difficulty; c++) board[r][c] = lineBoard[difficulty-1-c];
		}
	}
	else if(direction == 'w')
	{
		for(c=0; c<difficulty; c++)
		{
			for(r=0; r<difficulty; r++) lineBoard[r] = board[r][c];
			MergeLine();
			for(r=0; r<difficulty; r++) board[r][c] = lineBoard[r];
		}
	}
	else if(direction == 's')
	{
		for(c=0; c<difficulty; c++)
		{
			for(r=0; r<difficulty; r++) lineBoard[difficulty-1-r] = board[r][c];
			MergeLine();
			for(r=0; r<difficulty; r++) board[r][c] = lineBoard[difficulty-1-r];
		}
	}
}

// EGE专用
int sideLength = 96;
void DrawBlock(int x, int y, int number)
{
	int edgeWidth = sideLength/16;
	int fontSize;
	color_t fontColor, fillColor;
	color_t fillColorList[16] = {
		EGERGB(254, 228, 227),//0
		EGERGB(254, 228, 227),//2
		EGERGB(254, 206, 206),//4
		EGERGB(254, 192, 193),//8
		EGERGB(253, 162, 161),//16
		EGERGB(255, 141, 141),//32
		EGERGB(253, 117, 117),//64
		EGERGB(254, 156, 5),//128
		EGERGB(253, 136, 1),//256
		EGERGB(255, 199, 54),//512
		EGERGB(255, 212, 101),//1024
		EGERGB(254, 223, 141),//2048
		EGERGB(242, 175, 210),//4096
		EGERGB(242, 175, 210),//8192
		EGERGB(242, 175, 210),//16384
		EGERGB(242, 175, 210)//>32768
	};
	if(number < 10) fontSize = sideLength*3/4;
	else if(number < 100) fontSize = sideLength*2/3;
	else if(number < 1000) fontSize = sideLength/2;
	else fontSize = sideLength/3;
	fontColor = EGERGB(163, 95, 95);
	int i = 0;
	for(; number > (1 << i); i++);
	fillColor = fillColorList[i];
	// 背景和边框
	setfillcolor(WHITE);
	ege_fillrect(x, y, sideLength, sideLength);
	setfillcolor(fillColor);
	ege_fillroundrect(x+edgeWidth, y+edgeWidth, sideLength-2*edgeWidth, sideLength-2*edgeWidth, edgeWidth);
	// 数字
	if(number != 1)
	{
		setfont(fontSize, 0, "Consolas");
		setcolor(fontColor);
		rectprintf(x, y, sideLength, sideLength, "%d", number);
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
		}
	}
}

int main()
{
	char direction;
	mouse_msg mouseMsg;
	setcaption("2048");
	SetProcessDPIAware();
	initgraph(sideLength*3, sideLength, INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	ege_enable_aa(true);
	for(difficulty = 0; difficulty == 0;)
	{
		DrawBlock(sideLength*0, 0, 3);
		DrawBlock(sideLength*1, 0, 4);
		DrawBlock(sideLength*2, 0, 5);
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_up())
			{
				difficulty = mouseMsg.x/sideLength + 3;
			}
		}
		delay_ms(100);
	}
	if(difficulty < 2) difficulty = 2;//1阶会直接输
	if(difficulty > 10) difficulty = 10;
	Init();//初始化
	//初始化窗口
	resizewindow(sideLength*difficulty, sideLength*difficulty);
	while(1)
	{
		if(NoSpace()) break;//判断终局1
		Fresh();//刷新
		DrawBoard();//刷新显示
		if(!CanMove()) break;//判断终局2
		//获取操作
		for(direction = 0; direction == 0;)//无操作时在此处循环
		{
			if(kbhit())
			{
				direction = getch();
				if(direction == 'w' || direction == 'a' || direction == 's' || direction == 'd')
				{
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
				else
				{
					direction = 0;
				}
			}
			delay_ms(100);//最好延时一下
		}
		Move(direction);//移动
	}
	//终局，可以显示分数
	//resizewindow(sideLength*difficulty, sideLength*(difficulty+1));
	//DrawBoard();
	delay_ms(2000);
	flushkey();
	getch();
	closegraph();
	return 0;
}

/*--------------------------------
更新日志：
2048 0.2
——优化 出4概率由50%下降至10%
——优化 不再连续合成，如2222将合成4400而不是8000
--------------------------------*/
