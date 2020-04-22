#include <iostream>

#include "Core/Scene.h"
#include "Core/Game.h"
#include "Core/Logger.h"
#include "Core/Common.h"

#include "Renderer/Window.h"

int main()
{
    Logger::Init();

    Scene* scene = new Game();
    Window window(800, 600);

    for (;;)
    {
        window.OnUpdate();
    }
}