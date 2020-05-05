#include "Game/Ground.h"

#include "Renderer/Renderer.h"

Ground::Ground(float x)
    : m_position(x, 0), m_size(160 * 10, 33 * 10)
{
    SpriteManager::LoadTexture("groundSheet", "assets/textures/Ground.png");
    m_sprite = SpriteManager::LoadSprite("ground", "groundSheet", { 0, 0 }, { 160, 33 });
}

Ground::~Ground()
{
}

void Ground::Update(float dt)
{
}

void Ground::Render() const
{
    Renderer::DrawQuad(m_position, m_size, m_sprite.uv1, m_sprite.uv2, m_sprite.sheet);
}
