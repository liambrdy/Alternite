#include "Game/Ground.h"

#include "Renderer/Renderer.h"

Ground::Ground(float x)
    : m_position(x, 0), m_size(160 * 10, 33 * 10)
{
    m_texture = std::make_shared<Texture>("assets/textures/Ground.png");
}

Ground::~Ground()
{
}

void Ground::Update(float dt)
{
}

void Ground::Render() const
{
    Renderer::DrawQuad(m_position, m_size, m_texture);
}
