#pragma once

#include "Core/Events.h"

class Scene
{
public:
    virtual ~Scene() = default;

    virtual Scene* OnUpdate(double delta) = 0;
    virtual Scene* OnEvent(const Event& event) = 0;
    virtual void OnRender() const = 0;
};