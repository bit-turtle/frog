#pragma once

bool debug = false;	// Print TCP Messages

#include <string>
#include <sstream>
#include <cstdint>

#include <SFML/Network.hpp>

int frogiport = 32313;

enum FrogiType {
	REQUEST = 'r',
	RESPONSE = 's',
	QUESTION = 'q',
	ANSWER = 'a',
	PING = 'p',
	INFO = 'i',
	MESSAGE = 'm',
	NEXT = 'n',
	REDIRECT = 'd',
	JUMP = 'j',
	HOP = 'h'
};

struct FrogiMessage {
	bool bad = false;
	FrogiType type;
	std::string data;
};

// Forward Declare Utils
FrogiMessage request(std::string);
FrogiMessage response(std::string);
FrogiMessage question(std::string);
FrogiMessage answer(std::string);
FrogiMessage ping();
FrogiMessage info(std::string);
// For No Reason

FrogiMessage
strToMessage(std::string string) {
	FrogiMessage message;
	if (string.size() == 0) message.bad = true;
	else {
		message.type = static_cast<FrogiType>(string.at(0));
		if (
			message.type != REQUEST &&
			message.type != RESPONSE &&
			message.type != QUESTION &&
			message.type != ANSWER &&
			message.type != PING &&
			message.type != INFO &&
			message.type != MESSAGE &&
			message.type != NEXT &&
			message.type != JUMP &&
			message.type != REDIRECT
		) message.bad = true;
		message.data = string.substr(1, string.length() - 1);
	}
	return message;
}

std::string
messageToStr(FrogiMessage message) {
	std::string string;
	string += static_cast<char>(message.type);
	string += message.data;
	return string;
}

bool send(sf::TcpSocket& socket, FrogiMessage message) {
	std::string string = messageToStr(message);
	if (debug) std::cout << "[DEBUG] Sending: " << string << std::endl;
	sf::Socket::Status status = socket.send(string.c_str(), string.size() + 1);
	if (status != sf::Socket::Status::Done) {
		return false;
	}
	return true;
}

char buffer[8168];
FrogiMessage receive(sf::TcpSocket& socket) {
	FrogiMessage message;
	std::size_t received = 0;
	sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
	std::string string = buffer;
	if (debug) std::cout << "[DEBUG] Received: " << string << std::endl;
	if (status != sf::Socket::Status::Done) {
		message.bad = true;
		return message;
	}
	else {
		message = strToMessage(string);
	}
	return message;
}

// Utils
FrogiMessage
request(std::string string) {
	FrogiMessage message;
	message.type = REQUEST;
	message.data = string;
	return message;
}
FrogiMessage
response(std::string string) {
	FrogiMessage message;
	message.type = RESPONSE;
	message.data = string;
	return message;
}
FrogiMessage
question(std::string string) {
	FrogiMessage message;
	message.type = QUESTION;
	message.data = string;
	return message;
}
FrogiMessage
answer(std::string string) {
	FrogiMessage message;
	message.type = ANSWER;
	message.data = string;
	return message;
}
FrogiMessage
ping() {
	FrogiMessage message;
	message.type = PING;
	message.data = "";
	return message;
}
FrogiMessage
info(std::string string) {
	FrogiMessage message;
	message.type = INFO;
	message.data = string;
	return message;
}
FrogiMessage
msg(std::string string) {
	FrogiMessage message;
	message.type = MESSAGE;
	message.data = string;
	return message;
}
FrogiMessage
next() {
	FrogiMessage message;
	message.type = NEXT;
	message.data = "";
	return message;
}
FrogiMessage
redirect(std::string string) {
	FrogiMessage message;
	message.type = REDIRECT;
	message.data = string;
	return message;
}
FrogiMessage
jump(std::string string) {
	FrogiMessage message;
	message.type = JUMP;
	message.data = string;
	return message;
}
FrogiMessage
hop(std::string string) {
	FrogiMessage message;
	message.type = HOP;
	message.data = string;
	return message;
}
