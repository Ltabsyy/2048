#include <stdio.h>
#include <stdlib.h>

int board[5][5]={0};//地图，存2的幂
int difficulty = 4;//难度，可以是3-5阶，一般为4阶

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
	int r, c;
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
		
	}
	else if(direction == 'w')
	{
		
	}
	else if(direction == 's')
	{
		
	}
}

int main()
{
	Init();//初始化
	//初始化窗口
	while(1)
	{
		Fresh();//刷新
		//刷新显示
		if(!CanMove()) break;//判断终局
		//获取操作
		Move('a');//移动
	}
	//终局，可以显示分数
	return 0;
}
