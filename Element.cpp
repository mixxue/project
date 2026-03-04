#include "Element.h"
#include <iostream>

void Element::onApply(
Enemy* target,
std::vector<Enemy*>& enemies)
{
}

float Element::damageModifier()
{
    return 1.0f;
}

void FireElement::onApply(
Enemy* target,
std::vector<Enemy*>& enemies)
{
    StatusEffect burn;

    burn.type = BURN;
    burn.duration = 3.0f;
    burn.value = 10;

    target->applyEffect(burn);

    std::cout<<"Burn Applied\n";
}

void WaterElement::onApply(
Enemy* target,
std::vector<Enemy*>& enemies)
{
    StatusEffect slow;

    slow.type = SLOW;
    slow.duration = 2.0f;
    slow.value = 0.4f;

    target->applyEffect(slow);

    std::cout<<"Slow Applied\n";
}

float EarthElement::damageModifier()
{
    return 1.5f;
}