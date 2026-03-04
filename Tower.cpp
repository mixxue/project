#include "Tower.h"
#include <iostream>

Tower::Tower(Vector2 pos,float dmg,float atkSpeed,Element* e)
{
    position = pos;

    damage = dmg;

    attackSpeed = atkSpeed;

    cooldown = 0;

    element = e;
}

void Tower::attack(
Enemy* target,
std::vector<Enemy*>& enemies)
{
    if(!target->alive) return;

    float finalDamage =
    damage * element->damageModifier();

    target->takeDamage(finalDamage);

    element->onApply(target,enemies);
}

void Tower::update(
float dt,
std::vector<Enemy*>& enemies)
{
    cooldown -= dt;

    if(cooldown <= 0 && !enemies.empty())
    {
        Enemy* target = enemies.front();

        attack(target,enemies);

        cooldown = attackSpeed;
    }
}