#include "Game/Player.h"

#include "Core/Input.h"

#include "Renderer/Renderer.h"

Player::Player()
    : m_position(300, 33 * 10 - 20), m_size(19 * 4, 32 * 4)
{
    m_texture = std::make_shared<Texture>("assets/textures/Player.png");
}

Player::~Player()
{
}

void Player::Update(float dt)
{
    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        m_position.x -= 300.0f * dt;
    }
    else if (Input::IsKeyDown(GLFW_KEY_D))
    {
        m_position.x += 300.0f * dt;
    }

    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        m_position.y += 300.0f * dt;
    }
    else if (Input::IsKeyDown(GLFW_KEY_S))
    {
        m_position.y -= 300.0f * dt;
    }
}

void Player::Render() const
{
    Renderer::DrawQuad(m_position, m_size, m_texture);   
}