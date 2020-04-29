#pragma once

#include "Core/Scene.h"
#include "Core/Common.h"

#include "Renderer/Texture.h"
#include "Renderer/Text/Font.h"

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual void OnRender() const override;
private:
    Ref<Texture> m_texture;
    Ref<Font> m_font, m_anotherFont;

    float m_delta;
    float m_titleWidth, m_belowWidth;
};