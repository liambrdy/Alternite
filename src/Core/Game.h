#pragma once

#include "Scene.h"

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual Scene* OnEvent(const Event& event) override;
    virtual void OnRender() const override;
};