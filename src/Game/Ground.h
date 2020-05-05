#pragma once

#include "Game/Entity.h"
#include "Game/SpriteManager.h"

#include "Core/Common.h"

#include "Renderer/Texture.h"

class Ground : public Entity
{
public:
    Ground(float x);
    virtual ~Ground();

    virtual void Update(float dt) override;
    virtual void Render() const override;

    virtual const glm::vec2& GetPosition() const override { return m_position; }
    virtual const glm::vec2& GetSize() const override { return m_size; }
private:
    glm::vec2 m_position;
    glm::vec2 m_size;

    Sprite m_sprite;
};