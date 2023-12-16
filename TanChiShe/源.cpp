#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include "game.h"


#define X 500
#define Y 500
#define SNAKE_NUM X*Y//蛇的最大节数

enum DIR
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

struct Snake 
{
	int size;//有多少节
	int dir=RIGHT;//蛇的方向
	int speed;//蛇的速度
	int r;//蛇节半径
	POINT coor[SNAKE_NUM];//每节的坐标
}snake;


struct Food
{
	int x;
	int y;
	int r;//食物大小
	bool flag;//判定食物被吃了没有
	DWORD color;//食物颜色
}food;

//初始化数据
void GameInit()
{
	//init初始化graph图形窗口
	initgraph(X, Y);

	//设置随机数种子
	srand(GetTickCount());

	//初始化蛇
	snake.size = 3;
	snake.speed = 10;
	snake.dir;
	snake.r = 5;
	for (int i = 0; i < snake.size ; i++)
	{
		snake.coor[i].x = 40-10*i;
		snake.coor[i].y = 10;
	}

	//初始化食物
	food.x = rand() % X;		
	food.y = rand() % Y;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.r = rand() % 10 + 5;
	food.flag = true;
}


//绘制图形(分开一部分好操作)
void GameDraw()
{
	//双缓冲绘图
	BeginBatchDraw();

	//设置窗口颜色
	setbkcolor(RGB(19, 128, 113));

	//刷新才能显色
	cleardevice();

	//绘制蛇
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, snake.r);
	}
	
	EndBatchDraw();
}
//绘制食物
void DrawFood()
{
	BeginBatchDraw();
	if (food.flag)
	{
		setfillcolor(food.color);
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();
}


void snakeMove()
{
	for (int i = snake.size-1; i >0; i--)//从尾部开始向头部赋值可以拉开变化的重叠
	{
		snake.coor[i] = snake.coor[i - 1];
	}
	
	switch (snake.dir)
	{
	case RIGHT:
		snake.coor[0].x += snake.speed;
		if (snake.coor[0].x-10 == X)
		{
			snake.coor[0].x = 0;
		}
		break;

	case LEFT:
		snake.coor[0].x -= snake.speed;
		if (snake.coor[0].x+10 == 0)
		{
			snake.coor[0].x = X;
		}

		break;

	case UP:
		snake.coor[0].y -= snake.speed;
		if (snake.coor[0].y+10 == 0)
		{
			snake.coor[0].y = Y;
		}
		break;

	case DOWN:
		snake.coor[0].y += snake.speed;
		if (snake.coor[0].y-10 == Y)
		{
			snake.coor[0].y = 0;
		}
		break;
	}
}


//食物被吃判定
void EatFood()
{
	if (/*snake.coor[0].x>=food.x-food.r&& snake.coor[0].x <= food.x + food.r&& 
		snake.coor[0].y >= food.y - food.r&& snake.coor[0].y <= food.y + food.r*/ 
		(snake.coor[0].x - food.x) * (snake.coor[0].x - food.x)
		+ (snake.coor[0].y - food.y) * (snake.coor[0].y - food.y)
		<= (snake.r + food.r) * (snake.r + food.r)
		&&food.flag)
	{
		food.flag = false;
		snake.size++;
	}
	if (!food.flag)
	{
		//重新初始化食物
		food.x = rand() % X;	
		food.y = rand() % Y;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
		food.flag = true;
	}
}


//通过按键控制蛇的方向
void keyControl()
{
	//72 80 75 77上下左右的键值
	
	//这个_getch()是读入一个字符时，才运行一下的
	 
	//所以要先判断有没有按键

	if (_kbhit())//有按键就返回真
	{
		switch (_getch())
		{
		case 'W':
		case 'w':
		case 72:
			if(snake.dir!=DOWN)
			snake.dir = UP;
			break;

		case 'S':
		case 's':
		case 80:
			if (snake.dir != UP)
			snake.dir = DOWN;
			break;

		case 'A':
		case 'a':
		case 75:
			if (snake.dir !=RIGHT)
			snake.dir = LEFT;
			break;

		case 'D':
		case 'd':
		case 77:
			if (snake.dir != LEFT)
			snake.dir = RIGHT;
			break;
		
		//暂停设置
		case ' ':
			while (_getch() == ' ')
				break;
		}

	}
}


//int main()
//{
//	GameInit();
//	while (1)
//	{
//		snakeMove();
//		GameDraw();
//		DrawFood();
//		keyControl();
//		EatFood();
//		Sleep(100);
//	}
//	
//	return 0;
//}

/*
	setcolor(WHITE);
	setfillcolor(YELLOW);
	fillcircle(100, 100, 50);//圆形颜色填充
	line(0, 0, 300, 300);//画一条线
	fillrectangle(350, 300, 600, 650);//矩形颜色填充
	solidrectangle(30, 300, 60, 650);//实心矩形
	while (1);
	closegraph();
	return 0;
	*/