#pragma once

#include "Scene.h"

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* Update(double delta) override;
    virtual Scene* HandleEvent() override;
    virtual void Render() const override;
};