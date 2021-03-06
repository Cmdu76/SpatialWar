#include "BaseComponent.hpp"

BaseComponent::BaseComponent()
: ses::Component()
, lp::CollisionShape()
{
    //ctor
}

std::string BaseComponent::getId()
{
    return "BaseComponent";
}

float BaseComponent::getLife() const
{
    return mLife;
}

void BaseComponent::setLife(float life)
{
    mLife = life;
}

float BaseComponent::getLifeMax() const
{
    return mLifeMax;
}

void BaseComponent::setLifeMax(float lifeMax)
{
    mLifeMax = lifeMax;
    mLife = std::max(mLife,mLifeMax);
}

bool BaseComponent::inflige(float damage)
{
    mLife = std::max(mLife-damage,0.f);
    return isDead();
}

bool BaseComponent::restore(float restore)
{
    mLife = std::min(mLife+restore,mLifeMax);
    return isFullLife();
}

void BaseComponent::restoreFullLife()
{
    mLife = mLifeMax;
}

bool BaseComponent::isFullLife() const
{
    return mLife == mLifeMax;
}

bool BaseComponent::isDead() const
{
    return mLife <= 0.f;
}

float BaseComponent::getMass() const
{
    return mMass;
}

void BaseComponent::setMass(float mass)
{
    mMass = mass;
}

float BaseComponent::getSpeed() const
{
    return mSpeed;
}

void BaseComponent::setSpeed(float speed)
{
    mSpeed = speed;
}

sf::Vector2f BaseComponent::getDirection() const
{
    return mDirection;
}

void BaseComponent::setDirection(sf::Vector2f direction)
{
    mDirection = direction;
}
