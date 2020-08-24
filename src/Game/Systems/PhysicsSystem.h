#pragma once

#include "ECS/SystemManager.h"

#include <box2d/box2d.h>

class PhysicsSystem : public System
{
public:
    PhysicsSystem()
        : m_world({ 0.0f, -10.0f })
    {
    }

    void InitBodies()
    {
        for (auto& entity : m_entites)
        {
            auto& transform = ECS::GetComponent<TransformComponent>(entity);
            auto& collider = ECS::GetComponent<ColliderComponent>(entity);

            b2BodyDef bodyDef;
            bodyDef.type = collider.type == ColliderType::Static ? b2_staticBody : b2_dynamicBody;
            bodyDef.position.Set(transform.position.x + (transform.size.x / 2.0f), transform.position.y + (transform.size.y / 2.0f));
            collider.body = m_world.CreateBody(&bodyDef);

            b2PolygonShape shape;
            shape.SetAsBox(transform.size.x / 2, transform.size.y / 2);

            if (collider.type == ColliderType::Static)
            {
                collider.fixture = collider.body->CreateFixture(&shape, 0.0f);
            }
            else if (collider.type == ColliderType::Dynamic)
            {
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &shape;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.3f;
                collider.fixture = collider.body->CreateFixture(&fixtureDef);
            }
        }

        b2BodyDef groundDef;
        groundDef.type = b2_staticBody;
        groundDef.position.Set(0.0f, 0.5f);
        b2Body* ground = m_world.CreateBody(&groundDef);

        b2PolygonShape groundShape;
        groundShape.SetAsBox(10.0f, 0.5f);
        ground->CreateFixture(&groundShape, 0.0f);
    }

    void Update(float delta)
    {
        m_world.Step(delta, 6, 6);

        for (const auto& entity : m_entites)
        {
            auto& transform = ECS::GetComponent<TransformComponent>(entity);
            auto& collider = ECS::GetComponent<ColliderComponent>(entity);

            auto newPos = collider.body->GetPosition();
            transform.position = { newPos.x - (transform.size.x / 2.0f), newPos.y - (transform.size.y / 2.0f) };
        }
    }
private:
    b2World m_world;
};
