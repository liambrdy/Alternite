#pragma once

#include "ECS/SystemManager.h"

#include "Renderer/Renderer.h"

class RenderSystem : public System
{
public:
    RenderSystem() {}

    void Update()
    {
        for (auto const& entity : m_entites)
        {
            auto& transform = ECS::GetComponent<TransformComponent>(entity);
            auto& sprite = ECS::GetComponent<SpriteComponent>(entity);
            auto& collider = ECS::GetComponent<ColliderComponent>(entity);

            auto newPos = collider.body->GetPosition();

            Renderer::DrawQuad(transform.position, transform.size, sprite.sprite.uv1, sprite.sprite.uv2, sprite.sprite.sheet);
            Renderer::DrawQuad({ newPos.x - 0.25f, newPos.y - 0.25f }, { 0.5f, 0.5f }, { 0.8f, 0.2f, 0.2f, 0.5f });
        }
    }
};
