#include <iostream>

#include "Core/Scene.h"
#include "Core/Logger.h"
#include "Core/Common.h"

#include "Game/Game.h"

#include "Renderer/Window.h"
#include "Renderer/Renderer.h"

constexpr int width = 800;
constexpr int height = 600;

int main()
{
    Logger::Init();

    Window window(width, height);
    Renderer::Init(width, height);

    Scene* scene = new Game();

    double time = glfwGetTime();

    for (;;)
    {
        double currentTime = glfwGetTime();
        scene = scene->OnUpdate(currentTime - time);
        if (!scene)
            break;
        time = currentTime;

        Renderer::BeginFrame();
        scene->OnRender();
        Renderer::EndFrame();
        
        window.OnUpdate();
    }

    Renderer::Shutdown();
}