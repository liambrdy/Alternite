#pragma once

#include "ECS/ECS.h"

#include "Game/Components.h"

#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/InputSystem.h"
#include "Game/Systems/PhysicsSystem.h"

static void SetSystemSignatures()
{
    Signature signature;
    signature.set(ECS::GetComponentType<TransformComponent>());
    signature.set(ECS::GetComponentType<SpriteComponent>());
    ECS::SetSystemSignature<RenderSystem>(signature);

    signature.reset();
    signature.set(ECS::GetComponentType<MovementComponent>());
    signature.set(ECS::GetComponentType<TransformComponent>());
    signature.set(ECS::GetComponentType<ColliderComponent>());
    ECS::SetSystemSignature<InputSystem>(signature);

    signature.reset();
    signature.set(ECS::GetComponentType<TransformComponent>());
    signature.set(ECS::GetComponentType<ColliderComponent>());
    ECS::SetSystemSignature<PhysicsSystem>(signature);
}
