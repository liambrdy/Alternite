#pragma once

class Scene
{
public:
    virtual ~Scene() = default;

    virtual Scene* OnUpdate(double delta) = 0;
    virtual void OnRender() const = 0;
};