#include <iostream>

#include "Core/Scene.h"
#include "Core/Game.h"
#include "Core/Logger.h"
#include "Core/Common.h"

#include "Renderer/Window.h"
#include "Renderer/Renderer.h"

constexpr int width = 800;
constexpr int height = 600;

int main()
{
    Logger::Init();

    Scene* scene = new Game();
    Window window(width, height);

    Renderer::Init(width, height);

    double time = glfwGetTime();

    for (;;)
    {

        double currentTime = glfwGetTime();
        scene = scene->OnUpdate(currentTime - time);
        if (!scene)
            return 0;
        time = currentTime;

        Renderer::BeginFrame();
        scene->OnRender();
        Renderer::EndFrame();
        
        window.OnUpdate();
    }
}