#pragma once

#include "ECS/ECS.h"

#include "Game/Components.h"

#define NewSystem(name, args)   \
class name : public System      \
{                               \
public:                         \
    void Update(args);          \
};

NewSystem(RenderSystem, void)
NewSystem(InputSystem, float)

static void SetSystemSignatures()
{
    Signature signature;
    signature.set(ECS::GetComponentType<PositionComponent>());
    signature.set(ECS::GetComponentType<SizeComponent>());
    signature.set(ECS::GetComponentType<SpriteComponent>());
    ECS::SetSystemSignature<RenderSystem>(signature);

    signature.reset();
    signature.set(ECS::GetComponentType<PositionComponent>());
    signature.set(ECS::GetComponentType<InputComponent>());
    ECS::SetSystemSignature<InputSystem>(signature);
}