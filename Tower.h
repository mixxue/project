#pragma once

#include "Enemy.h"
#include "Element.h"

class Tower
{
protected:

    Vector2 position;

    float damage;

    float range;

    float attackSpeed;

    float cooldown;

    Element* element;

public:

    Tower(Vector2 pos,float dmg,float atkSpeed,Element* e);

    void attack(
        Enemy* target,
        std::vector<Enemy*>& enemies);

    void update(
        float dt,
        std::vector<Enemy*>& enemies);
};