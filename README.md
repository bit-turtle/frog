# Frog Internet Protocol
Hopping around the Internet
## What is Frog
Frog is a TCP based Internet protocol like `http` and `gopher`
## The Frog Protocol
Rules of the Frog Protocol:
1. Every message has a prefix. Valid prefixes:
    - 'p': Ping (Ask for server info)
    - 'i': Server Info
    - 'r': Server Request
    - 's': Server Response
    - 'q': Question for Client
    - 'a': Answer from Client
2. Messages are in ASCII
3. Message Structure
    - Prefix (1 Byte/Char)
    - String
4. After connecting to a server via TCP, the client must send a ping message
## Example Message Log
1. Client: "*p*": Ping Server
2. Server: "*i*Frog Protocol Search Engine": The server says it's name is "Frog Protocol Search Engine"
3. Client: "*r*search": Client requests the "search" page
4. Server: "*q*Search Query": Server asks Client for a "Search Query"
5. Client: "*a*Frog Protocol": Client Answers to the "Search Query" Question with "Frog Protocol"
6. Server: "*s*Frog Protocol is an internet protocol like http": The Server Responds with the Page the Client Requested
## What is in this repository
1. A Frog Protocol Client (`frogclient`)
    - Works out of the box
2. A Frog Protocol Demo Server (`frogserver`)
    - The source would need to be modified to configure the server
## Building
This project uses CMake, build with `cmake .`
## Dependencies
This project relies on SFML, which will be automatically downloaded by CMake for static linking of the used modules.
