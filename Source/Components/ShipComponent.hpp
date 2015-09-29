#ifndef SHIPCOMPONENT_HPP
#define SHIPCOMPONENT_HPP

#include "../../Lib/EntitySystem/Component.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class ShipComponent : public ses::Component, public sf::Drawable
{
    public:
        ShipComponent();

        static std::string getId();

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Sprite mSprite;
};

#endif // SHIP_HPP