#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"

#include "Core/Common.h"

class ECS
{
public:
    static void Init()
    {
        m_entityManager = std::make_shared<EntityManager>();
        m_componentManager = std::make_shared<ComponentManager>();
        m_systemManager = std::make_shared<SystemManager>();
    }

    static void Shutdownn()
    {
        m_componentManager->Shutdown();
    }

    static Entity CreateEntity()
    {
        return m_entityManager->CreateEntity();
    }

    static void DestroyEntity(Entity entity)
    {
        m_entityManager->DestroyEntity(entity);
        m_componentManager->EntityDestroyed(entity);
        m_systemManager->EntityDestroyed(entity);
    }

    template <typename T>
    static void RegisterComponent()
    {
        m_componentManager->RegisterComponent<T>();
    }

    template <typename T>
    static void AddComponent(Entity entity, T component)
    {
        m_componentManager->AddComponent<T>(entity, component);

        auto signature = m_entityManager->GetSignature(entity);
        signature.set(m_componentManager->GetComponentType<T>(), true);
        m_entityManager->SetSignature(entity, signature);

        m_systemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    static void RemoveComponent(Entity entity)
    {
        m_componentManager->RemoveComponent<T>(entity);

        auto signature = m_entityManager->GetSignature(entity);
        signature.set(m_componentManager->GetComponentType<T>(), false);
        m_entityManager->SetSignature(entity, signature);

        m_systemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    static T& GetComponent(Entity entity)
    {
        return m_componentManager->GetComponent<T>(entity);
    }

    template <typename T>
    static ComponentType GetComponentType()
    {
        return m_componentManager->GetComponentType<T>();
    }

    template <typename T>
    static Ref<T> RegisterSystem()
    {
        return m_systemManager->RegisterSystem<T>();
    }

    template <typename T>
    static void SetSystemSignature(Signature signature)
    {
        m_systemManager->SetSignature<T>(signature);
    }
private:
    static Ref<EntityManager> m_entityManager;
    static Ref<ComponentManager> m_componentManager;
    static Ref<SystemManager> m_systemManager;
};