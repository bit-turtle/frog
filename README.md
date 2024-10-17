# Frog Internet Protocol
Hopping around the Internet
## What is Frog
Frog is a TCP based Internet protocol like `http` and `gopher`
## The Frog Protocol
Rules of the Frog Protocol:
1. Every message has a prefix. Valid prefixes:
    - 'p': Ping (Ask for server info)
        - No arguments
        - Next message must be a server info message
    - 'i': Server Info
        - Arguments: The server name
    - 'r': Server Request
        - Arguments: The page being requested
        - Requests a page from a server, the request is fullfilled when one of the following messages are received:
            1. Response: The page that was requested
            2. Jump: The page should be loaded from another server
            3. Redirect: The page should be loaded from the same server with a different request
    - 's': Server Response
        - Arguments: The page that was requested
    - 'm': Message to display before the response from the server is sent
        - Arguments: The message to display
        - Is sent between a request message and the fullfillment of the request
        - A message is fullfilled with a Next Message
    - 'n': Next Message
        - Wait for the client to finish reading the message
        - Arguments: Message to display
    - 'q': Question for Client
        - Arguments: The question to display
        - Next message must be an answer
    - 'a': Answer from Client
        - Arguments: The answer from the client
    - 'j': Jump to another server
        - Arguments: The IP address to jump to, The page on the server to request
    - 'd': Redirect to another page on the same server
        - Arguments: The name of the page to redirect to
2. Messages are in ASCII
3. Message Structure
    - Prefix (1 Byte/Char)
    - String for arguments:
        - If there is only 1 argument, the entire string is the argument
            - 'The argument can have spaces'
        - If there are 2 arguments, the first argument ends after the first ' ' [Space]
            - 'argument1 argument 2 can have spaces'
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
This project relies on SFML, which will be automatically downloaded by CMake for static linking of the used modules. The SFML Network and System Modules are used.
