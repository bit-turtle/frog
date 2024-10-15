#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Network.hpp>

#include "frogicol.hpp"

std::string ask(std::string question) {
	std::cout << "[Question] " << question << ": ";
	std::string answer;
	getline(std::cin, answer);
	return answer;
}

void display(std::string name, std::string data) {
	std::cout << "[Response] " << name << ":" << std::endl;
	std::cout << "--------" << std::endl;
	std::cout << data << std::endl;
	std::cout << "--------" << std::endl;
}

int main() {
// Connect to server
	sf::TcpSocket socket;
	std::string address;
	address = ask("Server IP Address");
	socket.connect(sf::IpAddress(address), 32313);
	bool frogserver = true;
	// Get Server Info
	FrogiMessage info;
	if (send(socket,ping())) {
		info = receive(socket);
		if (	info.bad ||
			info.type != INFO
		) frogserver = false;
	}
	if (frogserver) {
		bool connected = true;
		std::string location = "";
		std::cout << "[Connected] " << address << std::endl;
		std::cout << "[Info] " << info.data << std::endl;	// Print Server Info
		// Connected!
		while (connected) {
			location = ask("Go To Page");
			if (!send(socket,request(location))) {
				connected = false;
				break;
			}
			FrogiMessage message = receive(socket);
			if (message.bad) {
				connected = false;
				break;
			}
			while (message.type != RESPONSE) {
				if (message.type == QUESTION) {
					std::string ans = ask(message.data);
					if (!send(socket,answer(ans))) {
						connected = false;
						break;
					}
				}
				message = receive(socket);
				if (message.bad) {
					connected = false;
					break;
				}
			} if (!connected) break;
			display(location, message.data);
		}
		std::cout << "[Disconnected]" << std::endl;
	}
	else std::cout << "[Connection Failed] " << address << std::endl;
	return 0;
}
