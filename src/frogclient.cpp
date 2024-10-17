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
	std::cout << "[Response] " << name << std::endl;
	std::cout << "--------" << std::endl;
	std::cout << data << std::endl;
	std::cout << "--------" << std::endl;
}

void displaymessage(std::string name, std::string data) {
	std::cout << "[Message] " << name << std::endl;
	std::cout << "--------" << std::endl;
	std::cout << data << std::endl;
	std::cout << "--------" << std::endl;
}

int main() {
// Connect to server
	sf::TcpSocket socket;
	std::string address;
	bool running = true;
	bool prompt = true;
	bool userip = true;
	while (running) {
		if (userip) address = ask("Server IP Address");
		else userip = true;
		socket.connect(sf::IpAddress(address), frogiport);
		bool frogserver = true;
		// Get Server Info
		FrogiMessage info;
		if (send(socket,ping())) {
			info = receive(socket);
			if (	info.bad ||
				info.type != INFO
			) frogserver = false;
		}
		else frogserver = false;
		if (frogserver) {
			bool connected = true;
			std::string location = "";
			std::cout << "[Connected] " << address << std::endl;
			std::cout << "[Info] " << info.data << std::endl;	// Print Server Info
			// Connected!
			bool prompt = true;	// Prompt user for page to go to
			while (connected) {
				if (prompt) location = ask("Go To Page");
				else prompt = true;
				if (!send(socket,request(location))) {
					connected = false;
					break;
				}
				FrogiMessage message = receive(socket);
				if (message.bad) {
					connected = false;
					break;
				}
				while (	// Message is not a request fullfilling message
					message.type != RESPONSE &&
					message.type != REDIRECT &&
					message.type != JUMP &&
					message.type != HOP
				) {
					if (message.type == MESSAGE) {
						displaymessage(location, message.data);
						std::cout << "Press enter to continue: ";
						{
						std::string trash;
						getline(std::cin,trash);	// Enter press
						}
						if (!send(socket,next())) {
							connected = false;
							break;
						}
					}
					else if (message.type == QUESTION) {
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
				if (message.type == RESPONSE) display(location, message.data);
				else if (message.type == JUMP || message.type == HOP) {
					std::stringstream jumpmessage(message.data);
					jumpmessage >> address;	// Extract IP Address as first argument
					location = "";
					if (message.type == JUMP) {
						location = jumpmessage.str().substr(jumpmessage.tellg());	// Read rest of message into location
						prompt = false;
						std::cout << "[Jump] " << address << std::endl;
					}
					else std::cout << "[Hop] " << address << std::endl;
					userip = false;
					socket.disconnect();
					connected = false;
				}
				else if (message.type == REDIRECT) {
					location = message.data;
					prompt = false;
					std::cout << "[Redirect] " << location << std::endl;
				}
			}
			std::cout << "[Disconnected]" << std::endl;
		}
		else std::cout << "[Connection Failed] " << address << std::endl;
	}
	return 0;
}
