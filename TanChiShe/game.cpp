#include "game.h"

int main()
{
	Game game;
	game.frame_gap_in_ms_ = 100;
	return game.Run();
}