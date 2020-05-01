#include "Core/Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

#include <cstring>

Game::Game()
{
    m_texture = std::make_shared<Texture>("assets/textures/Grid.png");
    m_font = std::make_shared<Font>("assets/fonts/hack.ttf", FONT_SIGNED_DISTANCE);

    m_ground = std::make_shared<Texture>("assets/textures/Ground.png");
    m_player = std::make_shared<Texture>("assets/textures/Character.png");
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

    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        WindowMode newMode = Window::Get()->GetWindowMode() == WindowMode::Windowed ? WindowMode::Fullscreen : WindowMode::Windowed;
        Window::Get()->SetWindowMode(newMode);
    }
    
    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        m_pos.x -= 200.0f * delta;
    }
    else if (Input::IsKeyDown(GLFW_KEY_D))
    {
        m_pos.x += 200.0f * delta;
    }

    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        m_pos.y += 200.0f * delta;
    }
    else if (Input::IsKeyDown(GLFW_KEY_S))
    {
        m_pos.y -= 200.0f * delta;
    }

    m_delta = delta;

    return this;
}

void Game::OnRender() const
{
    Renderer::SetRenderOrigin({ m_pos.x, m_pos.y + 250 });

    for (int x = -5; x <= 5; x++)
    {
        Renderer::DrawQuad({ x * (160 * 10), 0 }, { 160 * 10, 33 * 10 }, m_ground);
    }
    
    Renderer::DrawQuad(m_pos, { 19 * 4, 32 * 4 }, m_player);

    char str[100];
    snprintf(str, 100, "Frame Time: %f", m_delta);
    Renderer::DrawText({ 0.5f, Window::Get()->GetHeight() - 20.0f }, str, m_font, 0.3f);
}