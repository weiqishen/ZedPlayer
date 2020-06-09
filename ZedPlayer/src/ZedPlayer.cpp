
//C++ standard libraies
#include <iostream>
//project includes
#include "global.h"
#include "Player.h"

//handle arguments and start the player
int main(int argc, char*argv[])
{
	pl::Player player;
	player.Init();
	player.Run();
	return 0;
}