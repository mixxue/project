#pragma once
#include <vector>

struct Vector2
{
    float x;
    float y;
};

float distance(Vector2 a, Vector2 b);

enum StatusType
{
    BURN,
    SLOW,
    STUN
};

struct StatusEffect
{
    StatusType type;
    float duration;
    float value;
};

class Enemy
{
public:

    Vector2 position;

    float hp;
    float baseSpeed;
    float currentSpeed;

    bool alive;

    std::vector<StatusEffect> effects;

public:

    Enemy(float x, float y);

    void takeDamage(float dmg);

    void applyEffect(StatusEffect effect);

    void update(float dt);
};