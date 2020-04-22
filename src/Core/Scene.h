#pragma once

class Scene
{
public:
    virtual ~Scene() = default;

    virtual Scene* Update(double delta) = 0;
    virtual Scene* HandleEvent() = 0;
    virtual void Render() const = 0;
};