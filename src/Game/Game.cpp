#include "Game/Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

#include "Game/Player.h"
#include "Game/Ground.h"

#include <cstring>

Game::Game()
{
    m_texture = std::make_shared<Texture>("assets/textures/Grid.png");
    m_font = std::make_shared<Font>("assets/fonts/hack.ttf", FONT_SIGNED_DISTANCE);

    m_ground = std::make_shared<Texture>("assets/textures/Ground.png");

    m_player = new Player();
    m_entities.push_back(m_player);

    for (int x = -5; x <= 5; x++)
    {
        Entity* ground = new Ground(x * (160 * 10));
        m_entities.push_back(ground);
    }
}

Game::~Game()
{
    for (auto& entity : m_entities)
        delete entity;
}

Scene* Game::OnUpdate(double delta)
{
    m_delta = delta;

    if (Input::WindowShouldClose())
    {
        delete this;
        return nullptr;
    }

    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        WindowMode newMode = Window::Get()->GetWindowMode() == WindowMode::Windowed ? WindowMode::Fullscreen : WindowMode::Windowed;
        Window::Get()->SetWindowMode(newMode);
    }
    
    for (auto& entity : m_entities)
        entity->Update(delta);

    return this;
}

void Game::OnRender() const
{
    Renderer::SetRenderOrigin({ m_player->GetPosition().x, m_player->GetPosition().y + 250 });
    
    for (auto& entity : m_entities)
        entity->Render();

    char str[100];
    snprintf(str, 100, "Frame Time: %f", m_delta);
    Renderer::DrawText({ 0.5f, Window::Get()->GetHeight() - 20.0f }, str, m_font, 0.3f);
}