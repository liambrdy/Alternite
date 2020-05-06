#pragma once

#include <glm/glm.hpp>

#include "ECS/ECS.h"

#include "Game/SpriteManager.h"

struct PositionComponent
{
    glm::vec2 position;
};

struct SizeComponent
{
    glm::vec2 size;
};

struct InputComponent
{
    float speed;
};

struct SpriteComponent
{
    Sprite sprite;
};

static void RegisterComponents()
{
    ECS::RegisterComponent<PositionComponent>();
    ECS::RegisterComponent<SizeComponent>();
    ECS::RegisterComponent<SpriteComponent>();
    ECS::RegisterComponent<InputComponent>();
}