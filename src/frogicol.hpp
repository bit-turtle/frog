#pragma once

#include <string>
#include <sstream>
#include <cstdint>

#include <SFML/Network.hpp>

enum FrogiType {
	REQUEST = 'r',
	RESPONSE = 's',
	QUESTION = 'q',
	ANSWER = 'a',
	PING = 'p',
	INFO = 'i',
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
FrogiMessage info();
// 

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
			message.type != PING &&
			message.type != INFO &&
			message.type != ANSWER
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
