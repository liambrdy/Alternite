#pragma once

#include "Core/Scene.h"
#include "Core/Common.h"

#include "Renderer/Texture.h"

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual void OnRender() const override;
private:
    Ref<Texture> m_texture;
};