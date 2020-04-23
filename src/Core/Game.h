#pragma once

#include "Core/Scene.h"

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual void OnRender() const override;
};