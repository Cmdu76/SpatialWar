#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/View.hpp>

#include <SFML/Network/TcpSocket.hpp>

#include "EntityManager.hpp"
#include "../Lib/EntitySystem/SystemManager.hpp"

#include "Configuration.hpp"
#include "Space.hpp"

#include "Systems.hpp"
#include "Components.hpp"

#include "Server/PacketType.hpp"
#include "Server/Message.hpp"

class World
{
    public:
        static void init();
        static void terminate();

        static void handleEvent(sf::Event const& event);
        static void update(sf::Time dt);
        static void render(sf::RenderTarget& target, sf::RenderStates states);

        static bool isClient();
        static bool isServer();

        static sf::TcpSocket& getSocket();
        static ses::EntityManager& getEntities();
        static ses::SystemManager& getSystems();
        static ah::ResourceHolder& getResources();
        static sf::View& getView();
        static Space& getSpace();

    private:
        World();
        ~World();

        static World mInstance;

        #ifdef SW_CLIENT
        sf::TcpSocket mSocket;
        #endif

        ses::EntityManager::Ptr mEntities;
        ses::SystemManager mSystems;
        ah::ResourceHolder mResources;
        sf::View mView;
        Space mSpace;
};

#endif // WORLD_HPP
