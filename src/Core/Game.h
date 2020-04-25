#pragma once

#include "Core/Scene.h"
#include "Renderer/Texture.h"

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual void OnRender() const override;
private:
    Texture* m_texture;
};