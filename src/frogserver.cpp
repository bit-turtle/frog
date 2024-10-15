#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "frogicol.hpp"

void frogiConnection(sf::TcpSocket* socket) {
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
		if (message.type == PING) {
			if (!send(*socket,info("Frogicol Demo Server"))) {
				connected = false;
				break;
			}
		}
		else if (message.type == REQUEST) {
			if (message.data == "search") {
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
			else {
				send(*socket,response("Document Not Found: " + message.data));
			}
		}
	}
	std::cout << "[Disconnected] " << address << std::endl;
	delete socket;
}

int main() {
	sf::TcpListener listener;
	listener.listen(32313);
	while (true) {
		sf::TcpSocket* socket = new sf::TcpSocket;
		listener.accept(*socket);
		sf::Thread* thread = new sf::Thread(frogiConnection, socket);
		thread->launch();
	}
	return 0;
}
