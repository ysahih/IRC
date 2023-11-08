# IRC

<p align="center">  <img src="Assets/IrcExample.png" </p>

# Definition:
Internet Relay Chat or IRC is a text-based communication protocol on the Internet, It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.
IRC clients connect to IRC servers in order connect with each other and to join channels.

# Summary:
This project is about creating your own IRC server. You will use an actual IRC client to connect to your server and test it. Internet is ruled by solid standards protocols that allow connected computers to interact with each other.

# Project features:
- You must be able to register and to authenticate; to define a nickname, a username.
- You should be able to join a channel, send or receive private messages.
- Every message sent by a client in a channel must be received by all of the clients who have joined this channel.
- you can change channel modes and set operator privileges.

* To install and run IRC on your local machine, follow these steps:
    Clone the repository into your machine using : git clone https://github.com/ysahih/IRC.
    Navigate into the repository directory using cd IRC.
    Run the make command to compile the project using make.
    Your executable will be run as follows: `./ircserv <port> <password>`
        - port : The port number on which your IRC server will be listening to for incoming IRC connections.
        - password: The connection password. It will be needed by any IRC client that tries to connect to your server.

# Configuration:
- the IRC server in built with C++98.
- The server is capable of handling multiple clients at the same time and never hang.
- All I/O operations are non-blocking.
- Communication between client and server is done via TCP/IP(v4).
- ctrl+D is handled to send the command in several parts.
- LimeChat is used as a refrence client to our IRC server.


# Commands:
| Command | Description |
| :-----------: | :----------- |
| Pass | The `PASS` command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. |
| Nick | The `NICK` command is used to give the client a nickname or change the previous one. |
| User | The `USER` command is used at the beginning of a connection to specify the username and realname of a new user. |
| Privmsg | The `PRIVMSG` command is used to send private messages between users, as well as to send messages to channels. |
| Join | The `JOIN` command indicates that the client wants to join the given channel(s), each channel using the given key for it. |
| Topic | The `TOPIC` command is used to change or view the topic of the given channel. |
| Invite | The `INVITE` command is used to invite a user to a channel. |
| Kick | The `KICK` command can be used to request the forced removal of a user from a channel. |
| Mode | The `MODE` command is used to set or remove options (or modes) from a given target. Channels modes: l,k,t,i,o.|
| BOT | The `BOT` command can be used with argument `DATE` to display the present time, and with the argument `HELP` to display a manual of all the commands |
| Ping | The `PING` command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer. |
| Part | The `PART` command removes the client from the given channel(s). |
| Quit | The `QUIT` command is used to terminate a client’s connection to the server.


# Ressources
* Internet Relay Chat : http://chi.cs.uchicago.edu/chirc/index.html | https://www.rfc-editor.org/rfc/rfc1459.html | https://beej.us/guide/bgnet/html/
* Modern IRC Client Protocol : https://modern.ircdocs.horse/
* Command Reference : https://dd.ircdocs.horse/refs/commands/
