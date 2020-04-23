#include "Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

Game::Game()
{
}

Game::~Game()
{
}

Scene* Game::OnUpdate(double delta)
{
    if (Input::IsKeyDown(GLFW_KEY_Q))
        LOG_INFO("Q Down");
    if (Input::IsKeyPressed(GLFW_KEY_W))
        LOG_INFO("W Pressed");

    if (Input::WindowShouldClose())
    {
        delete this;
        return nullptr;
    }

    return this;
}

void Game::OnRender() const
{

}