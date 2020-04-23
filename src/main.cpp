#include <iostream>

#include "Core/Scene.h"
#include "Core/Game.h"
#include "Core/Logger.h"
#include "Core/Common.h"

#include "Renderer/Window.h"
#include "Renderer/Renderer.h"

int main()
{
    Logger::Init();

    Scene* scene = new Game();
    Window window(800, 600);

    Renderer::Init();
    
    double time = glfwGetTime();

    for (;;)
    {
        scene->OnRender();

        double currentTime = glfwGetTime();
        scene = scene->OnUpdate(currentTime - time);
        if (!scene)
            return 0;
        time = currentTime;

        window.OnUpdate();
    }
}