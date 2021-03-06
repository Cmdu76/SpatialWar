#include "Server.hpp"

namespace srv
{

Server::Server()
: mThread(&Server::run,this)
, mRunning(false)
, mListener()
, mListeningState(false)
, mSettings()
, mConnectedPlayers(0)
, mPeers(1)
{

    mMaxPlayers = mSettings.getInt("maxplayers");
    mPort = mSettings.getInt("port");
    mClientTimeoutTime = sf::seconds(3.0f);
    mUpdateInterval = sf::seconds(1.f/60.f);

    mListener.setBlocking(false);
	mPeers[0].reset(new Peer());
}

void Server::start()
{
    ah::Application::instance() << "[Server] Game Server Version 0.1";
	ah::Application::instance() << "[Server]  - Max Players : " + lp::to_string(mMaxPlayers);
	ah::Application::instance() << "[Server]  - Server Port : " + lp::to_string(mPort);

    ah::Application::instance() << "[Server] Starting server...";

    mRunning = true;

    mThread.launch();

    ah::Application::instance() << "[Server] Server started !";
}

void Server::stop()
{
    ah::Application::instance() << "[Server] Stopping server";

    setListening(false);

    sf::Packet packet;
    packet << Packet::ServerStopped;
    sendToAll(packet);

    mRunning = false;

    mThread.wait();

    ah::Application::instance() << "[Server] Server stopped !";
}

bool Server::isRunning() const
{
    return mRunning;
}

void Server::sendToAll(sf::Packet& packet)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mPeers[i]->isReady())
        {
            mPeers[i]->send(packet);
        }
    }
}

void Server::sendToPeer(sf::Packet& packet, unsigned int peerId)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mPeers[i]->isReady() && mPeers[i]->getId() == peerId)
        {
            mPeers[i]->send(packet);
        }
    }
}

void Server::sendToPeer(sf::Packet& packet, Peer& peer)
{
    if (peer.isReady())
    {
        peer.send(packet);
    }
}

Settings& Server::getSettings()
{
    return mSettings;
}

void Server::setListening(bool enable)
{
    if (enable) // Check if it isn't already listening
	{
		if (!mListeningState)
        {
            mListeningState = (mListener.listen(mPort) == sf::TcpListener::Done);
        }
	}
	else
	{
		mListener.close();
		mListeningState = false;
	}
}

void Server::run()
{
    setListening(true);

    sf::Clock updateClock;

    while (isRunning())
    {
        handlePackets();
        handleConnections();

        if (updateClock.getElapsedTime() > mUpdateInterval)
        {
            update(mUpdateInterval);
            updateClock.restart();
        }

        sf::sleep(sf::milliseconds(100));
    }
}

void Server::update(sf::Time dt)
{

}

void Server::handlePackets()
{
    bool detectedTimeout = false;

	for (unsigned int i = 0; i < mConnectedPlayers; i++)
	{
		if (mPeers[i]->isReady())
		{
			sf::Packet packet;
			while (mPeers[i]->receive(packet) == sf::Socket::Done)
			{
                handlePacket(packet,*mPeers[i], detectedTimeout);
				packet.clear();
			}

			if (mPeers[i]->getLastPacketTime() > mClientTimeoutTime)
			{
				mPeers[i]->setTimedOut(true);
				detectedTimeout = true;
			}
		}
	}

	if (detectedTimeout)
		handleDisconnections();
}

void Server::handlePacket(sf::Packet& packet, Peer& peer, bool& detectedTimeout)
{
    sf::Int32 packetType;
    packet >> packetType;

    switch (packetType)
    {
        default: break;
    }
}

void Server::handleConnections()
{
    if (!mListeningState)
		return;

	if (mListener.accept(mPeers[mConnectedPlayers]->getSocket()) == sf::TcpListener::Done)
	{
		mPeers[mConnectedPlayers]->setReady(true);
		mConnectedPlayers++;

		if (mConnectedPlayers >= mMaxPlayers)
			setListening(false);
		else
			mPeers.push_back(Peer::Ptr(new Peer()));
	}
}

void Server::handleDisconnections()
{
    for (auto itr = mPeers.begin(); itr != mPeers.end(); )
	{
		if ((*itr)->hasTimedOut())
		{
            if ((*itr)->isConnected())
            {
                std::string name = (*itr)->getName();
                ah::Application::instance() << "[Server] " + name + " left the game";

                sf::Packet packet;
                packet << Packet::ClientLeft << name;
                sendToAll(packet);

                (*itr)->setName("");
                (*itr)->setReady(false);
            }

			mConnectedPlayers--;

			itr = mPeers.erase(itr);

			if (mConnectedPlayers < mMaxPlayers)
			{
				mPeers.push_back(Peer::Ptr(new Peer()));
				setListening(true);
			}
        }
		else
		{
			++itr;
		}
	}
}

} // namespace srv
