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
    Renderer::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.7f, 0.2f, 0.2f, 1.0f });
}