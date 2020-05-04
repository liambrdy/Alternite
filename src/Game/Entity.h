#pragma once

#include <glm/glm.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() const = 0;

    virtual const glm::vec2& GetPosition() const = 0;
    virtual const glm::vec2& GetSize() const = 0;
};