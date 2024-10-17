#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "frogicol.hpp"

struct Connection {
	sf::TcpSocket* socket;
	sf::Thread* thread;
};

sf::Mutex threadlock;
unsigned long threads = 0;
sf::Mutex garbagelock;
std::vector<Connection*> garbage;	// List of connections to garbage collect

void frogConnection(Connection* connection) {
	threadlock.lock();
	threads++;
	threadlock.unlock();
	std::cout << "[Info] Active Threads: " << threads << std::endl;
	sf::TcpSocket* socket = connection->socket;
	bool connected = true;
	std::stringstream addrss;
	addrss << socket->getRemoteAddress();
	std::string address = addrss.str();
	std::cout << "[Connected] " << address << std::endl;
	while (connected) {
		FrogiMessage message = receive(*socket);
		if (message.bad) {
			connected = false;
			break;
		}
		if (message.type == PING) {	// Page Title
			if (!send(*socket,info("Frog Demo Server"))) {
				connected = false;
				break;
			}
		}
		else if (message.type == REQUEST) {
			if (message.data == "search") {	// Search Page
				if (!send(*socket,question("Search Query"))) {
					connected = false;
					break;
				}
				message = receive(*socket);
				if (message.bad) {
					connected = false;
					break;
				}
				if (message.type != ANSWER) {
					connected = false;
					break;
				}
				send(*socket,response("Searched for: " + message.data));
			}
			else if (message.data == "") {	// Homepage
				if (!send(*socket,response("Frog Demo Server:\nTo Search the site, Go To 'search'"))) {
					connected = false;
					break;
				}
			}
			else if (message.data == "redirect") {
				if (!send(*socket,msg("This is a message:\nRedirecting to the search page"))) {
					connected = false;
					break;
				}
				FrogiMessage next = receive(*socket);
				if (next.bad) {
					connected = false;
					break;
				}
				if (next.type != NEXT) {
					connected = false;
					break;
				}
				if (!send(*socket,redirect("search"))) {
					connected = false;
					break;
				}
			}
			else {	// Page not found page
				if (!send(*socket,response("Document Not Found: " + message.data + "\nGo To Page '' for homepage"))) {
					connected = false;
					break;
				}
			}
		}
	}
	std::cout << "[Disconnected] " << address << std::endl;
	garbagelock.lock();
	garbage.push_back(connection);
	garbagelock.unlock();
	threadlock.lock();
	threads--;
	threadlock.unlock();
	std::cout << "[Info] Active Threads: " << threads << std::endl;
}

void collectGarbage() {
	while (true) {
		garbagelock.lock();
		while (garbage.size() > 0) {
			garbage.at(0)->thread->wait();
			delete garbage.at(0)->thread;
			delete garbage.at(0)->socket;
			delete garbage.at(0);
			garbage.erase(garbage.begin());
			std::cout << "[GarbageCollector] Deleted Garbage" << std::endl;
		}
		garbagelock.unlock();
	}
}

int main() {
	sf::Thread garbageCollector(collectGarbage);
	garbageCollector.launch();
	sf::TcpListener listener;
	listener.listen(32313);
	while (true) {	// Forever accept new connections
		Connection* connection = new struct Connection;
		connection->socket = new sf::TcpSocket;
		listener.accept(*connection->socket);
		connection->thread = new sf::Thread(frogConnection, connection);
		connection->thread->launch();
	}
	return 0;
}
