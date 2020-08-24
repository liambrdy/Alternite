#pragma once

#include <glm/glm.hpp>

#include <box2d/box2d.h>

#include "ECS/ECS.h"

#include "Game/SpriteManager.h"

struct TransformComponent
{
    glm::vec2 position;
    glm::vec2 size;
};

struct MovementComponent
{
    float speed;
    bool jump;
};

enum class ColliderType
{
    Static,
    Dynamic
};

struct ColliderComponent
{
    ColliderType type;
    b2Body* body;
    b2Fixture* fixture;
};

struct SpriteComponent
{
    Sprite sprite;
};

static void RegisterComponents()
{
    ECS::RegisterComponent<TransformComponent>();
    ECS::RegisterComponent<MovementComponent>();
    ECS::RegisterComponent<ColliderComponent>();
    ECS::RegisterComponent<SpriteComponent>();
}
