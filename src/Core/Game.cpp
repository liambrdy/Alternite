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
    m_anotherFont = std::make_shared<Font>("assets/fonts/EightBitDragon.ttf", FONT_NORMAL);

    m_titleWidth = m_font->GetTextWidth("ALTERNITE", 1.5f);
    m_belowWidth = m_anotherFont->GetTextWidth("VERY COOL GAME", 1.0f);
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

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        WindowMode newMode = Window::Get()->GetWindowMode() == WindowMode::Windowed ? WindowMode::Fullscreen : WindowMode::Windowed;
        Window::Get()->SetWindowMode(newMode);
    }

    m_delta = delta;

    return this;
}

void Game::OnRender() const
{
    for (int y = 0; y < Window::Get()->GetHeight(); y += 20)
        for (int x = 0; x < Window::Get()->GetWidth(); x += 20)
        {
            glm::vec4 color = { (float)x / (Window::Get()->GetWidth() * 2), 0.3f, (float)y / (Window::Get()->GetHeight() * 2), 1.0f };
            Renderer::DrawQuad({ x, y }, { 19.0f, 19.0f }, color);
        }

    Renderer::DrawText({ (Window::Get()->GetWidth() / 2) - (m_titleWidth / 2), Window::Get()->GetHeight() / 2 }, "ALTERNITE", m_font, 1.5f, { 0.0f, 0.0f, 0.0f, 1.0f });
    Renderer::DrawText({ (Window::Get()->GetWidth() / 2) - (m_belowWidth / 2), 200.0f }, "VERY COOL GAME", m_anotherFont, 1.0f);

    char str[100];
    snprintf(str, 100, "Frame Time: %f", m_delta);

    Renderer::DrawText({ 0.5f, Window::Get()->GetHeight() - 20.0f }, str, m_font, 0.3f);
}