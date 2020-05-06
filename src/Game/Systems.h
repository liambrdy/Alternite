#pragma once

#include "ECS/ECS.h"

#define NewSystem(name, args)   \
class name : public System      \
{                               \
public:                         \
    void Update(args);          \
};

NewSystem(RenderSystem, void)
NewSystem(InputSystem, float)