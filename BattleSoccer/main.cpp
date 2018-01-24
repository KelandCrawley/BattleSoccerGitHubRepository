#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <LuaBridge.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Game.h"

int main()
{
    int isGameOver = 1;
    std::cout << "Starting" << std::endl;
    Game game;
    game.Initialize();

    while (isGameOver != 0)
    {
        isGameOver = game.Run();
        //isGameOver = game.Run();
    }
    std::cout << "Waiting" << std::endl;
    return 0;
}