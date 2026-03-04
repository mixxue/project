#include "Enemy.h"
#include <iostream>
#include <cmath>
#include <algorithm>

float distance(Vector2 a, Vector2 b)
{
    return std::sqrt(
        (a.x-a.x)*(a.x-b.x) +
        (a.y-b.y)*(a.y-b.y)
    );
}

Enemy::Enemy(float x,float y)
{
    position = {x,y};

    hp = 200;

    baseSpeed = 40;

    currentSpeed = baseSpeed;

    alive = true;
}

void Enemy::takeDamage(float dmg)
{
    hp -= dmg;

    if(hp <= 0)
    {
        alive = false;
        std::cout<<"Enemy Dead\n";
    }
}

void Enemy::applyEffect(StatusEffect effect)
{
    effects.push_back(effect);
}

void Enemy::update(float dt)
{
    currentSpeed = baseSpeed;

    for(auto& ef : effects)
    {
        ef.duration -= dt;

        switch(ef.type)
        {
            case BURN:
                takeDamage(ef.value * dt);
                break;

            case SLOW:
                currentSpeed = baseSpeed * (1 - ef.value);
                break;

            case STUN:
                currentSpeed = 0;
                break;
        }
    }

    effects.erase(
        std::remove_if(
            effects.begin(),
            effects.end(),
            [](StatusEffect& e)
            {
                return e.duration <= 0;
            }),
        effects.end()
    );

    position.x += currentSpeed * dt;
}