#pragma once

#include <set>
#include <unordered_map>

#include "ECS/EntityManager.h"

class System
{
public:
    std::set<Entity> m_entites;
};

class SystemManager
{
public:
    template <typename T>
    Ref<T> RegisterSystem()
    {
        const char* typeName = typeid(T).name();

        ASSERT(m_systems.find(typeName) == m_systems.end(), "Registering system more than once!");

        auto system = std::make_shared<T>();
        m_systems.insert({ typeName, system });
        return system;
    }

    template <typename T>
    void SetSignature(Signature signature)
    {
        const char* typeName = typeid(T).name();

        ASSERT(m_systems.find(typeName) != m_systems.end(), "System used before registered!");

        m_signatures.insert({ typeName, signature });
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const& pair : m_systems)
        {
            auto const& system = pair.second;
            system->m_entites.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for (auto const& pair : m_systems)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = m_signatures[type];

            if ((entitySignature & systemSignature) == systemSignature)
            {
                system->m_entites.insert(entity);
            }
            else
            {
                system->m_entites.erase(entity);
            }
            
        }
    }
private:
    std::unordered_map<const char*, Signature> m_signatures = {};
    std::unordered_map<const char*, Ref<System>> m_systems = {};
};