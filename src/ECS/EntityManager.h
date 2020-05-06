#pragma once

#include <cstdint>
#include <bitset>
#include <queue>
#include <array>

#include "Core/Common.h"

#define MAX_ENTITIES 5000

using Entity = uint32_t;
using Signature = std::bitset<MAX_ENTITIES>;

class EntityManager
{
public:
    EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
        {
            s_availableEntities.push(entity);
        }
    }

    Entity CreateEntity()
    {
        ASSERT(s_livingEnityCount < MAX_ENTITIES, "Too many entities in existance!");

        Entity id = s_availableEntities.front();
        s_availableEntities.pop();
        s_livingEnityCount++;

        return id;
    }

    void DestroyEntity(Entity entity)
    {
        ASSERT(entity < MAX_ENTITIES, "Entity is out of range!");

        s_signatures[entity].reset();

        s_availableEntities.push(entity);
        s_livingEnityCount--;
    }

    void SetSignature(Entity entity, Signature signature)
    {
        ASSERT(entity < MAX_ENTITIES, "Entity is out of range!");

        s_signatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) 
    {
        ASSERT(entity < MAX_ENTITIES, "Entity is out of range!"); 
        return s_signatures[entity]; 
    }
private:
    std::queue<Entity> s_availableEntities = {};
    std::array<Signature, MAX_ENTITIES> s_signatures = {};

    uint32_t s_livingEnityCount;
};
