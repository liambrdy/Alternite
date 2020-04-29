#include "Core/Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"

#include <cstring>

Game::Game()
{
    m_texture = std::make_shared<Texture>("assets/textures/Grid.png");
    m_font = std::make_shared<Font>("assets/fonts/hack.ttf");

    m_titleWidth = m_font->GetTextWidth("ALTERNITE", 1.5f);
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

    m_delta = delta;

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

    Renderer::DrawText({ 400.0f - (m_titleWidth / 2), 300.0f }, "ALTERNITE", m_font, 1.5f, { 0.0f, 0.0f, 0.0f, 1.0f });

    char str[100];
    snprintf(str, 100, "Frame Time: %f", m_delta);

    Renderer::DrawText({ 0.5f, 580.0f }, str, m_font, 0.3f);
}