#pragma once

#include "ECS/SystemManager.h"

#include "Core/Input.h"

class InputSystem : public System
{
public:
    InputSystem() {}

    void Update(float delta)
    {
        for (const auto& entity : m_entites)
        {
            auto& transform = ECS::GetComponent<TransformComponent>(entity);
            auto& movement = ECS::GetComponent<MovementComponent>(entity);
            auto& collider = ECS::GetComponent<ColliderComponent>(entity);

            if (Input::IsKeyDown(GLFW_KEY_A))
            {
                collider.body->ApplyForceToCenter({ -movement.speed, 0.0f }, true);
            }
            else if (Input::IsKeyDown(GLFW_KEY_D))
            {
                collider.body->ApplyForceToCenter({ movement.speed, 0.0f }, true);
            }

            if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            {
                for (b2ContactEdge* ce = collider.body->GetContactList(); ce != nullptr; ce = ce->next)
                {
                    b2Contact* c = ce->contact;
                    if (c->IsTouching())
                    {
                        b2WorldManifold manifold;
                        c->GetWorldManifold(&manifold);

                        bool below = false;
                        for (int i = 0; i < b2_maxManifoldPoints; ++i)
                        {
                            if (manifold.points[i].y < collider.body->GetPosition().y - transform.size.y / 2.0f + 0.01f)
                                below = true;
                        }

                        if (below)
                        {
                            collider.body->ApplyLinearImpulse({ 0.0f, 0.1f }, { 0.0f, 0.0f }, true);
                        }
                    }
                }
            }
        }
    }
};
