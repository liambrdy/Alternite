#include "Game/Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

#include "Game/SpriteManager.h"

#include <cstring>

Game::Game()
{
    SpriteManager::Init();
    SpriteManager::LoadTexture("playerSheet", "assets/textures/Player.png");
    SpriteManager::LoadTexture("groundSheet", "assets/textures/Ground.png");
    
    m_font = std::make_shared<Font>("assets/fonts/hack.ttf", FONT_SIGNED_DISTANCE);

    ECS::Init();
    RegisterComponents();

    m_renderSystem = ECS::RegisterSystem<RenderSystem>();

    Signature signature;
    signature.set(ECS::GetComponentType<PositionComponent>());
    signature.set(ECS::GetComponentType<SizeComponent>());
    signature.set(ECS::GetComponentType<SpriteComponent>());
    ECS::SetSystemSignature<RenderSystem>(signature);

    m_inputSystem = ECS::RegisterSystem<InputSystem>();

    signature.reset();
    signature.set(ECS::GetComponentType<PositionComponent>());
    signature.set(ECS::GetComponentType<InputComponent>());
    ECS::SetSystemSignature<InputSystem>(signature);
    
    m_player = ECS::CreateEntity();
    ECS::AddComponent<PositionComponent>(m_player, { .position = { 300, 33 * 10 - 20 } });
    ECS::AddComponent<SizeComponent>(m_player, { .size = { 19 * 4, 32 * 4 } });
    ECS::AddComponent<SpriteComponent>(m_player, { .sprite = SpriteManager::LoadSprite("player", "playerSheet", { 0, 0 }, { 19, 32 }) });
    ECS::AddComponent<InputComponent>(m_player, { .speed = 300 });

    for (int x = -5; x <= 5; x++)
    {
        Entity entity = ECS::CreateEntity();
        ECS::AddComponent<PositionComponent>(entity, { .position = { x * (160 * 10), 0 } });
        ECS::AddComponent<SizeComponent>(entity, { .size = { 160 * 10, 33 * 10 } });
        ECS::AddComponent<SpriteComponent>(entity, { .sprite = SpriteManager::LoadSprite("ground", "groundSheet", { 0, 0 }, { 160, 33 }) });
        m_grounds.push_back(entity);
    }
}

Game::~Game()
{
    for (auto& entity : m_grounds)
    {
        ECS::DestroyEntity(entity);
    }

    ECS::DestroyEntity(m_player);
    ECS::Shutdownn();

    SpriteManager::Shutdown();
}

Scene* Game::OnUpdate(double delta)
{
    m_delta = delta;

    if (Input::WindowShouldClose())
    {
        delete this;
        return nullptr;
    }

    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        WindowMode newMode = Window::Get()->GetWindowMode() == WindowMode::Windowed ? WindowMode::Fullscreen : WindowMode::Windowed;
        Window::Get()->SetWindowMode(newMode);
    }
    
    m_inputSystem->Update(delta);

    return this;
}

void Game::OnRender() const
{
    auto playerPos = ECS::GetComponent<PositionComponent>(m_player);
    Renderer::SetRenderOrigin({ playerPos.position.x, playerPos.position.y + 250 });

    m_renderSystem->Update();

    Renderer::DrawQuad({ 0, 0 }, { 200, 100 }, { 0.8f, 0.2f, 0.2f, 0.7f });

    char str[100];
    snprintf(str, 100, "Frame Time: %f", m_delta);
    Renderer::DrawText({ 0.5f, Window::Get()->GetHeight() - 20.0f }, str, m_font, 0.3f);
}