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

    double time = glfwGetTime();

    for (;;)
    {
        window.OnUpdate();
        scene->OnRender();

        Event e;
        while (window.HasEventReady(e))
        {
            scene = scene->OnEvent(e);
            if (!scene)
                return 0;
        }

        double currentTime = glfwGetTime();
        scene = scene->OnUpdate(currentTime - time);
        if (!scene)
            return 0;
        time = currentTime;
    }
}