#include "Game/Game.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

#include "Game/SpriteManager.h"

#include <cstring>

// One Meter is 32 pixels

Game::Game()
{
    SpriteManager::Init();
    SpriteManager::LoadTexture("playerSheet", "assets/textures/Player.png");
    SpriteManager::LoadTexture("groundSheet", "assets/textures/Ground.png");

    m_font = std::make_shared<Font>("assets/fonts/hack.ttf", FONT_SIGNED_DISTANCE);

    ECS::Init();
    RegisterComponents();

    m_renderSystem = ECS::RegisterSystem<RenderSystem>();
    m_inputSystem = ECS::RegisterSystem<InputSystem>();
    m_physicsSystem = ECS::RegisterSystem<PhysicsSystem>();
    SetSystemSignatures();

    m_player = ECS::CreateEntity();
    ECS::AddComponent<TransformComponent>(m_player, { .position = { 5, 2 }, .size = { 0.593f, 1.0f } });
    ECS::AddComponent<MovementComponent>(m_player, { .speed = 3 });
    ECS::AddComponent<SpriteComponent>(m_player, { .sprite = SpriteManager::LoadSprite("player", "playerSheet", { 0, 0 }, { 19, 32 }) });
    ECS::AddComponent<ColliderComponent>(m_player, { .type = ColliderType::Dynamic });

    for (int x = -10; x <= 10; x += 5)
    {
        Entity entity = ECS::CreateEntity();
        ECS::AddComponent<TransformComponent>(entity, { .position = { x, 0 }, .size = { 5.0f, 1.001f } });
        ECS::AddComponent<SpriteComponent>(entity, { .sprite = SpriteManager::LoadSprite("ground", "groundSheet", { 0, 0 }, { 160, 33 }) });
        ECS::AddComponent<ColliderComponent>(entity, { .type = ColliderType::Static });
        m_grounds.push_back(entity);
    }

    m_physicsSystem->InitBodies();
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
    m_physicsSystem->Update(delta);

    return this;
}

void Game::OnRender() const
{
    auto playerPos = ECS::GetComponent<TransformComponent>(m_player);
    Renderer::SetRenderOrigin({ playerPos.position.x * 32, playerPos.position.y * 32 });

    m_renderSystem->Update();

    char str[100];
    snprintf(str, 100, "Frame Time: %f", m_delta);
    Renderer::DrawText({ 0.5f, Window::Get()->GetHeight() - 20.0f }, str, m_font, 0.3f);
}
