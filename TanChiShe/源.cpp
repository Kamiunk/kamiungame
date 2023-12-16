#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include "game.h"


#define X 500
#define Y 500
#define SNAKE_NUM X*Y//�ߵ�������

enum DIR
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

struct Snake 
{
	int size;//�ж��ٽ�
	int dir=RIGHT;//�ߵķ���
	int speed;//�ߵ��ٶ�
	int r;//�߽ڰ뾶
	POINT coor[SNAKE_NUM];//ÿ�ڵ�����
}snake;


struct Food
{
	int x;
	int y;
	int r;//ʳ���С
	bool flag;//�ж�ʳ�ﱻ����û��
	DWORD color;//ʳ����ɫ
}food;

//��ʼ������
void GameInit()
{
	//init��ʼ��graphͼ�δ���
	initgraph(X, Y);

	//�������������
	srand(GetTickCount());

	//��ʼ����
	snake.size = 3;
	snake.speed = 10;
	snake.dir;
	snake.r = 5;
	for (int i = 0; i < snake.size ; i++)
	{
		snake.coor[i].x = 40-10*i;
		snake.coor[i].y = 10;
	}

	//��ʼ��ʳ��
	food.x = rand() % X;		
	food.y = rand() % Y;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.r = rand() % 10 + 5;
	food.flag = true;
}


//����ͼ��(�ֿ�һ���ֺò���)
void GameDraw()
{
	//˫�����ͼ
	BeginBatchDraw();

	//���ô�����ɫ
	setbkcolor(RGB(19, 128, 113));

	//ˢ�²�����ɫ
	cleardevice();

	//������
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, snake.r);
	}
	
	EndBatchDraw();
}
//����ʳ��
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
	for (int i = snake.size-1; i >0; i--)//��β����ʼ��ͷ����ֵ���������仯���ص�
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


//ʳ�ﱻ���ж�
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
		//���³�ʼ��ʳ��
		food.x = rand() % X;	
		food.y = rand() % Y;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
		food.flag = true;
	}
}


//ͨ�����������ߵķ���
void keyControl()
{
	//72 80 75 77�������ҵļ�ֵ
	
	//���_getch()�Ƕ���һ���ַ�ʱ��������һ�µ�
	 
	//����Ҫ���ж���û�а���

	if (_kbhit())//�а����ͷ�����
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
		
		//��ͣ����
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
	fillcircle(100, 100, 50);//Բ����ɫ���
	line(0, 0, 300, 300);//��һ����
	fillrectangle(350, 300, 600, 650);//������ɫ���
	solidrectangle(30, 300, 60, 650);//ʵ�ľ���
	while (1);
	closegraph();
	return 0;
	*/