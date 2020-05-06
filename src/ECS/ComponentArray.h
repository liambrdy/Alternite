#pragma once

#include <array>
#include <unordered_map>

#include "ECS/EntityManager.h"

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
    ~ComponentArray()
    {
        for (int i = 0; i < m_size; i++)
        {
            m_componentArray[i].~T();
        }
    }

    void InsertData(Entity entity, T component)
    {
        ASSERT(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end(), "Component added to same entity more than once!");
        
        size_t newIndex = m_size;
        m_entityToIndexMap[entity] = newIndex;
        m_indexToEntityMap[newIndex] = entity;
        m_componentArray[newIndex] = component;
        m_size++;
    }

    void RemoveData(Entity entity)
    {
        ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Removing non-existent component!");

        size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
        size_t indexOfLastEntity = m_size - 1;
        m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastEntity];

        Entity entityOfLastElement = m_indexToEntityMap[indexOfLastEntity];
        m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        m_entityToIndexMap.erase(entity);
        m_indexToEntityMap.erase(indexOfLastEntity);

        m_size--;
    }

    T& GetData(Entity entity)
    {
        ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Retrieving non-existent component!");

        return m_componentArray[m_entityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) override
    {
        if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
        {
            RemoveData(entity);
        }
    }
private:
    std::array<T, MAX_ENTITIES> m_componentArray;
    
    std::unordered_map<Entity, size_t> m_entityToIndexMap;
    std::unordered_map<size_t, Entity> m_indexToEntityMap;

    size_t m_size;
};