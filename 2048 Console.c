#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

int board[5][5]={0};//地图，存2的幂
int lineBoard[5]={0};
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

// 控制台专用
int Place(int n)//计算某数所占位数
{
	int i = 0;
	if(n == 0) return 1;
	while(n > 0)
	{
		i++;
		n /= 10;
	}
	return i;
}

void PrintBlank(int n)//打印n个空格
{
	for(; n > 0; n--)
	{
		putchar(' ');
	}
}

int main()
{
	char direction;
	printf("[difficulty:3/4/5]>");
	scanf("%d", &difficulty);
	if(difficulty < 3) difficulty = 3;
	if(difficulty > 5) difficulty = 5;
	Init();//初始化
	//初始化窗口
	while(1)
	{
		if(NoSpace()) break;//判断终局1
		Fresh();//刷新
		//刷新显示
		int r, c, maxPlace = 0;
		for(r=0; r<difficulty; r++)
		{
			for(c=0; c<difficulty; c++)
			{
				if(Place(1 << board[r][c]) > maxPlace)
				{
					maxPlace = Place(1 << board[r][c]);
				}
			}
		}
		for(r=0; r<difficulty; r++)
		{
			for(c=0; c<difficulty; c++)
			{
				if(board[r][c] == 0)
				{
					PrintBlank(maxPlace+1);
				}
				else
				{
					PrintBlank(maxPlace-Place(1 << board[r][c]));
					printf("%d ", 1 << board[r][c]);
				}
			}
			printf("\n");
		}
		printf("----------------\n");
		if(!CanMove()) break;//判断终局2
		//获取操作
		for(direction = 0; direction == 0;)
		{
			if(kbhit())
			{
				direction = getch();
				if(direction == 'w' || direction == 'a' || direction == 's' || direction == 'd')
				{
					break;
				}
				else
				{
					direction = 0;
				}
			}
			Sleep(100);//最好延时一下
		}
		Move(direction);//移动
	}
	//终局，可以显示分数
	system("pause");
	return 0;
}

/*--------------------------------
更新日志：
2048 0.2
——优化 出4概率由50%下降至10%
——优化 不再连续合成，如2222将合成4400而不是8000
--------------------------------*/
