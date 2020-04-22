#include <iostream>

#include "Core/Scene.h"
#include "Core/Game.h"
#include "Core/Logger.h"
#include "Core/Common.h"

int main()
{
    Logger::Init();

    Scene* scene = new Game();
}