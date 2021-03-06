#ifndef ASTEROIDCOMPONENT_HPP
#define ASTEROIDCOMPONENT_HPP

#include "../../Lib/EntitySystem/Component.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class AsteroidComponent : public ses::Component, public sf::Drawable
{
    public:
        AsteroidComponent();

        static std::string getId();

        void setDirection(sf::Vector2f direction);
        sf::Vector2f getDirection() const;

        void setSpeed(float speed);
        float getSpeed() const;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Sprite mSprite;
        sf::Vector2f mDirection;
        float mSpeed;
};

#endif // ASTEROIDCOMPONENT_HPP
