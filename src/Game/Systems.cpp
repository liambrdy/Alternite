#include "Game/Systems.h"

#include "Game/Components.h"
#include "Renderer/Renderer.h"

#include "Core/Input.h"

void RenderSystem::Update(void)
{
    for (auto const& entity : m_entites)
    {
        auto& position = ECS::GetComponent<PositionComponent>(entity);
        auto& size = ECS::GetComponent<SizeComponent>(entity);
        auto& sprite = ECS::GetComponent<SpriteComponent>(entity);

        Renderer::DrawQuad(position.position, size.size, sprite.sprite.uv1, sprite.sprite.uv2, sprite.sprite.sheet);
    }
}

void InputSystem::Update(float delta)
{
    for (auto const& entity : m_entites)
    {
        auto& position = ECS::GetComponent<PositionComponent>(entity);
        auto& speed = ECS::GetComponent<InputComponent>(entity);

        if (Input::IsKeyDown(GLFW_KEY_A))
        {
            position.position.x -= speed.speed * delta;
        }
        else if (Input::IsKeyDown(GLFW_KEY_D))
        {
            position.position.x += speed.speed * delta;
        }

        if (Input::IsKeyDown(GLFW_KEY_W))
        {
            position.position.y += speed.speed * delta;
        }
        else if (Input::IsKeyDown(GLFW_KEY_S))
        {
            position.position.y -= speed.speed * delta;
        }
    }
}