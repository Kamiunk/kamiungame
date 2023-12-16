#pragma once

#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include <vector>
#include <random>
#include <ranges>
#include <windows.h>
#include <thread>
#include <chrono>
#include <iostream>

namespace srv = std::ranges::views;

constexpr int X_CELL_SIZE = 20;
constexpr int Y_CELL_SIZE = 20;
constexpr int X_CELL = 20;
constexpr int Y_CELL = 20;
constexpr int X_SIZE = X_CELL_SIZE * X_CELL;
constexpr int Y_SIZE = Y_CELL_SIZE * Y_CELL;

class Game
{
public:

	int frame_gap_in_ms_ = 500;
	int time_to_next_frame_in_ms_ = 0;
	int food_gap_frame_ = 20;

	enum DIR
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};

	enum GameState 
	{
		NORMAL,
		GAME_OVER
	};

	struct Snake
	{
		DIR dir = RIGHT; // 蛇的方向
		std::vector<POINT> poses; // 每节的坐标
	};


	struct Food
	{
		POINT pos;
		DWORD color = RED;
	};

	struct GameData 
	{
		std::mt19937 random_generator;
		Snake snake;
		std::vector<Food> foods;
		int frame_count = 0;
		GameState state = NORMAL;
	};

	static DIR VKCodeToDIR(const int& vk_code) {
		switch (vk_code)
		{
		case VK_UP:
			return UP;
		case VK_DOWN:
			return DOWN;
		case VK_RIGHT:
			return RIGHT;
		case VK_LEFT:
			return LEFT;
		default:
			throw std::runtime_error("invalid vk_code");
		}
	}

	static bool IsSamePoint(const POINT& a, const POINT& b) {
		return a.x == b.x && a.y == b.y;
	}

	static bool IsKeyPressed(int vk_code)
	{
		return GetAsyncKeyState(vk_code) & 0x8000;
	}

	static bool IsSnakeIn(const Snake& snake,const POINT &point)
	{
		return std::ranges::any_of(snake.poses | srv::all, [&](POINT pos) {return IsSamePoint(pos, point); });
	}

	static bool IsSnakeDead(const Snake& snake)
	{
		for (auto i = 0; i < snake.poses.size(); i++)
		{
			for (auto j = i + 1; j < snake.poses.size(); j++)
			{
				if (IsSamePoint(snake.poses[i], snake.poses[j]))
					return true;
			}
		}
		return false;
	}

	static bool IsFoodIn(const std::vector<Food>& foods, const POINT& point)
	{
		return std::ranges::any_of(foods | srv::all | srv::transform([](Food f) {return f.pos; }), [&](POINT pos) {return pos.x == point.x && pos.y == point.y; });
	}

	static bool IsMapFull(const GameData& game_data)
	{
		std::vector<POINT> all_poses;
		for (auto i = 0; i < X_SIZE; i++)
		{
			for (auto j = 0; j < Y_SIZE; j++)
			{
				if (!IsSnakeIn(game_data.snake, POINT(i, j)) && !IsFoodIn(game_data.foods, POINT(i, j)))
					return false;
			}
		}
		return true;
	}

	GameData NextFrame(const GameData& game_data, const std::vector<DIR>& input_dirs) const
	{
		GameData next_game_data = game_data;
		next_game_data.frame_count++;
		if(game_data.snake.poses.empty()) throw std::runtime_error("snake is empty");
		std::ranges::for_each(game_data.foods,
			[&](auto& food)
		{
			if (IsSnakeIn(game_data.snake,food.pos))
			{
				next_game_data.snake.poses.push_back(POINT(game_data.snake.poses.back()));
				std::erase_if(next_game_data.foods, [&](auto& f) {return IsSamePoint(f.pos, food.pos); });
			}
		});
		switch (game_data.snake.dir)
		{
			case UP:
				next_game_data.snake.poses[0].y--;
				break;
			case DOWN:
				next_game_data.snake.poses[0].y++;
				break;
			case RIGHT:
				next_game_data.snake.poses[0].x++;
				break;
			case LEFT:
				next_game_data.snake.poses[0].x--;
				break;
			default:
				throw std::runtime_error("invalid direction");
				break;
		}
		std::ranges::for_each(
			next_game_data.snake.poses
			| srv::all
			| std::views::enumerate,
			[&](auto e)
		{
			auto& [index, pos] = e;
			if (index != 0)
				next_game_data.snake.poses[index] = game_data.snake.poses[index - 1];
			auto& x = next_game_data.snake.poses[index].x;
			auto& y = next_game_data.snake.poses[index].y;
			if (x < 0) x += X_CELL;
			if (x >= X_CELL) x -= X_CELL;
			if (y < 0) y += Y_CELL;
			if (y >= Y_CELL) y -= Y_CELL;
		});

		if (!input_dirs.empty())
		{
			auto dir = input_dirs.back();
			switch (next_game_data.snake.dir)
			{
			case UP:
				if (dir != DOWN)
					next_game_data.snake.dir = dir;
				break;
			case DOWN:
				if (dir != UP)
					next_game_data.snake.dir = dir;
				break;
			case RIGHT:
				if (dir != LEFT)
					next_game_data.snake.dir = dir;
				break;
			case LEFT:
				if (dir != RIGHT)
					next_game_data.snake.dir = dir;
				break;
			default:
				throw std::runtime_error("invalid direction");
				break;
			}
		}

		if (next_game_data.frame_count % food_gap_frame_ == 0&& !IsMapFull(game_data))
		{
			Food food;
			do
			{
				food.pos.x = next_game_data.random_generator() % X_CELL;
				food.pos.y = next_game_data.random_generator() % Y_CELL;
			} while (IsSnakeIn(game_data.snake, food.pos) || IsFoodIn(game_data.foods, food.pos));
			next_game_data.foods.push_back(food);
			std::cout << "new food at: (" << food.pos.x << "," << food.pos.y << ")" << std::endl;
		}

		if (std::ranges::size(game_data.snake.poses
			| srv::filter([&](auto& pos)
		{
			return IsSnakeDead(game_data.snake);
		})
			| std::ranges::to<std::vector<POINT>>()) > 0)
			next_game_data.state = GAME_OVER;

		return next_game_data;
	}

	void Draw(const GameData& game_data)
	{
		cleardevice();
		setfillcolor(RGB(255, 255, 255));
		for (const auto& food : game_data.foods)
		{
			setfillcolor(food.color);
			solidcircle(food.pos.x * X_CELL_SIZE + X_CELL_SIZE / 2, food.pos.y * Y_CELL_SIZE + Y_CELL_SIZE / 2, X_CELL_SIZE / 2);
		}
		setfillcolor(RGB(255, 255, 255));
		for (const auto& pos : game_data.snake.poses)
		{
			solidrectangle(pos.x * X_CELL_SIZE, pos.y * Y_CELL_SIZE, (pos.x + 1) * X_CELL_SIZE, (pos.y + 1) * Y_CELL_SIZE);
		}
		FlushBatchDraw();
	}

	int Run() {
		GameData game_data;
		game_data.random_generator=std::mt19937(std::random_device()());

		Snake snake;
		snake.poses = std::vector<POINT>{ {0,0},{1,0},{2,0} };
		snake.dir = DOWN;
		game_data.snake = snake;

		initgraph(X_SIZE, Y_SIZE);

		std::vector<DIR> dirs;
		while (!GetAsyncKeyState('Q') && !game_data.state == GAME_OVER)
		{
			Draw(game_data);

			std::ranges::copy(std::array<int, 4>{VK_UP, VK_DOWN, VK_RIGHT, VK_LEFT}
			| srv::all
				| srv::filter(IsKeyPressed)
				| srv::transform(VKCodeToDIR),
				std::back_inserter(dirs));

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			time_to_next_frame_in_ms_-= 10;
			if (time_to_next_frame_in_ms_ <= 0) 
			{
				time_to_next_frame_in_ms_ += frame_gap_in_ms_;
				game_data = NextFrame(game_data, dirs);
				std::cout << "frame_count: " << game_data.frame_count << std::endl;
			}
		}

		std::cout << "GAME_OVER" << std::endl << "Snake Length: " << game_data.snake.poses.size() << std::endl;
		std::cout << "Press any key to quit..." << std::endl;

		_getch();

		closegraph();

		return 0;
	}
};
