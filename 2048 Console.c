#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

int board[5][5]={0};//地图，存2的幂
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
			board[r][c] = 1 + rand()%2;//随机放置2或4
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

void Move(char direction)
{
	int r, c, i;
	for(i=0; i<difficulty-1; i++)
	{
		if(direction == 'a')
		{
			for(r=0; r<difficulty; r++)
			{
				for(c=1; c<difficulty; c++)
				{
					if(board[r][c] != 0)
					{
						if(board[r][c-1] == 0)//移动
						{
							board[r][c-1] = board[r][c];
							board[r][c] = 0;
						}
						else if(board[r][c-1] == board[r][c])//合并
						{
							board[r][c-1]++;
							board[r][c] = 0;
						}
					}
				}
			}
		}
		else if(direction == 'd')
		{
			for(r=0; r<difficulty; r++)
			{
				for(c=difficulty-2; c>=0; c--)
				{
					if(board[r][c] != 0)
					{
						if(board[r][c+1] == 0)//移动
						{
							board[r][c+1] = board[r][c];
							board[r][c] = 0;
						}
						else if(board[r][c+1] == board[r][c])//合并
						{
							board[r][c+1]++;
							board[r][c] = 0;
						}
					}
				}
			}
		}
		else if(direction == 'w')
		{
			for(c=0; c<difficulty; c++)
			{
				for(r=1; r<difficulty; r++)
				{
					if(board[r][c] != 0)
					{
						if(board[r-1][c] == 0)//移动
						{
							board[r-1][c] = board[r][c];
							board[r][c] = 0;
						}
						else if(board[r-1][c] == board[r][c])//合并
						{
							board[r-1][c]++;
							board[r][c] = 0;
						}
					}
				}
			}
		}
		else if(direction == 's')
		{
			for(c=0; c<difficulty; c++)
			{
				for(r=difficulty-2; r>=0; r--)
				{
					if(board[r][c] != 0)
					{
						if(board[r+1][c] == 0)//移动
						{
							board[r+1][c] = board[r][c];
							board[r][c] = 0;
						}
						else if(board[r+1][c] == board[r][c])//合并
						{
							board[r+1][c]++;
							board[r][c] = 0;
						}
					}
				}
			}
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
