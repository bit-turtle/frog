#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "frogicol.hpp"

void frogConnection(sf::TcpSocket* socket) {
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
			else {	// Page not found page
				if (!send(*socket,response("Document Not Found: " + message.data + "\nGo To Page '' for homepage"))) {
					connected = false;
					break;
				}
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
		sf::Thread* thread = new sf::Thread(frogConnection, socket);
		thread->launch();
	}
	return 0;
}
