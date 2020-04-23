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