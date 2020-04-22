#include "Game.h"

#include "Core/Common.h"

Game::Game()
{
    
}

Game::~Game()
{
    
}

Scene* Game::OnUpdate(double delta)
{
    return this;
}

Scene* Game::OnEvent(const Event& event)
{
    switch (event.type)
    {
        case WindowClosed:
        {
            delete this;
            return nullptr;
        }
        case KeyPressed:
        {
            LOG_INFO((char)event.keyboard.key);
        }
    }

    return this;
}

void Game::OnRender() const
{

}
