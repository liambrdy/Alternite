#include "Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"

Game::Game()
{
}

Game::~Game()
{
}

Scene* Game::OnUpdate(double delta)
{
    if (Input::WindowShouldClose())
    {
        delete this;
        return nullptr;
    }

    return this;
}

void Game::OnRender() const
{
    for (int y = 0; y < 600; y += 20)
        for (int x = 0; x < 800; x += 20)
        {
            glm::vec4 color = { (float)x / 1600.0f, 0.3f, (float)y / 1200.0f, 1.0f };
            Renderer::DrawQuad({ x, y }, { 19.0f, 19.0f }, color);
        }
}