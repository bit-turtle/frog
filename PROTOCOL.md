# Frog Protocol Specification
Contains all the specifics on the frog protocol
## Parsing Messages
Messages in frog are encoded in ASCII (`char`). Messages in frog begin with a single character as the header, the header contains information on what type of message it is. After the header, messages may have no arguments, 1 argument, or 2 arguments.
* No Arguments: The message has nothing after the header, and if there is something, it is ignored.
    - *h*no one cares what is here
* 1 Argument: The entire rest of the message after the header is the argument
    - *h*the argument goes all the way to the end, if there is nothing after the header, the argument is ""
* 2 Arguments: The first argument spans from the first character after the header to the first space character in the message. The second argument spans from the character after the first space in the message to the end of the message
    - *h*argument1 argument 2 goes all the way to the end
## Headers
Headers in the frog protocol communicate the purpose of a message. There are 2 types of headers, headers that begin a conversation, and headers that end the conversation, there can be conversations inside of conversations. There are multiple levels of header, there are rootheaders are able to be used when no conversation in going on and subheaders which can only be used inside of a specific conversation.
* Ping
    - Asks the server what it's name is
    - Used by: Client
    - Character: 'p'
    - Starts a conversation (Ping)
    - Is a rootheader
    - Arguments: None
* Info
    - Tells the client the name of the server
    - Used by: Server
    - Character: 'i'
    - Ends a conversation (Ping)
    - Is a subheader (Ping)
    - Arguments: Server Name
* Request:
    - Requests a page from a server
    - Used by: Client
    - Character: 'r'
    - Starts a conversation (Request)
    - Is a rootheader
    - Arguments: Page Name
* Question:
    - Asks the client a question
    - Used by: Server
    - Character: 'q'
    - Starts a subconveration (Question)
    - Is a subheader (Request)
    - Arguments: Question Prompt
* Answer
    - Answers the question asked by the server
    - Used by: Client
    - Character: 'a'
    - Ends a subconversation (Question)
    - Is a subheader (Question)
    - Arguments: Question Response
* Message
    - Sends an intermediate message to the client
    - Used by: Server
    - Character: 'm'
    - Starts a subconversation (Message)
    - Is a subheader (Request)
    - Arguments: Message Text
* Next
    - Tells the server that the intermediate message has been read and the server should continue sending messages
    - Used by: Client
    - Character: 'n'
    - Ends a subconversation (Message)
    - Is a subheader (Message)
    - Arguments: None
* Response:
    - Returns the page requested by the client
    - Used by: Server
    - Character: 's'
    - Ends a conversation (Request)
    - Is a subheader (Request)
    - Arguments: Page Text
* Jump:
    - Tells the client to connect to a different server and request a page
    - Used by: Server
    - Character: 'j'
    - Ends a conversation (Request)
    - Is a subheader (Request)
    - Arguments: Server IP Address, Page Name
* Hop:
    - Tells the client to connect to a different server
    - Used by: Server
    - Character: 'h'
    - Ends a conversation (Request)
    - Is a subheader (Request)
    - Arguments: Server IP Address
* Redirect:
    - Tells the client to request a different page
    - Used by: Server
    - Character: 'd'
    - Ends a conversation (Request)
    - Is a subheader (Request)
    - Arguments: Page Name
## Example Frog Connection Transcripts
### Basic Frog Request
* Client connects to Server
* Client: p
* Server: iFrogipedia
* Client: rfrog
* Server: sFrog is a protocol like http and gopher
### Frogmail Login
* Client connects to Server
* Client: p
* Server: iFrogmail
* Client: r
* Server: qUsername
* Client: afrog
* Server: qPassword
* Client: averysecurepassword
* Server: sYou have no new messages
### Frog Search Engine
* Client connects to Server
* Client: p
* Server: iFrog Search Engine
* Client: rsearch
* Server: qSearch Query
* Client: adogs
* Server: mResults:\n(0)poodle\n(1)chihuahua
* Client: n
* Server: qChoose a Result
* Client: a0
* Server: mJumping to frog://poodles.com/what are poodles?
* Client: n
* Server: jsome.different.IP.Address what are poodles?
* Client disconnects from Server
* Client connects to server at new IP Address
* Client: rwhat are poodles?
* Server: sPoodles are a type of dog
